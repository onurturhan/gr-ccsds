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
#include <gnuradio/blocks/null_sink.h>
#include <gnuradio/messages/msg_passing.h>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <gnuradio/top_block.h>
#include <gnuradio/blocks/head.h>
#include <random>

namespace gr {
  namespace ccsds {

    void
    qa_ccsds_modulator::t1()
    {
      std::random_device rd;
      std::mt19937 mt(rd());
      std::uniform_int_distribution<uint8_t> uni(0, 255);
      gr::top_block_sptr tb = gr::make_top_block("top");
      ccsds::ccsds_constellation_sptr bpsk= ccsds::ccsds_constellation_oqpsk::make();
      ccsds::ccsds_modulator::sptr mod = ccsds::ccsds_modulator::make(bpsk, BPL);
      gr::block_sptr dst = gr::blocks::null_sink::make(sizeof(gr_complex));

      tb->connect(mod, 0, dst, 0);

      tb->start();
      uint8_t tx[8192];
      for(size_t i = 0; i < 8192; i++) {
        tx[i] = uni(mt);
      }
      pmt::pmt_t port(pmt::intern("PDU_in"));
      for (int i = 0; i < 12; i++){
        send(mod, port, pmt::mp(pmt::make_blob(tx, 8192*sizeof(uint8_t))));
      }

      // Give the messages a chance to be processed
      sleep(1);

      tb->stop();
      tb->wait();


    }

  } /* namespace ccsds */
} /* namespace gr */

