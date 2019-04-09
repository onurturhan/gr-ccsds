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

#ifndef INCLUDED_CCSDS_CCSDS_DEMODULATOR_IMPL_H
#define INCLUDED_CCSDS_CCSDS_DEMODULATOR_IMPL_H

#include <ccsds/ccsds_demodulator.h>
#include <ccsds/ccsds_constellation.h>
#include <ccsds/pcm_decoder.h>
#include <vector>

namespace gr
{
namespace ccsds
{

class ccsds_demodulator_impl : public ccsds_demodulator
{
private:
  pcm_decoder                   d_pcm;
  uint8_t                       d_pcm_rate;
  uint8_t                       d_bits_per_symbol;
  ccsds_constellation_sptr      d_constellation;
  int8_t*                       d_pcm_buffer;
  std::vector<float>            d_dist_vec;
  bool                          printit;

  float
  calc_dist(gr_complex p1, gr_complex p2);

  int8_t
  normalize(float min, float max, float point);

  void
  forecast(int noutput_items,gr_vector_int &ninput_items_required);


public:
  ccsds_demodulator_impl (ccsds::ccsds_constellation_sptr constellation,
                          uint8_t pcm);
  ~ccsds_demodulator_impl ();

  // Where all the action really happens
  int general_work(int noutput_items,
                   gr_vector_int &ninput_items,
                   gr_vector_const_void_star &input_items,
                   gr_vector_void_star &output_items);
};

} // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CCSDS_DEMODULATOR_IMPL_H */

