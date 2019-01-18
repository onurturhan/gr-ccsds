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
#include <ccsds/conv_decoder.h>

namespace gr
{
namespace ccsds
{

decoder::decoder_sptr
conv_decoder::make (coding_rate_t coding_rate, size_t max_frame_len)
{
  return decoder::decoder_sptr (
      new conv_decoder (coding_rate, max_frame_len));
}

conv_decoder::conv_decoder (coding_rate_t coding_rate, size_t max_frame_len) :
    decoder(max_frame_len),
    d_rate(coding_rate)
{
  switch(coding_rate) {
    case RATE_1_2:
    case RATE_2_3:
    case RATE_3_4:
    case RATE_5_6:
    case RATE_7_8:
      break;
    default:
      throw std::invalid_argument("conv_decoder: Invalid coding rate");
  }
}

conv_decoder::~conv_decoder ()
{
}

ssize_t
conv_decoder::decode (uint8_t* out, const uint8_t* in, size_t len)
{
  return -1;
}

void
conv_decoder::reset ()
{
}

ssize_t
conv_decoder::decode_once (uint8_t* out, const uint8_t* in, size_t len)
{
  return -1;
}

} /* namespace ccsds */
} /* namespace gr */

