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

#include <gnuradio/attributes.h>
#include <cppunit/TestAssert.h>
#include "qa_conv_encoder.h"
#include <ccsds/conv_encoder.h>

namespace gr
{
namespace ccsds
{

void
qa_conv_encoder::t1 ()
{
  encoder::encoder_sptr conv = conv_encoder::make (conv_encoder::RATE_1_2,
                                                   4096);
  uint8_t tx[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  uint8_t *rx = new uint8_t[255];
  memset (rx, 0, 255);

  for(int i =0; i<2; i++){
    ssize_t ret = conv->encode_trunc (rx, tx, sizeof(tx) * 8);
  }
  conv->finalize(rx);

  conv->encode(rx, tx, sizeof(tx) * 8);
  delete[] rx;
}

} /* namespace ccsds */
} /* namespace gr */

