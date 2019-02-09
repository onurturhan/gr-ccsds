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
#include "qa_rs_decoder.h"
#include <ccsds/rs_decoder.h>
#include <random>

extern "C" {
  #include <fec.h>
}

namespace gr
{
namespace ccsds
{

void
qa_rs_decoder::test_simple_decode ()
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  decoder::decoder_sptr rs8 = rs_decoder::make(rs_decoder::ECC_16,
                                               rs_decoder::INTERLEAVER_DEPTH_1);

  uint8_t *tx = new uint8_t[255];
  uint8_t *rx = new uint8_t[255];

  for(size_t i = 0; i < 255 - 32; i++) {
    tx[i] = uni(mt);
  }
  encode_rs_8(tx, tx + 255 - 32, 0);

  /* Randomly inject some errors */
  for(size_t i = 0; i < 8; i++) {
    uint8_t idx = uni(mt);
    uint8_t err = 1 << (uni(mt) % 7);
    tx[idx] |= err;
  }

  ssize_t ret = rs8->decode (rx, tx, 255 * 8);

  CPPUNIT_ASSERT(ret >= 0);
  delete [] tx;
  delete [] rx;
}

void
qa_rs_decoder::test_ecc16_decode ()
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  decoder::decoder_sptr rs8 = rs_decoder::make(rs_decoder::ECC_16,
                                               rs_decoder::INTERLEAVER_DEPTH_1);

  uint8_t *tx = new uint8_t[255];
  uint8_t *rx = new uint8_t[255];

  for(size_t i = 0; i < 255 - 32; i++) {
    tx[i] = uni(mt);
  }
  encode_rs_8(tx, tx + 255 - 32, 0);

  /* Randomly inject some errors */
  for(size_t i = 0; i < 8; i++) {
    uint8_t idx = uni(mt);
    uint8_t err = 1 << (uni(mt) % 7);
    tx[idx] |= err;
  }

  ssize_t ret = rs8->decode (rx, tx, 255 * 8);

  CPPUNIT_ASSERT(ret >= 0);
  delete [] tx;
  delete [] rx;
}

void
qa_rs_decoder::test_ecc32_decode ()
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  decoder::decoder_sptr rs8 = rs_decoder::make(rs_decoder::ECC_16,
                                               rs_decoder::INTERLEAVER_DEPTH_1);

  uint8_t *tx = new uint8_t[255];
  uint8_t *rx = new uint8_t[255];

  for(size_t i = 0; i < 255 - 32; i++) {
    tx[i] = uni(mt);
  }
  encode_rs_8(tx, tx + 255 - 32, 0);

  /* Randomly inject some errors */
  for(size_t i = 0; i < 8; i++) {
    uint8_t idx = uni(mt);
    uint8_t err = 1 << (uni(mt) % 7);
    tx[idx] |= err;
  }

  ssize_t ret = rs8->decode (rx, tx, 255 * 8);

  CPPUNIT_ASSERT(ret >= 0);
  delete [] tx;
  delete [] rx;
}

} /* namespace ccsds */
} /* namespace gr */

