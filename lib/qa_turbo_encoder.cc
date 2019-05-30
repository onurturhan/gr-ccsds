/* -*- c++ -*- */
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

#include <gnuradio/attributes.h>
#include <cppunit/TestAssert.h>
#include "qa_turbo_encoder.h"
#include <ccsds/turbo_encoder.h>

namespace gr
{
namespace ccsds
{

void
qa_turbo_encoder::t1 ()
{
  encoder::encoder_sptr turbo1 = turbo_encoder::make (turbo_encoder::RATE_1_6,
                                                      turbo_encoder::LEN_8920,
                                                      10000);
  size_t len = 1784;
  uint8_t alphabet[] =
    { "ABCDEFGHIJKLMNOPQRSTYVWXYZabcdefghijklmnopqrstyvwxyz" };
  uint8_t *tx = (uint8_t*) malloc (len * sizeof(uint8_t));
  uint8_t *rx = new uint8_t[10000];
  size_t count = 0;
  for (int i = 0; i < len; i++) {
    count = i % 52;
    tx[i] = alphabet[count] & 0x01;
  }
  turbo1->encode_trunc (rx, tx, len);

  free (tx);
  delete[] rx;

}

} /* namespace ccsds */
} /* namespace gr */

