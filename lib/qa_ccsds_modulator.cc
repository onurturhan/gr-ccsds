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
#include "qa_ccsds_modulator.h"
#include <ccsds/ccsds_modulator.h>
#include <ccsds/ccsds_constellation.h>
#include <ccsds/pcm_encoder.h>
#include <ccsds/pcm_decoder.h>
#include <gnuradio/blocks/null_sink.h>
#include <gnuradio/messages/msg_passing.h>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <gnuradio/top_block.h>
#include <gnuradio/blocks/head.h>
#include <random>
#include <gnuradio/blocks/vector_sink.h>

namespace gr {
  namespace ccsds {

    void
    qa_ccsds_modulator::t1()
    {
    }

  } /* namespace ccsds */
} /* namespace gr */

