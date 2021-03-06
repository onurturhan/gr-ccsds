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

#ifndef _QA_CONV_DECODER_H_
#define _QA_CONV_DECODER_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>
#include <ccsds/conv_decoder.h>

namespace gr
{
namespace ccsds
{

class qa_conv_decoder : public CppUnit::TestCase
{
public:
  CPPUNIT_TEST_SUITE(qa_conv_decoder);
  CPPUNIT_TEST(test_simple_decode_1_2);
  CPPUNIT_TEST(test_simple_decode_2_3);
  CPPUNIT_TEST(test_simple_decode_3_4);
  CPPUNIT_TEST(test_simple_decode_5_6);
  CPPUNIT_TEST(test_simple_decode_7_8);
  CPPUNIT_TEST(test_various_msg_len_1_2);
  CPPUNIT_TEST(test_various_msg_len_2_3);
  CPPUNIT_TEST(test_various_msg_len_3_4);
  CPPUNIT_TEST(test_various_msg_len_5_6);
  CPPUNIT_TEST(test_various_msg_len_7_8);
  CPPUNIT_TEST_SUITE_END();

private:
  void
  test_simple_decode_1_2 ();

  void
  test_simple_decode_2_3 ();

  void
  test_simple_decode_3_4 ();

  void
  test_simple_decode_5_6 ();

  void
  test_simple_decode_7_8 ();

  void
  test_various_msg_len_1_2 ();

  void
  test_various_msg_len_2_3 ();

  void
  test_various_msg_len_3_4 ();

  void
  test_various_msg_len_5_6 ();

  void
  test_various_msg_len_7_8 ();

  void
  test_decode(size_t msg_len, conv_decoder::coding_rate_t rate);
};

} /* namespace ccsds */
} /* namespace gr */

#endif /* _QA_CONV_DECODER_H_ */

