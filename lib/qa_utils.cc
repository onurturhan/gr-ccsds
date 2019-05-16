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
#include "qa_utils.h"
#include <ccsds/utils.h>
#include <random>
#include <cstdlib>

namespace gr
{

namespace ccsds
{

void
qa_utils::test_packing ()
{
  size_t ret;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  uint8_t orig[255];
  uint8_t unpacked[255 * 8];
  uint8_t packed[255];

  for(size_t i = 0; i < 255; i++) {
    orig[i] = uni(mt);
  }
  ret = packed_to_unpacked(unpacked, orig, 255);
  CPPUNIT_ASSERT(ret == 255*8);
  ret = unpacked_to_packed(packed, unpacked, ret);
  CPPUNIT_ASSERT(ret == 255);
  for(size_t i = 0; i < 255; i++) {
    CPPUNIT_ASSERT(packed[i] == orig[i]);
  }
}

void
qa_utils::test_soft_packing ()
{
  size_t ret;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  uint8_t orig[255];
  int8_t unpacked[255 * 8];
  uint8_t packed[255];

  for(size_t i = 0; i < 255; i++) {
    orig[i] = uni(mt);
  }
  ret = packed_to_unpacked_soft(unpacked, orig, 255);
  CPPUNIT_ASSERT(ret == 255*8);
  ret = unpacked_to_packed_soft(packed, unpacked, ret);
  CPPUNIT_ASSERT(ret == 255);
  for(int i = 0; i < 255; i++) {
    CPPUNIT_ASSERT(packed[i] == orig[i]);
  }
}

}  // namespace ccsds

}  // namespace gr

