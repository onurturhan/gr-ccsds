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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <ccsds/rs_decoder.h>
#include <cstring>
#include <ccsds/utils.h>

#include <ccsds/libfec/fec.h>

namespace gr
{
namespace ccsds
{

decoder::decoder_sptr
rs_decoder::make (rs_decoder::ecc_t ecc, rs_decoder::interleaver_t inter)
{
  return decoder::decoder_sptr(new rs_decoder(ecc, inter));
}

rs_decoder::rs_decoder (ecc_t ecc, interleaver_t inter) :
    decoder(255 * inter * 8),
    d_parity_bytes(0),
    d_inter_depth(0),
    d_packed_buffer(nullptr),
    d_buffers(inter)
{
  switch(ecc) {
    case ECC_8:
      d_parity_bytes = 16;
    break;
    case ECC_16:
      d_parity_bytes = 32;
    break;
    default:
      throw std::invalid_argument("rs_decoder: Invalid parity mode");
  }
  switch(inter) {
    case INTERLEAVER_DEPTH_1:
    case INTERLEAVER_DEPTH_2:
    case INTERLEAVER_DEPTH_3:
    case INTERLEAVER_DEPTH_4:
    case INTERLEAVER_DEPTH_5:
    case INTERLEAVER_DEPTH_8:
      d_inter_depth = inter;
      break;
    default:
      throw std::invalid_argument("rs_decoder: Invalid interleaving depth");
  }

  for(size_t i = 0; i < d_inter_depth; i++) {
    d_buffers[i] = new uint8_t[255];
  }
  d_packed_buffer = new uint8_t[d_inter_depth * 255];
}

rs_decoder::~rs_decoder ()
{
  for(uint8_t *i : d_buffers) {
    delete [] i;
  }
  delete [] d_packed_buffer;
}

ssize_t
rs_decoder::decode (uint8_t* out, const int8_t* in, size_t len)
{
  /* RS decoder is stateless and requires the whole code word */
  return decode_trunc(out, in, len);
}

/**
 *
 * @param out
 * @param in the encoded data in packed bytes form
 * @param len the number of the encoded bits, with the parity bits also.
 * The decoder will automatically apply the virtual fill bits if any
 * @return
 */
ssize_t
rs_decoder::decode_trunc (uint8_t* out, const int8_t* in, size_t len)
{
  int ret;
  ssize_t cnt = 0;
  size_t vfill = 255 * 8 * d_inter_depth - len;
  if(!out || !in) {
    return -1;
  }
  /* Check proper length settings */
  if(len > 255 * 8 * d_inter_depth
      || (vfill %  (d_inter_depth * 8) != 0)) {
    return -1;
  }
  reset();

  /* Convert the unpacked soft symbols to packed hard */
  unpacked_to_packed_soft(d_packed_buffer, in, len);

  /* Perform deinterleave and virtual fill */
  size_t ivfill = (vfill / d_inter_depth) / 8;
  size_t s = 0;

  /* Now that we known the virtual fill size, create the decoder instance */
  void *rs = init_rs_char(8, 0x187, 112, 11, d_parity_bytes, ivfill);
  if(!rs) {
    return -1;
  }

  for(size_t i = 0; i < len / 8; i++) {
    d_buffers[s][ivfill + i / d_inter_depth] = d_packed_buffer[i];
    s = (s + 1) % d_inter_depth;
  }
  /* Decode each codeword */
  for (uint8_t *i : d_buffers) {
    ret = decode_rs_char(rs, i + ivfill, NULL, 0);
    if(ret < 0) {
      free_rs_char(rs);
      return -1;
    }
  }
  /* Compact the messages from each decoder */
  s = 0;
  for(size_t i = 0; i < len / 8 - d_parity_bytes * d_inter_depth; i++) {
    d_packed_buffer[cnt++] = d_buffers[s][ivfill + i / d_inter_depth];
    s = (s + 1) % d_inter_depth;
  }
  packed_to_unpacked(out, d_packed_buffer, cnt);
  free_rs_char(rs);
  return cnt * 8;
}

void
rs_decoder::reset ()
{
  reset_buffers();
}

void
rs_decoder::reset_buffers ()
{
  for (uint8_t *i : d_buffers) {
    memset(i, 0, 255);
  }
}

} /* namespace ccsds */
} /* namespace gr */

