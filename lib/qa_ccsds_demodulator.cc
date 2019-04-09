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
#include "qa_ccsds_demodulator.h"
#include <ccsds/ccsds_demodulator.h>

#include "qa_ccsds_modulator.h"
#include <ccsds/ccsds_modulator.h>
#include <ccsds/ccsds_constellation.h>
#include <ccsds/pcm_encoder.h>
#include <gnuradio/blocks/null_sink.h>
#include <gnuradio/blocks/vector_sink_b.h>
#include <gnuradio/messages/msg_passing.h>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <gnuradio/top_block.h>
#include <gnuradio/blocks/head.h>
#include <random>
#include <fstream>
#include <iostream>

namespace gr
{
namespace ccsds
{

void
qa_ccsds_demodulator::bpsk_uncoded ()
{
  size_t data_length = 2048*2;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  gr::top_block_sptr tb = gr::make_top_block("top");
  ccsds::ccsds_constellation_sptr constellation= ccsds::ccsds_constellation_bpsk::make();
  ccsds::ccsds_modulator::sptr mod = ccsds::ccsds_modulator::make(constellation, UNCODED);
  ccsds::ccsds_demodulator::sptr demod = ccsds::ccsds_demodulator::make(constellation, UNCODED);
  gr::blocks::vector_sink_b::sptr dst = gr::blocks::vector_sink_b::make(1);

  tb->connect(mod, 0, demod, 0);
  tb->connect(demod, 0, dst, 0);

  tb->start();

  uint8_t tx[data_length];
  for(int i=0 ; i< sizeof(tx); i++){
    tx[i] = uni(mt);
  }
  uint8_t tx_b[sizeof(tx)*8];
  for(size_t i = 0; i < sizeof(tx)*8; i++) {
    tx_b[i] = (tx[i/8] >> (7 - i%8)) & 0x01;
  }

//  memcpy(tx, str.data(), str.length());
  pmt::pmt_t port(pmt::intern("PDU_in"));
  for (int i = 0; i < 1; i++){
    send(mod, port, pmt::make_blob(tx_b, sizeof(tx)*8));
  }

  // Give the messages a chance to be processed
  sleep(1);

  tb->stop ();
  tb->wait ();

  std::vector<uint8_t> dat = dst->data();

  uint8_t c[dat.size()/8];
  for(int i=0 ; i< dat.size(); i++){
    int8_t o = dat[i];
    if(o >= 0){
      c[i/8] = (c[i/8] << 1 ) | 0x01;
    }
    else{
      c[i/8] = (c[i/8] << 1 );
    }
  }

  for(uint32_t i = 0; i < sizeof(tx); i++) {
    CPPUNIT_ASSERT(tx[i] == c[i]);
  }
}

void
qa_ccsds_demodulator::bpsk_nrzs ()
{
  size_t data_length = 2048*2;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  gr::top_block_sptr tb = gr::make_top_block("top");
  ccsds::ccsds_constellation_sptr constellation= ccsds::ccsds_constellation_bpsk::make();
  ccsds::ccsds_modulator::sptr mod = ccsds::ccsds_modulator::make(constellation, NRZ_S);
  ccsds::ccsds_demodulator::sptr demod = ccsds::ccsds_demodulator::make(constellation, NRZ_S);
  gr::blocks::vector_sink_b::sptr dst = gr::blocks::vector_sink_b::make(1);

  tb->connect(mod, 0, demod, 0);
  tb->connect(demod, 0, dst, 0);

  tb->start();

  uint8_t tx[data_length];
  for(int i=0 ; i< sizeof(tx); i++){
    tx[i] = uni(mt);
  }
  uint8_t tx_b[sizeof(tx)*8];
  for(size_t i = 0; i < sizeof(tx)*8; i++) {
    tx_b[i] = (tx[i/8] >> (7 - i%8)) & 0x01;
  }

//  memcpy(tx, str.data(), str.length());
  pmt::pmt_t port(pmt::intern("PDU_in"));
  for (int i = 0; i < 1; i++){
    send(mod, port, pmt::make_blob(tx_b, sizeof(tx)*8));
  }

  // Give the messages a chance to be processed
  sleep(1);

  tb->stop ();
  tb->wait ();

  std::vector<uint8_t> dat = dst->data();

  uint8_t c[dat.size()/8];
  for(int i=0 ; i< dat.size(); i++){
    int8_t o = dat[i];
    if(o >= 0){
      c[i/8] = (c[i/8] << 1 ) | 0x01;
    }
    else{
      c[i/8] = (c[i/8] << 1 );
    }
  }

  for(uint32_t i = 0; i < sizeof(tx); i++) {
    CPPUNIT_ASSERT(tx[i] == c[i]);
  }

}

void
qa_ccsds_demodulator::bpsk_nrzm ()
{
  size_t data_length = 2048*2;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  gr::top_block_sptr tb = gr::make_top_block("top");
  ccsds::ccsds_constellation_sptr constellation= ccsds::ccsds_constellation_bpsk::make();
  ccsds::ccsds_modulator::sptr mod = ccsds::ccsds_modulator::make(constellation, NRZ_M);
  ccsds::ccsds_demodulator::sptr demod = ccsds::ccsds_demodulator::make(constellation, NRZ_M);
  gr::blocks::vector_sink_b::sptr dst = gr::blocks::vector_sink_b::make(1);

  tb->connect(mod, 0, demod, 0);
  tb->connect(demod, 0, dst, 0);

  tb->start();

  uint8_t tx[data_length];
  for(int i=0 ; i< sizeof(tx); i++){
    tx[i] = uni(mt);
  }
  uint8_t tx_b[sizeof(tx)*8];
  for(size_t i = 0; i < sizeof(tx)*8; i++) {
    tx_b[i] = (tx[i/8] >> (7 - i%8)) & 0x01;
  }

//  memcpy(tx, str.data(), str.length());
  pmt::pmt_t port(pmt::intern("PDU_in"));
  for (int i = 0; i < 1; i++){
    send(mod, port, pmt::make_blob(tx_b, sizeof(tx)*8));
  }

  // Give the messages a chance to be processed
  sleep(1);

  tb->stop ();
  tb->wait ();

  std::vector<uint8_t> dat = dst->data();

  uint8_t c[dat.size()/8];
  for(int i=0 ; i< dat.size(); i++){
    int8_t o = dat[i];
    if(o >= 0){
      c[i/8] = (c[i/8] << 1 ) | 0x01;
    }
    else{
      c[i/8] = (c[i/8] << 1 );
    }
  }

  for(uint32_t i = 0; i < sizeof(tx); i++) {
    CPPUNIT_ASSERT(tx[i] == c[i]);
  }

}

void
qa_ccsds_demodulator::qpsk_uncoded ()
{
  size_t data_length = 2048*2;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  gr::top_block_sptr tb = gr::make_top_block("top");
  ccsds::ccsds_constellation_sptr constellation= ccsds::ccsds_constellation_qpsk::make();
  ccsds::ccsds_modulator::sptr mod = ccsds::ccsds_modulator::make(constellation, UNCODED);
  ccsds::ccsds_demodulator::sptr demod = ccsds::ccsds_demodulator::make(constellation, UNCODED);
  gr::blocks::vector_sink_b::sptr dst = gr::blocks::vector_sink_b::make(1);

  tb->connect(mod, 0, demod, 0);
  tb->connect(demod, 0, dst, 0);

  tb->start();

  uint8_t tx[data_length];
  for(int i=0 ; i< sizeof(tx); i++){
    tx[i] = uni(mt);
  }
  uint8_t tx_b[sizeof(tx)*8];
  for(size_t i = 0; i < sizeof(tx)*8; i++) {
    tx_b[i] = (tx[i/8] >> (7 - i%8)) & 0x01;
  }

//  memcpy(tx, str.data(), str.length());
  pmt::pmt_t port(pmt::intern("PDU_in"));
  for (int i = 0; i < 1; i++){
    send(mod, port, pmt::make_blob(tx_b, sizeof(tx)*8));
  }

  // Give the messages a chance to be processed
  sleep(1);

  tb->stop ();
  tb->wait ();

  std::vector<uint8_t> dat = dst->data();

  uint8_t c[dat.size()/8];
  for(int i=0 ; i< dat.size(); i++){
    int8_t o = dat[i];
    if(o >= 0){
      c[i/8] = (c[i/8] << 1 ) | 0x01;
    }
    else{
      c[i/8] = (c[i/8] << 1 );
    }
  }

  for(uint32_t i = 0; i < sizeof(tx); i++) {
    CPPUNIT_ASSERT(tx[i] == c[i]);
  }

}

void
qa_ccsds_demodulator::qpsk_nrzm ()
{
  size_t data_length = 2048*3;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  gr::top_block_sptr tb = gr::make_top_block("top");
  ccsds::ccsds_constellation_sptr constellation= ccsds::ccsds_constellation_qpsk::make();
  ccsds::ccsds_modulator::sptr mod = ccsds::ccsds_modulator::make(constellation, NRZ_M);
  ccsds::ccsds_demodulator::sptr demod = ccsds::ccsds_demodulator::make(constellation, NRZ_M);
  gr::blocks::vector_sink_b::sptr dst = gr::blocks::vector_sink_b::make(1);

  tb->connect(mod, 0, demod, 0);
  tb->connect(demod, 0, dst, 0);

  tb->start();

  uint8_t tx[data_length];
  for(int i=0 ; i< sizeof(tx); i++){
    tx[i] = uni(mt);
  }
  uint8_t tx_b[sizeof(tx)*8];
  for(size_t i = 0; i < sizeof(tx)*8; i++) {
    tx_b[i] = (tx[i/8] >> (7 - i%8)) & 0x01;
  }

//  memcpy(tx, str.data(), str.length());
  pmt::pmt_t port(pmt::intern("PDU_in"));
  for (int i = 0; i < 1; i++){
    send(mod, port, pmt::make_blob(tx_b, sizeof(tx)*8));
  }

  // Give the messages a chance to be processed
  sleep(1);

  tb->stop ();
  tb->wait ();

  std::vector<uint8_t> dat = dst->data();

  uint8_t c[dat.size()/8];
  for(int i=0 ; i< dat.size(); i++){
    int8_t o = dat[i];
    if(o >= 0){
      c[i/8] = (c[i/8] << 1 ) | 0x01;
    }
    else{
      c[i/8] = (c[i/8] << 1 );
    }
  }

  for(uint32_t i = 0; i < sizeof(tx); i++) {
    CPPUNIT_ASSERT(tx[i] == c[i]);
  }

}

void
qa_ccsds_demodulator::qpsk_nrzs ()
{
  size_t data_length = 2048*2;
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  gr::top_block_sptr tb = gr::make_top_block("top");
  ccsds::ccsds_constellation_sptr constellation= ccsds::ccsds_constellation_qpsk::make();
  ccsds::ccsds_modulator::sptr mod = ccsds::ccsds_modulator::make(constellation, NRZ_S);
  ccsds::ccsds_demodulator::sptr demod = ccsds::ccsds_demodulator::make(constellation, NRZ_S);
  gr::blocks::vector_sink_b::sptr dst = gr::blocks::vector_sink_b::make(1);

  tb->connect(mod, 0, demod, 0);
  tb->connect(demod, 0, dst, 0);

  tb->start();

  uint8_t tx[data_length];
  for(int i=0 ; i< sizeof(tx); i++){
    tx[i] = uni(mt);
  }
  uint8_t tx_b[sizeof(tx)*8];
  for(size_t i = 0; i < sizeof(tx)*8; i++) {
    tx_b[i] = (tx[i/8] >> (7 - i%8)) & 0x01;
  }

//  memcpy(tx, str.data(), str.length());
  pmt::pmt_t port(pmt::intern("PDU_in"));
  for (int i = 0; i < 1; i++){
    send(mod, port, pmt::make_blob(tx_b, sizeof(tx)*8));
  }

  // Give the messages a chance to be processed
  sleep(1);

  tb->stop ();
  tb->wait ();

  std::vector<uint8_t> dat = dst->data();

  uint8_t c[dat.size()/8];
  for(int i=0 ; i< dat.size(); i++){
    int8_t o = dat[i];
    if(o >= 0){
      c[i/8] = (c[i/8] << 1 ) | 0x01;
    }
    else{
      c[i/8] = (c[i/8] << 1 );
    }
  }

  for(uint32_t i = 0; i < sizeof(tx); i++) {
    CPPUNIT_ASSERT(tx[i] == c[i]);
  }

}

} /* namespace ccsds */
} /* namespace gr */

