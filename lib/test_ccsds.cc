/* -*- c++ -*- */
/*
 * gr-ccsds: CCSDS Telemetry and Telecommand Transceivers
 *
 *  Copyright (C) 2018
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cppunit/TextTestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestResult.h>

#include <gnuradio/unittests.h>
#include "qa_ccsds.h"
#include <iostream>
#include <fstream>

int
main (int argc, char **argv)
{
  CppUnit::TestResultCollector result;
  CppUnit::BriefTestProgressListener progressListener;
  CppUnit::TextTestRunner runner;
  //std::ofstream xmlfile(get_unittest_path("ccsds.xml").c_str());
  //CppUnit::XmlOutputter *xmlout = new CppUnit::XmlOutputter(&runner.result(), xmlfile);

  runner.addTest(qa_ccsds::suite());
  //runner.setOutputter(xmlout);
  runner.eventManager().addListener(&result);
  runner.eventManager().addListener(&progressListener);

  bool was_successful = runner.run("", false);

  return was_successful ? 0 : 1;
}
