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
#include <gnuradio/blocks/vector_sink_c.h>

namespace gr {
  namespace ccsds {

    void
    qa_ccsds_modulator::t1()
    {
      std::random_device rd;
      size_t data_length= 2048*2;
      std::mt19937 mt(rd());
      std::uniform_int_distribution<uint8_t> uni(0, 255);
      gr::top_block_sptr tb = gr::make_top_block("top");
      ccsds::pcm_decoder dec = pcm_decoder(NRZ_S);
      ccsds::ccsds_constellation_sptr bpsk= ccsds::ccsds_constellation_qpsk::make();
      int bps = 2;
      ccsds::ccsds_modulator::sptr mod = ccsds::ccsds_modulator::make(bpsk, NRZ_S);
      gr::blocks::vector_sink_c::sptr dst = gr::blocks::vector_sink_c::make(1);
      //ccsds_constellation_sptr      constellation = ccsds::ccsds_constellation_bpsk::make();

      tb->connect(mod, 0, dst, 0);

      tb->start();
      uint8_t tx[data_length];
      for(size_t i = 0; i < data_length; i++) {
        tx[i] = uni(mt);
      }

      uint8_t tx_b[sizeof(tx)*8];
      for(size_t i = 0; i < sizeof(tx)*8; i++) {
        tx_b[i] = (tx[i/8] >> (7 - i%8)) & 0x01;
      }

      pmt::pmt_t port(pmt::intern("PDU_in"));
      for (int i = 0; i < 1; i++){
        send(mod, port, pmt::make_blob(tx_b, data_length*8*sizeof(uint8_t)));
      }

      // Give the messages a chance to be processed
      sleep(1);

      tb->stop();
      tb->wait();

      std::vector<gr_complex> dat = dst->data();

      int8_t rx[dat.size()*bps];
      for(int i=0; i< dat.size();i++){
        if(bps == 1){
          if(dat[i] == bpsk->get_const_pts()[0]){
            rx[i] = -128;
          }
          else {
            rx[i] = 127;
          }
        }
        else{
          if(dat[i] == bpsk->get_const_pts()[0]){
            rx[2*i] = -128;
            rx[2*i+1] = -128;
          }
          else if(dat[i] ==bpsk->get_const_pts()[1]){
            rx[2*i] = -128;
            rx[2*i+1] = 127;
          }
          else if(dat[i] ==bpsk->get_const_pts()[2]){
            rx[2*i] = 127;
            rx[2*i+1] = -128;
          }
          else if(dat[i] ==bpsk->get_const_pts()[3]){
            rx[2*i] = 127;
            rx[2*i+1] = 127;
          }
        }
      }
      int8_t pcm_buffer[dat.size()*bps];
      int8_t out[dat.size()*bps];
      dec.decode(out, rx, dat.size()*bps);

      uint8_t c[dat.size()*bps/8];
      printf("Returned size %d\n",dat.size());
      for(int i=0 ; i< dat.size()*bps; i++){
        int8_t o = out[i];
        if(o >= 0){
          c[i/8] = (c[i/8] << 1 ) | 0x01;
        }
        else{
          c[i/8] = (c[i/8] << 1 );
        }
      }
      for(uint32_t i = 0; i < sizeof(tx); i++) {
        //printf("(%d %d %d)",tx[i], c[i], i);
        CPPUNIT_ASSERT(tx[i] == c[i]);
      }
    }

  } /* namespace ccsds */
} /* namespace gr */

