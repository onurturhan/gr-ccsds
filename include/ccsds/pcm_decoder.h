//* -*- c++ -*- */
/*
 * gr-ccsds: CCSDS Telemetry and Telecommand Transceivers
 *
 *  Copyright (C) 2018, 2019
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

#ifndef INCLUDED_CCSDS_PCM_DECODER_H
#define INCLUDED_CCSDS_PCM_DECODER_H

#include <ccsds/api.h>
#include <cstring>

namespace gr {
namespace ccsds {

class CCSDS_API pcm_decoder
{
public:

  pcm_decoder(pcm_type_t type);
  ~pcm_decoder();

  /**
   * Decode input buffer according to a given Pulse Code Modulation.
   * This function continues from the previous state
   *
   * @param out    the output buffer
   * @param in     the input buffer
   * @param length the length of the input in bits
   */
  ssize_t decode_trunc(int8_t* out, int8_t* in, size_t length);

  /**
   * Decode input buffer according to a given Pulse Code Modulation
   *
   * @param out    the output buffer
   * @param in     the input buffer
   * @param length the length of the input in bits
   */
  ssize_t decode(int8_t* out, int8_t* in, size_t length);

private:
  pcm_type_t d_type;
  int8_t    d_prev_bit;

  int8_t remap (int8_t point, int8_t decision);

  ssize_t pcm(int8_t* out, int8_t* in, size_t length);

};

} // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_PCM_DECODER_H */

