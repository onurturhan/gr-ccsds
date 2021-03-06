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

/*
 * This class gathers together all the test cases for the gr-filter
 * directory into a single test suite.  As you create new test cases,
 * add them here.
 */

#include "qa_ccsds.h"
#include "qa_encoder.h"
#include "qa_decoder.h"
#include "qa_conv_decoder.h"
#include "qa_utils.h"
#include "qa_rs_decoder.h"
#include "qa_rs_encoder.h"
#include "qa_conv_encoder.h"
#include "qa_turbo_encoder.h"
#include "qa_pcm_encoder.h"
#include "qa_pcm_decoder.h"
#include "qa_ccsds_modulator.h"
#include "qa_ccsds_demodulator.h"

CppUnit::TestSuite *
qa_ccsds::suite()
{
  CppUnit::TestSuite *s = new CppUnit::TestSuite("ccsds");
  s->addTest(gr::ccsds::qa_encoder::suite());
  s->addTest(gr::ccsds::qa_decoder::suite());
  s->addTest(gr::ccsds::qa_conv_decoder::suite());
  s->addTest(gr::ccsds::qa_utils::suite());
  s->addTest(gr::ccsds::qa_rs_decoder::suite());
  s->addTest(gr::ccsds::qa_rs_encoder::suite());
  s->addTest(gr::ccsds::qa_conv_encoder::suite());
  s->addTest(gr::ccsds::qa_turbo_encoder::suite());
  s->addTest(gr::ccsds::qa_pcm_encoder::suite());
  s->addTest(gr::ccsds::qa_pcm_decoder::suite());
  s->addTest(gr::ccsds::qa_ccsds_demodulator::suite());

  return s;
}
