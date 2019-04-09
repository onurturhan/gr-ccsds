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
#include "ccsds_demodulator_impl.h"

namespace gr
{
namespace ccsds
{

ccsds_demodulator::sptr
ccsds_demodulator::make (ccsds::ccsds_constellation_sptr constellation,
                         uint8_t pcm)
{
  return gnuradio::get_initial_sptr (
      new ccsds_demodulator_impl (constellation, pcm));
}

/*
 * The private constructor
 */
ccsds_demodulator_impl::ccsds_demodulator_impl(ccsds::ccsds_constellation_sptr constellation, uint8_t pcm)
: gr::block("ccsds_demodulator",
    gr::io_signature::make(1, 1, sizeof(gr_complex)),
    gr::io_signature::make(1, 1, sizeof(int8_t))),
    d_pcm(pcm_decoder((pcm_type_t)pcm)),
    d_constellation(constellation)
{
  if(d_constellation->get_mod_type()==QPSK || d_constellation->get_mod_type()==OQPSK){
    d_bits_per_symbol = 2;
  }
  else{
    d_bits_per_symbol = 1;
  }
  if(pcm == BPL){
    d_pcm_rate = 2;
  }
  else{
    d_pcm_rate = 1;
  }
  set_max_noutput_items(8193);
  d_pcm_buffer = (int8_t*)malloc(max_noutput_items()*sizeof(uint8_t));
  memset(d_pcm_buffer, 0, max_noutput_items()*sizeof(int8_t));
  printit = false;

  //set_relative_rate(1.0);
}

/*
 * Our virtual destructor.
 */
ccsds_demodulator_impl::~ccsds_demodulator_impl ()
{
  free(d_pcm_buffer);
}

void
ccsds_demodulator_impl::forecast(int noutput_items,
                                  gr_vector_int &ninput_items_required)
{
  unsigned ninputs = ninput_items_required.size ();
  for(unsigned i = 0; i < ninputs; i++){

    ninput_items_required[i] = (int) (noutput_items*d_pcm_rate/d_bits_per_symbol +0.5);
  }
}

int
ccsds_demodulator_impl::general_work (int noutput_items,
                                      gr_vector_int &ninput_items,
                                      gr_vector_const_void_star &input_items,
                                      gr_vector_void_star &output_items)
{
  const gr_complex *in = (const gr_complex *) input_items[0];
  int8_t *out = (int8_t *) output_items[0];
  size_t items_consumed = 0;
  size_t items_produced = (noutput_items*d_pcm_rate/d_bits_per_symbol)*(d_bits_per_symbol/d_pcm_rate);

  if(ninput_items[0] == 0){
    consume_each(0);
    return 0;
  }

  for(size_t i =0; i< noutput_items*d_pcm_rate/d_bits_per_symbol; i++){
    if(d_bits_per_symbol == 1){
      d_pcm_buffer[i] = normalize(std::real(d_constellation->get_const_pts()[0]),
                                  std::real(d_constellation->get_const_pts()[1]),
                                  std::real(in[i]));
    }
    else if(d_bits_per_symbol == 2){
      d_pcm_buffer[2*i] = normalize(std::real(d_constellation->get_const_pts()[0]),
                                  std::real(d_constellation->get_const_pts()[2]),
                                  std::real(in[i]));
      d_pcm_buffer[2*i + 1] = normalize(std::real(d_constellation->get_const_pts()[0]),
                                  std::real(d_constellation->get_const_pts()[2]),
                                  std::imag(in[i]));
    }
    d_dist_vec.clear();
  }

  d_pcm.decode_trunc(out, d_pcm_buffer, items_produced);
  consume_each((int)(noutput_items*d_pcm_rate/d_bits_per_symbol));

  memset(d_pcm_buffer, 0, max_noutput_items()*sizeof(uint8_t));
  return items_produced;
}

int8_t
ccsds_demodulator_impl::normalize(float min, float max, float point){
  return (int8_t)std::round(255.0*(point - min)/(max-min) - 128);
}


} /* namespace ccsds */
} /* namespace gr */

