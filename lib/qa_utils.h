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

#ifndef LIB_QA_UTILS_H_
#define LIB_QA_UTILS_H_

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>
#include <ccsds/utils.h>

namespace gr
{
namespace ccsds
{

class qa_utils : public CppUnit::TestCase
{
public:
  CPPUNIT_TEST_SUITE(qa_utils);
  CPPUNIT_TEST(test_packing);
  CPPUNIT_TEST(test_soft_packing);
  CPPUNIT_TEST_SUITE_END();

private:
  void
  test_packing();

  void
  test_soft_packing();
};

}  // namespace ccsds


}  // namespace gr


#endif /* LIB_QA_UTILS_H_ */
