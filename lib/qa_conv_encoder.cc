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
  uint8_t tx[20];
  for(int i=0; i< 20; i++){
    tx[i] = 0x00;
  }
  uint8_t rx[255];
  memset (rx, 0xff, 255);


  conv->encode_trunc(rx, tx, sizeof(tx) * 8);
  volatile size_t len = conv->finalize(&rx[40]);
}

} /* namespace ccsds */
} /* namespace gr */

