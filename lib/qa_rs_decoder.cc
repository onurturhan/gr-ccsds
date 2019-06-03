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
#include <ccsds/utils.h>
#include <random>

#include <ccsds/libfec/fec.h>

namespace gr
{
namespace ccsds
{


void
qa_rs_decoder::test_ecc8_decode ()
{
  ssize_t ret;
  std::random_device rd;
  std::mt19937 mt (rd ());
  std::uniform_int_distribution<uint8_t> uni (0, 255);
  encoder::encoder_sptr enc_rs8 = rs_encoder::make (
      rs_encoder::ECC_8, rs_encoder::INTERLEAVER_DEPTH_1);
  decoder::decoder_sptr rs8 = rs_decoder::make (
      rs_decoder::ECC_8, rs_decoder::INTERLEAVER_DEPTH_1);

  uint8_t raw_packed[255];
  uint8_t raw_unpacked[255 * 8];
  uint8_t coded_unpacked_hard[255 * 8];
  int8_t coded_unpacked_soft[255 * 8];
  uint8_t decoded_unpacked[255 * 8];
  uint8_t decoded_packed[255];

  for (size_t i = 0; i < 255 - 16; i++) {
    raw_packed[i] = uni (mt);
  }
  packed_to_unpacked (raw_unpacked, raw_packed, 255 - 16);
  ret = enc_rs8->encode (coded_unpacked_hard, raw_unpacked, (255 - 16) * 8);
  CPPUNIT_ASSERT(ret == 255 * 8);

  hard_to_soft_int8 (coded_unpacked_soft, coded_unpacked_hard, 255 * 8);

  /*
   * This will actually test also proper compatibility with the CCSDS version
   * of libfec
   */
  ret = rs8->decode (decoded_unpacked, coded_unpacked_soft, 255 * 8);
  CPPUNIT_ASSERT(ret >= 0);
  unpacked_to_packed (decoded_packed, decoded_unpacked, 255 * 8);
  for (size_t i = 0; i < 255 - 16; i++) {
    CPPUNIT_ASSERT(raw_packed[i] == decoded_packed[i]);
  }
}

void
qa_rs_decoder::test_ecc16_decode ()
{
  ssize_t ret;
  std::random_device rd;
  std::mt19937 mt (rd ());
  std::uniform_int_distribution<uint8_t> uni (0, 255);
  encoder::encoder_sptr enc_rs8 = rs_encoder::make (
      rs_encoder::ECC_16, rs_encoder::INTERLEAVER_DEPTH_1);
  decoder::decoder_sptr rs8 = rs_decoder::make (
      rs_decoder::ECC_16, rs_decoder::INTERLEAVER_DEPTH_1);

  uint8_t raw_packed[255];
  uint8_t raw_unpacked[255 * 8];
  uint8_t coded_unpacked_hard[255 * 8];
  int8_t coded_unpacked_soft[255 * 8];
  uint8_t decoded_unpacked[255 * 8];
  uint8_t decoded_packed[255];

  for (size_t i = 0; i < 255 - 32; i++) {
    raw_packed[i] = uni (mt);
  }
  packed_to_unpacked (raw_unpacked, raw_packed, 255 - 32);
  ret = enc_rs8->encode (coded_unpacked_hard, raw_unpacked, (255 - 32) * 8);
  CPPUNIT_ASSERT(ret == 255 * 8);

  hard_to_soft_int8 (coded_unpacked_soft, coded_unpacked_hard, 255 * 8);

  /*
   * This will actually test also proper compatibility with the CCSDS version
   * of libfec
   */
  ret = rs8->decode (decoded_unpacked, coded_unpacked_soft, 255 * 8);
  CPPUNIT_ASSERT(ret >= 0);
  unpacked_to_packed (decoded_packed, decoded_unpacked, 255 * 8);
  for (size_t i = 0; i < 255 - 32; i++) {
    CPPUNIT_ASSERT(raw_packed[i] == decoded_packed[i]);
  }
}


static void
test(size_t ecc, size_t depth, size_t vfill = 0)
{
  ssize_t ret;
  std::random_device rd;
  std::mt19937 mt (rd ());
  std::uniform_int_distribution<uint8_t> uni (0, 255);
  encoder::encoder_sptr enc_rs = rs_encoder::make ((rs_encoder::ecc_t) ecc,
                                                   (rs_encoder::interleaver_t)depth);
  decoder::decoder_sptr rs = rs_decoder::make ((rs_decoder::ecc_t) ecc,
                                               (rs_decoder::interleaver_t) depth);

  uint8_t raw_packed[255 * depth];
  uint8_t raw_unpacked[255 * 8 * depth];
  uint8_t coded_unpacked_hard[255 * 8 * depth];
  int8_t coded_unpacked_soft[255 * 8 * depth];
  uint8_t decoded_unpacked[255 * 8 * depth];
  uint8_t decoded_packed[255 * depth];

  for (size_t i = 0; i < 255 * depth; i++) {
    raw_packed[i] = uni (mt);
  }
  packed_to_unpacked (raw_unpacked, raw_packed, depth * (255 - vfill - 2 * ecc));
  ret = enc_rs->encode (coded_unpacked_hard, raw_unpacked,
                        depth * (255 - vfill - 2 * ecc) * 8);
  CPPUNIT_ASSERT(ret == depth * (255 - vfill) * 8);

  hard_to_soft_int8 (coded_unpacked_soft, coded_unpacked_hard,
                     depth * (255 - vfill) * 8);

  /*
   * This will actually test also proper compatibility with the CCSDS version
   * of libfec
   */
  ret = rs->decode (decoded_unpacked, coded_unpacked_soft,
                    depth * (255 - vfill) * 8);
  CPPUNIT_ASSERT(ret >= 0);
  unpacked_to_packed (decoded_packed, decoded_unpacked,
                      depth * (255 - vfill - 2 * ecc) * 8);
  for (size_t i = 0; i < depth * (255 - vfill - 2 * ecc); i++) {
    CPPUNIT_ASSERT(raw_packed[i] == decoded_packed[i]);
  }
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

void
qa_rs_decoder::test_errors ()
{
  CPPUNIT_ASSERT_THROW(
      encoder::encoder_sptr enc_rs8 = rs_encoder::make (
          rs_encoder::ECC_16, (rs_encoder::interleaver_t) 12),
      std::invalid_argument);

  CPPUNIT_ASSERT_THROW(
      encoder::encoder_sptr enc_rs8 = rs_encoder::make (
      (rs_encoder::ecc_t) 99, rs_encoder::INTERLEAVER_DEPTH_1),
      std::invalid_argument);

  CPPUNIT_ASSERT_THROW(
      encoder::encoder_sptr enc_rs8 = rs_encoder::make (
          rs_encoder::ECC_16, (rs_encoder::interleaver_t) 12),
      std::invalid_argument);

  CPPUNIT_ASSERT_THROW (
      decoder::decoder_sptr rs8 = rs_decoder::make (
          rs_decoder::ECC_16, (rs_decoder::interleaver_t) 15),
          std::invalid_argument);
  CPPUNIT_ASSERT_THROW (
    decoder::decoder_sptr rs8 = rs_decoder::make (
      (rs_decoder::ecc_t) 99, rs_decoder::INTERLEAVER_DEPTH_1),
      std::invalid_argument);

  encoder::encoder_sptr enc_rs8 = rs_encoder::make (
      rs_encoder::ECC_16, rs_encoder::INTERLEAVER_DEPTH_1);

  decoder::decoder_sptr rs8 = rs_decoder::make (
      rs_decoder::ECC_16, rs_decoder::INTERLEAVER_DEPTH_1);
  uint8_t tmp[1];
  CPPUNIT_ASSERT(enc_rs8->encode(nullptr, nullptr, 0) == -1);
  CPPUNIT_ASSERT(enc_rs8->encode(tmp, tmp, 40*1024) == -1);
  CPPUNIT_ASSERT(rs8->decode(nullptr, nullptr, 0) == -1);
CPPUNIT_ASSERT(rs8->decode(tmp, (const int8_t *)tmp, 40*1024) == -1);
}

} /* namespace ccsds */
} /* namespace gr */

