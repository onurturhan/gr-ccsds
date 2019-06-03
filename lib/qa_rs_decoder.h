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

#ifndef _QA_RS_DECODER_H_
#define _QA_RS_DECODER_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>

namespace gr
{
namespace ccsds
{

class qa_rs_decoder : public CppUnit::TestCase
{
public:
  CPPUNIT_TEST_SUITE(qa_rs_decoder);
  CPPUNIT_TEST(test_ecc8_decode);
  CPPUNIT_TEST(test_ecc16_decode);
  CPPUNIT_TEST(test_no_vfill);
  CPPUNIT_TEST(test_vfill);
  CPPUNIT_TEST(test_errors);
  CPPUNIT_TEST_SUITE_END();

private:
  void
  test_ecc8_decode ();

  void
  test_ecc16_decode ();

  void
  test_no_vfill();

  void
  test_vfill();

  void
  test_errors();
};

} /* namespace ccsds */
} /* namespace gr */

#endif /* _QA_RS_DECODER_H_ */

