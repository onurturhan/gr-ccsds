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
#include <ccsds/rs_encoder.h>
#include <random>

#include <ccsds/libfec/fec.h>

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
    uint8_t idx = uni(mt) % 255;
    uint8_t err = 1 << (uni(mt) % 7);
    tx[idx] |= err;
  }

  /*
   * This will actually test also proper compatibility witth the CCSDS version
   * of libfec
   */
  ssize_t ret = rs8->decode (rx, tx, 255 * 8);

  CPPUNIT_ASSERT(ret >= 0);
  delete [] tx;
  delete [] rx;
}

void
qa_rs_decoder::test_ecc8_decode ()
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  encoder::encoder_sptr enc_rs8 = rs_encoder::make(rs_encoder::ECC_8,
                                                   rs_encoder::INTERLEAVER_DEPTH_1);
  decoder::decoder_sptr rs8 = rs_decoder::make(rs_decoder::ECC_8,
                                               rs_decoder::INTERLEAVER_DEPTH_1);

  ssize_t ret;
  uint8_t *tx = new uint8_t[255];
  uint8_t *rx = new uint8_t[255];

  for(size_t i = 0; i < 255; i++) {
    tx[i] = uni(mt);
  }
  ret = enc_rs8->encode(tx, tx, (255 - 16) * 8);
  CPPUNIT_ASSERT(ret == 255 * 8);

  /* Randomly inject some errors */
  for(size_t i = 0; i < 8; i++) {
    uint8_t idx = uni(mt) % 255;
    uint8_t err = 1 << (uni(mt) % 7);
    tx[idx] |= err;
  }

  ret = rs8->decode (rx, tx, 255 * 8);

  CPPUNIT_ASSERT(ret >= 0);
  delete [] tx;
  delete [] rx;
}

void
qa_rs_decoder::test_ecc16_decode ()
{
  ssize_t ret;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  encoder::encoder_sptr enc_rs = rs_encoder::make(rs_encoder::ECC_16,
                                                  rs_encoder::INTERLEAVER_DEPTH_1);
  decoder::decoder_sptr rs = rs_decoder::make(rs_decoder::ECC_16,
                                              rs_decoder::INTERLEAVER_DEPTH_1);

  uint8_t *tx = new uint8_t[255];
  uint8_t *rx = new uint8_t[255];

  for(size_t i = 0; i < 255 - 32; i++) {
    tx[i] = uni(mt);
  }

  ret = enc_rs->encode(tx, tx, (255 - 32) * 8);
  CPPUNIT_ASSERT(ret == 255 * 8);

  /* Randomly inject some errors */
  for(size_t i = 0; i < 8; i++) {
    uint8_t idx = uni(mt) % 255;
    uint8_t err = 1 << (uni(mt) % 7);
    tx[idx] |= err;
  }

  ret = rs->decode (rx, tx, 255 * 8);

  CPPUNIT_ASSERT(ret >= 0);
  delete [] tx;
  delete [] rx;
}

void
qa_rs_decoder::test_ecc8_vfill_decode ()
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  encoder::encoder_sptr enc_rs8 = rs_encoder::make(rs_encoder::ECC_8,
                                                   rs_encoder::INTERLEAVER_DEPTH_1);
  decoder::decoder_sptr rs8 = rs_decoder::make(rs_decoder::ECC_8,
                                               rs_decoder::INTERLEAVER_DEPTH_1);

  ssize_t ret;
  uint8_t *tx = new uint8_t[255];
  uint8_t *rx = new uint8_t[255];

  for(size_t i = 0; i < 255; i++) {
    tx[i] = uni(mt);
  }
  /* 30 bytes vfill */
  ret = enc_rs8->encode(tx, tx, (255 - 16 - 30) * 8);
  CPPUNIT_ASSERT(ret == (255 - 30) * 8);

  /* Randomly inject some errors */
  for(size_t i = 0; i < 8; i++) {
    uint8_t idx = uni(mt) % (255 - 30);
    uint8_t err = 1 << (uni(mt) % 7);
    tx[idx] |= err;
  }

  ret = rs8->decode (rx, tx, (255 - 30) * 8);

  CPPUNIT_ASSERT(ret >= 0);
  delete [] tx;
  delete [] rx;
}

