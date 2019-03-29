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


#include <ccsds/conv_decoder.h>
#include <ccsds/libfec/fec.h>

namespace gr
{
namespace ccsds
{

decoder::decoder_sptr
conv_decoder::make (conv_decoder::coding_rate_t coding_rate,
                    size_t max_frame_len)
{
  return decoder::decoder_sptr (new conv_decoder (coding_rate, max_frame_len));
}

conv_decoder::conv_decoder (coding_rate_t coding_rate, size_t max_frame_len) :
        decoder (max_frame_len),
        d_rate (coding_rate),
        d_trunc_depth(0),
        d_syms(nullptr),
        d_unpacked(nullptr),
        d_packed_b(0x0),
        d_first_block(true),
        d_vp(0),
        d_last_state(0)
{
  /*
   * The truncate depth greatly affects the performance and the computational
   * demands of the Viterbo decoder.
   *
   * Based on the findings of B.  Moision. “A  Truncation  Depth  Rule
   * of  Thumb  for  Convolutional  Codes.”    In
   * Information  Theory  and  Applications  Workshop
   * (January  27  2008-February  1  2008,  San Diego, California)
   * , 555-557.  New York: IEEE, 2008. mentioned by  CCSDS 130.1-G-2,
   * we set the truncate depth to $3 \times (K/1-R)$
   */
  switch (coding_rate)
    {
    case RATE_1_2:
      d_trunc_depth = 3 * (7 / (1 - 1.0/2.0));
      break;
    case RATE_2_3:
      d_trunc_depth = 3 * (7 / (1 - 2.0/3.0));
      break;
    case RATE_3_4:
      d_trunc_depth = 3 * (7 / (1 - 3.0/4.0));
      break;
    case RATE_5_6:
      d_trunc_depth = 3 * (7 / (1 - 5.0 / 6.0));
      break;
    case RATE_7_8:
      d_trunc_depth = 3 * (7 / (1 - 7.0 / 8.0));
      break;
    default:
      d_trunc_depth = 0;
      throw std::invalid_argument ("conv_decoder: Invalid coding rate");
    }

  /*
   * If the traceback depth is a multiple of 16, then the decoding process
   * can be simplified a lot, making it also more efficient and faster
   */
  while(d_trunc_depth % 16) {
    d_trunc_depth++;
  }
  d_syms = new uint8_t[d_trunc_depth];
  d_unpacked = new uint8_t[d_trunc_depth / 2];

  int polys[2] = { V27POLYB, -V27POLYA };
  d_vp = create_viterbi27 (d_trunc_depth / 2);
  if(!d_vp) {
    throw std::runtime_error("conv_decoder: Could not create Viterbi instance");
  }
  set_viterbi27_polynomial (polys);
  init_viterbi27 (d_vp, d_last_state);
}

conv_decoder::~conv_decoder ()
{
  delete_viterbi27(d_vp);
  delete [] d_syms;
  delete [] d_unpacked;
}

ssize_t
conv_decoder::decode_trunc (uint8_t* out, const uint8_t* in, size_t len)
{
  return -1;
}

void
conv_decoder::reset ()
{
  d_first_block = true;
}

ssize_t
conv_decoder::decode (uint8_t* out, const uint8_t* in, size_t len)
{
  size_t idx = 0;
  size_t ret;
  /* Decode the blocks that fill entirelly the whole traceback depth */
  for(size_t i = 0; i < len; i += d_trunc_depth) {
    ret = decode_block(out + idx, in + i/8, d_trunc_depth);
    idx += ret;
  }

  /* Decode any remaining bits */
  ret = decode_block(out + idx, in + (len / d_trunc_depth) * d_trunc_depth / 8,
                     len - (len / d_trunc_depth) * d_trunc_depth);
  idx += ret;
  return idx;
}



size_t
conv_decoder::decode_block (uint8_t* out, const uint8_t* in, size_t len)
{
  if(len < 2) {
    return 0;
  }
  init_viterbi27 (d_vp, d_last_state);
  for (uint32_t i = 0; i < len; i += 8) {
    d_syms[i] = ((in[i / 8] >> 7) & 0x1) * 255;
    d_syms[i + 1] = ((in[i / 8] >> 6) & 0x1) * 255;
    d_syms[i + 2] = ((in[i / 8] >> 5) & 0x1) * 255;
    d_syms[i + 3] = ((in[i / 8] >> 4) & 0x1) * 255;
    d_syms[i + 4] = ((in[i / 8] >> 3) & 0x1) * 255;
    d_syms[i + 5] = ((in[i / 8] >> 2) & 0x1) * 255;
    d_syms[i + 6] = ((in[i / 8] >> 1) & 0x1) * 255;
    d_syms[i + 7] = (in[i / 8] & 0x1) * 255;
  }

  for (uint32_t i = (len / 8) * 8; i < len; i++) {
    d_syms[i] = ((in[i / 8] >> (7 - (i % 8))) & 0x1) * 255;
  }

  update_viterbi27_blk(d_vp, d_syms, len / 2);

  int state = chainback_viterbi27_unpacked_trunc(d_vp, d_unpacked,
                                                 len / 2);
  d_last_state = (uint32_t)state;

  /* Repack bits, skipping the first 6 bits if this was the first block */
  uint8_t *bits = d_unpacked;
  size_t nbits = len / 2;

  if(d_first_block) {
    bits+=6;
    nbits -= 6;
    d_first_block = false;
    for(size_t i = 0; i < nbits; i++) {
      d_packed_b = (d_packed_b << 1) | bits[i];
      out[i >> 3] = d_packed_b;
    }
    return nbits / 8;
  }
  for (size_t i = 0; i < nbits; i++) {
    d_packed_b = (d_packed_b << 1) | bits[i];
    /*
     * 2 bits are already, decoded from the previous iteration, due to the fact
     * that we have discarded the first 6 bits
     */
    out[(i + 2) >> 3] = d_packed_b;
  }
  return nbits / 8;
}

} /* namespace ccsds */
} /* namespace gr */



