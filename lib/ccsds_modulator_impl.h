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

#ifndef INCLUDED_CCSDS_CCSDS_MODULATOR_IMPL_H
#define INCLUDED_CCSDS_CCSDS_MODULATOR_IMPL_H

#include <ccsds/ccsds_modulator.h>
#include <ccsds/pcm_encoder.h>

namespace gr {
namespace ccsds {

class ccsds_modulator_impl : public ccsds_modulator
{
 private:
  ccsds::ccsds_constellation_sptr   d_constellation;
  pmt::pmt_t                        d_pdu_port;
  std::list<pmt::pmt_t>             d_buffers;
  size_t                            d_remaining;
  size_t                            d_tx_index;
  int8_t                           d_oqpsk_symbol;
  uint8_t                           d_bits_per_symbol;
  pcm_encoder                       d_pcm;
  uint8_t*                          d_pcm_buffer;
  uint8_t                           d_pcm_rate;

  void handle_pdu(pmt::pmt_t pdu_in);
  void modulate_input(gr_complex* out, size_t length);

 public:
  ccsds_modulator_impl(ccsds::ccsds_constellation_sptr constellation, uint8_t pcm);
  ~ccsds_modulator_impl();

      // Where all the action really happens
  int work(int noutput_items,
     gr_vector_const_void_star &input_items,
     gr_vector_void_star &output_items);
};

} // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CCSDS_MODULATOR_IMPL_H */

