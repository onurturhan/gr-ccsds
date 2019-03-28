/* -*- c++ -*- */
/*
 * gr-ccsds: CCSDS Telemetry and Telecommand Transceivers
 *
 *  Copyright (C) 2019
 *  Libre Space Foundation <https://libre.space>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <gnuradio/filter/firdes.h>
#include <boost/bind.hpp>
#include "ccsds_modulator_impl.h"

namespace gr {
namespace ccsds {

ccsds_modulator::sptr
ccsds_modulator::make(ccsds::ccsds_constellation_sptr constellation, uint8_t pcm)
{
  return gnuradio::get_initial_sptr
    (new ccsds_modulator_impl(constellation, pcm));
}

ccsds_modulator_impl::ccsds_modulator_impl(ccsds::ccsds_constellation_sptr constellation, uint8_t pcm)
  : gr::sync_block("ccsds_modulator",
          gr::io_signature::make(0, 0, 0),
          gr::io_signature::make(1, 1, sizeof(gr_complex))),
          d_pcm(pcm_encoder((pcm_type_t)pcm))
{
  d_remaining = 0;
  d_tx_index = 0;
  d_pdu_port = pmt::mp("PDU_in");
  d_constellation = constellation;
  d_oqpsk_symbol = -1;
  if(d_constellation->get_mod_type() == BPSK){
    d_bits_per_symbol = 1;
  }
  else if (d_constellation->get_mod_type() == OQPSK || d_constellation->get_mod_type() == QPSK){
    d_bits_per_symbol = 2;
  }
  d_pcm_buffer = (uint8_t*)malloc(max_noutput_items()*sizeof(uint8_t));
  message_port_register_in(d_pdu_port);
  set_msg_handler(d_pdu_port,
    boost::bind(&ccsds_modulator_impl::handle_pdu, this, _1));
  if(pcm == BPL){
    d_pcm_rate = 2;
  }
  else{
    d_pcm_rate = 1;
  }
}

ccsds_modulator_impl::~ccsds_modulator_impl()
{
free(d_pcm_buffer);
}

int
ccsds_modulator_impl::work(int noutput_items,
    gr_vector_const_void_star &input_items,
    gr_vector_void_star &output_items)
{
  gr_complex *out = (gr_complex *) output_items[0];
  if(d_buffers.size() == 0){
    return 0;
  }
  size_t noutput_items_mult8 = noutput_items/8;
  noutput_items_mult8 = noutput_items_mult8 * 8;
  if(noutput_items_mult8 == 0){
    return 0;
  }
  size_t return_items = 0;
  if(d_remaining == 0){
    d_remaining = pmt::blob_length(d_buffers.front()) * 8;

  }
  ssize_t ret_pcm_len = 0;
  uint8_t* ptr = (uint8_t*)pmt::blob_data(d_buffers.front());
  if((d_remaining * d_pcm_rate)/d_bits_per_symbol <= noutput_items_mult8){
    ret_pcm_len = d_pcm.encode(d_pcm_buffer, ptr, d_remaining);
    if(ret_pcm_len < 0){
      return 0;
    }
    if(d_constellation->get_mod_type() == OQPSK && d_oqpsk_symbol < 0){
      d_oqpsk_symbol = d_pcm_buffer[0] >> 6;
    }
    modulate_input(out, d_remaining * d_pcm_rate);
    return_items = (d_remaining * d_pcm_rate)/d_bits_per_symbol;
    d_remaining = 0;
    d_tx_index = 0;
    d_oqpsk_symbol = -1;
    d_buffers.pop_front();
  }
  else{
    ret_pcm_len = d_pcm.encode(d_pcm_buffer, &ptr[d_tx_index], (noutput_items_mult8 * d_bits_per_symbol)/d_pcm_rate);
    if(ret_pcm_len < 0){
      return 0;
    }
    if(d_constellation->get_mod_type() == OQPSK && d_oqpsk_symbol < 0){
      d_oqpsk_symbol = d_pcm_buffer[0] >> 6;
    }
    modulate_input(out, noutput_items_mult8*d_bits_per_symbol);
    return_items = noutput_items_mult8;
    d_remaining -= (noutput_items_mult8 * d_bits_per_symbol)/d_pcm_rate;
    d_tx_index += (noutput_items_mult8 * d_bits_per_symbol)/d_pcm_rate/8;
  }
  return return_items;
}

void
ccsds_modulator_impl::handle_pdu(pmt::pmt_t pdu_in){
  d_buffers.push_back(pdu_in);
}

void
ccsds_modulator_impl::modulate_input(gr_complex* out, size_t length){
  switch(d_constellation->get_mod_type()){
    case BPSK:
      for(size_t i = 0; i< length; i++){
        if(d_pcm_buffer[i/8] & (1 << (7 - i%8))){
          out[i] = d_constellation->get_const_pts()[1];
        }
        else{
          out[i] = d_constellation->get_const_pts()[0];
        }
      }
      break;
    case QPSK:
      uint8_t symbol;
      for(size_t i = 0; i< length; i+=2){
        symbol = (d_pcm_buffer[i/8] >> (6 - i%8)) & 0x03;
        out[i] = d_constellation->get_const_pts()[symbol];
      }
      break;
    case OQPSK:
      for(size_t i = 0; i< length; i++){
        if(d_pcm_buffer[i/8] & (1 << (7 - i%8))){
          d_oqpsk_symbol |= (1 << (1 - i%2));
        }
        else{
          d_oqpsk_symbol &= ~(1 << (1 - i%2));
        }
        out[i] = d_constellation->get_const_pts()[d_oqpsk_symbol];
      }
      break;
  }
}

} /* namespace ccsds */
} /* namespace gr */