void
qa_rs_decoder::test_ecc16_vfill_decode ()
{
  ssize_t ret;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  encoder::encoder_sptr enc_rs = rs_encoder::make(rs_encoder::ECC_16,
                                                  rs_encoder::INTERLEAVER_DEPTH_1);
  decoder::decoder_sptr rs = rs_decoder::make(rs_decoder::ECC_16,
                                              rs_decoder::INTERLEAVER_DEPTH_1);

  uint8_t *tx = new uint8_t[255];
  uint8_t *rx = new uint8_t[255];

  for(size_t i = 0; i < 255 - 32; i++) {
    tx[i] = uni(mt);
  }
  /* Virtual fill 30 bytes */
  ret = enc_rs->encode(tx, tx, (255 - 30 - 32) * 8);
  CPPUNIT_ASSERT(ret == (255 - 30) * 8);

  /* Randomly inject some errors */
  for(size_t i = 0; i < 8; i++) {
    uint8_t idx = uni(mt) % (255 - 30);
    uint8_t err = 1 << (uni(mt) % 7);
    tx[idx] |= err;
  }

  ret = rs->decode (rx, tx, (255 - 30) * 8);
  CPPUNIT_ASSERT(ret >= 0);

  delete [] tx;
  delete [] rx;
}


static void
test(size_t ecc, size_t depth, size_t vfill = 0)
{
  ssize_t ret;
  std::random_device rd;
  std::mt19937 mt (rd ());
  std::uniform_int_distribution<uint8_t> uni (0, 255);
  std::uniform_int_distribution<size_t> uni_idx (0, depth * (255 - 2 * ecc) - 1);
  encoder::encoder_sptr enc_rs = rs_encoder::make ((rs_encoder::ecc_t) ecc,
                                                   (rs_encoder::interleaver_t)depth);
  decoder::decoder_sptr rs = rs_decoder::make ((rs_decoder::ecc_t) ecc,
                                               (rs_decoder::interleaver_t) depth);
  uint8_t *tx = new uint8_t[depth * 255];
  uint8_t *rx = new uint8_t[depth * 255];

  /* Fill random data */
  for (size_t i = 0; i < depth * 255; i++) {
    tx[i] = uni (mt);
  }

  ret = enc_rs->encode (tx, tx, depth * (255 - vfill - 2 * ecc) * 8);
  CPPUNIT_ASSERT(ret == depth * (255 - vfill) * 8);

  /* Randomly inject 0.1% of errors */
  for (size_t i = 0; i < (size_t)(depth * (255 - vfill - 2 * ecc) * 8 * 0.001); i++) {
    uint8_t idx = uni_idx (mt);
    uint8_t err = 1 << (uni (mt) % 7);
    tx[idx] |= err;
  }

  ret = rs->decode (rx, tx, depth * (255 - vfill)* 8);
  CPPUNIT_ASSERT(ret >= 0);
  delete[] tx;
  delete[] rx;
}


void
test_multiple (size_t vfill = 0)
{
  test(8, 1, vfill);
  test(8, 2, vfill);
  test(8, 3, vfill);
  test(8, 4, vfill);
  test(8, 5, vfill);
  test(8, 8, vfill);

  test(16, 1, vfill);
  test(16, 2, vfill);
  test(16, 3, vfill);
  test(16, 4, vfill);
  test(16, 5, vfill);
  test(16, 8, vfill);
}

void
qa_rs_decoder::test_no_vfill ()
{
  test_multiple();
}

void
qa_rs_decoder::test_vfill ()
{
  test_multiple(10);
  test_multiple(15);
  test_multiple(40);
  test_multiple(50);
}


} /* namespace ccsds */
} /* namespace gr */

