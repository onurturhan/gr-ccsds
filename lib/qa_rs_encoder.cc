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
#include "qa_rs_encoder.h"
#include <ccsds/rs_encoder.h>
#include <random>

#include <ccsds/libfec/fec.h>
#include <ccsds/utils.h>

namespace gr
{
namespace ccsds
{

void
qa_rs_encoder::test_simple_encoder ()
{
  std::random_device rd;
  std::mt19937 mt (rd ());
  std::uniform_int_distribution<uint8_t> uni (0, 255);
  encoder::encoder_sptr rs8 = rs_encoder::make (
      rs_encoder::ECC_16, rs_encoder::INTERLEAVER_DEPTH_1);

  uint8_t *tx = new uint8_t[255];
  uint8_t *tx_unpacked = new uint8_t[255 * 8];
  uint8_t *rx_unpacked = new uint8_t[255 * 8];

  for (size_t i = 0; i < 255 - 32; i++) {
    tx[i] = uni (mt);
  }
  packed_to_unpacked(tx_unpacked, tx, 255 - 32);

  ssize_t ret = rs8->encode (rx_unpacked, tx_unpacked, 255 - 32);

  delete[] tx;
  delete[] tx_unpacked;
  delete[] rx_unpacked;
}

} /* namespace ccsds */
} /* namespace gr */

