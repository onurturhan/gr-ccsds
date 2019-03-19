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

#include <gnuradio/attributes.h>
#include <cppunit/TestAssert.h>
#include "qa_pcm_decoder.h"
#include <ccsds/pcm_decoder.h>
#include <ccsds/pcm_encoder.h>

namespace gr {
  namespace ccsds {

    void
    qa_pcm_decoder::t1()
    {
      pcm_decoder *decoder = new pcm_decoder(pcm_decoder::BPL);
      pcm_encoder *encoder = new pcm_encoder(pcm_encoder::BPL);
      size_t len = 8920;
      uint8_t alphabet[] = {"ABCDEFGHIJKLMNOPQRSTYVWXYZabcdefghijklmnopqrstyvwxyz"};
      uint8_t *mid = new uint8_t[10000];
      uint8_t *rx = new uint8_t[10000];
      size_t count = 0;

      encoder->encode(mid, alphabet, sizeof(alphabet)*8);
      decoder->decode(rx,mid,sizeof(alphabet)*8*2);
      delete [] rx;
      delete [] mid;
      delete encoder;
      delete decoder;
    }

  } /* namespace ccsds */
} /* namespace gr */
