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

#include <ccsds/rs_encoder.h>
#include <ccsds/libfec/fec.h>

namespace gr
{
namespace ccsds
{

encoder::encoder_sptr
rs_encoder::make (ecc_t ecc, interleaver_t inter_depth)
{
  return encoder::encoder_sptr (new rs_encoder (ecc, inter_depth));
}

rs_encoder::rs_encoder (ecc_t ecc, interleaver_t inter_depth) :
        encoder (255 * 8 * inter_depth),
        d_buffers (inter_depth)
{
  switch (ecc)
    {
    case ECC_8:
      d_rs_ecc = 8;
      break;
    case ECC_16:
      d_rs_ecc = 16;
      break;
    default:
      throw std::invalid_argument (
          "rs_encoder: Invalid error correction capability");
    }
  d_rs_parity = 2 * d_rs_ecc;
  d_data_per_cdblk = 255 - d_rs_parity;

  switch (inter_depth)
    {
    case INTERLEAVER_DEPTH_1:
    case INTERLEAVER_DEPTH_2:
    case INTERLEAVER_DEPTH_3:
    case INTERLEAVER_DEPTH_4:
    case INTERLEAVER_DEPTH_5:
    case INTERLEAVER_DEPTH_8:
      d_inter_depth = inter_depth;
      break;
    default:
      throw std::invalid_argument ("rs_decoder: Invalid interleaving depth");
    }

  for(size_t i = 0; i < inter_depth; i++) {
    d_buffers[i] = new uint8_t[255];
  }

  d_max_frame_len = 255 * d_inter_depth; // As stated in CCSDS, max frame length will be 255* inteleaving_depth
  d_rs_code = NULL;
  init_rs_code (0);
}

rs_encoder::~rs_encoder ()
{
  for (uint8_t *i : d_buffers) {
    delete[] i;
  }
  free_rs_char (d_rs_code);
}

uint8_t
rs_encoder::init_rs_code (int fill)
{
  if (d_rs_code) {
    free_rs_char (d_rs_code);
  }
  d_rs_code = init_rs_char (8, 0x187, 112, 11, d_rs_parity, fill);
  if (d_rs_code)
    return 1;
  else
    return 0;
}

ssize_t
rs_encoder::encode_trunc (uint8_t *out, const uint8_t *in, size_t len)
{
  ssize_t cnt = 0;
  size_t total_vfill;
  size_t vfill = 0;
  size_t byte_len = len / 8;
  total_vfill = 255 * d_inter_depth - byte_len - d_inter_depth * d_rs_parity;

  if(!out || !in) {
    return -1;
  }

  if (len % 8 != 0 || byte_len > 255 * d_inter_depth
      || (total_vfill % d_inter_depth != 0)) {
    return -1;
  }

  vfill = (total_vfill / d_inter_depth);

  /* Re-init the RS encoder */
  init_rs_code (vfill);

  size_t s = 0;
  reset();
  for(size_t i = 0; i < byte_len; i++) {
    d_buffers[s][vfill + i / d_inter_depth] = in[i];
    s = (s + 1) % d_inter_depth;
  }

  /* Encode the interleaved bytes */
  for (uint8_t *i : d_buffers) {
    encode_rs_char (d_rs_code, i + vfill, i + (255 - d_rs_parity));
  }

  /* Repack messages from each encoder */
  s = 0;
  for(size_t i = 0; i < byte_len + d_inter_depth * d_rs_parity; i++) {
    out[cnt++] = d_buffers[s][vfill + i / d_inter_depth];
    s = (s + 1) % d_inter_depth;
  }

  return cnt * 8;
}

void
rs_encoder::reset ()
{
  for (uint8_t *i : d_buffers) {
    memset(i, 0, 255);
  }
}

ssize_t
rs_encoder::encode (uint8_t *out, const uint8_t *in, size_t len)
{
  return encode_trunc(out, in, len);
}
} /* namespace ccsds */
} /* namespace gr */

