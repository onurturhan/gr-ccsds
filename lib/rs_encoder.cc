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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <ccsds/rs_encoder.h>

extern "C" {
  #include <fec.h>
}

namespace gr {
namespace ccsds {

encoder::encoder_sptr
rs_encoder::make(ecc_t ecc, size_t max_frame_len)
{
  return encoder::encoder_sptr (
     new rs_encoder (ecc, max_frame_len));
}

rs_encoder::rs_encoder(ecc_t ecc, size_t max_frame_len)
{
  switch(ecc){
    case ECC_8:
      d_rs_ecc= 8;
      break;
    case ECC_16:
      d_rs_ecc = 16;
      break;
    default:
      throw std::invalid_argument("rs_encoder: Invalid error correction capability");
  }
  d_rs_parity_size = 2 * d_rs_ecc;
  d_rs_codeblock_size = 255 - d_rs_parity_size;
  d_rs_parity_buffer = (uint8_t*)malloc(d_rs_parity_size*sizeof(uint8_t));
  init_rs_code (0);
}

rs_encoder::~rs_encoder()
{
}

uint8_t
rs_encoder::init_rs_code (int fill)
{
  d_rs_code = init_rs_char (8, 0x187, 112, 11, d_rs_parity_size, fill);
  if (d_rs_code)
    return 1;
  else
    return 0;
}



ssize_t
rs_encoder::encode (uint8_t *out, const uint8_t *in, size_t len)
{
  size_t rs_bytes = 0;
  int i = 0;
  /* If data length is divided exactly by the data length of the
   *  codeblock then no fill is needed
   */
  if (len % d_rs_codeblock_size == 0) {
    for (i = 0; i < len / d_rs_codeblock_size; i++) {
      encode_rs_char (d_rs_code, (unsigned char *) in, d_rs_parity_buffer);
      /* RS is a systematic code in CCSDS so copy data first, and parity afterwards */
      memcpy (&out[i * (d_rs_codeblock_size + d_rs_parity_size)],
              &in[i * d_rs_codeblock_size], d_rs_codeblock_size);
      memcpy (
          &out[i * (d_rs_codeblock_size + d_rs_parity_size)
              + d_rs_codeblock_size],
          d_rs_parity_buffer, d_rs_parity_size);
    }
    rs_bytes = i * (d_rs_codeblock_size + d_rs_parity_size);
  }
  else {
    /* If data length is not divided exactly by the data length of the
     *  codeblock then fill with zeros is needed */
    int i = 0;
    for (i = 0; i < len / d_rs_codeblock_size; i++) {
      encode_rs_char (d_rs_code, (unsigned char *) in, d_rs_parity_buffer);
      /* RS is a systematic code in CCSDS so copy data first, and parity afterwards */
      memcpy (&out[i * (d_rs_codeblock_size + d_rs_parity_size)],
              &in[i * d_rs_codeblock_size], d_rs_codeblock_size);
      memcpy (
          &out[i * (d_rs_codeblock_size + d_rs_parity_size)
              + d_rs_codeblock_size],
          d_rs_parity_buffer, d_rs_parity_size);
    }
    /*Initialize again RS code with fill*/
    init_rs_code (d_rs_codeblock_size - (len % d_rs_codeblock_size));
    encode_rs_char (d_rs_code, (unsigned char *) in, d_rs_parity_buffer);
    memcpy (&out[i * (d_rs_codeblock_size + d_rs_parity_size)],
            &in[i * d_rs_codeblock_size],
            d_rs_codeblock_size - (len % d_rs_codeblock_size));
    memcpy (
        &out[i * (d_rs_codeblock_size + d_rs_parity_size) + d_rs_codeblock_size
            - (len % d_rs_codeblock_size)],
        d_rs_parity_buffer, d_rs_parity_size);
    rs_bytes = i * (d_rs_codeblock_size + d_rs_parity_size)
        + d_rs_codeblock_size - (len % d_rs_codeblock_size) + d_rs_parity_size;
    init_rs_code (0);
  }
  return rs_bytes;
}

void
rs_encoder::reset ()
{
  return;
}

ssize_t
rs_encoder::encode_once (uint8_t *out, const uint8_t *in, size_t len)
{
  return -1;
}
} /* namespace ccsds */
} /* namespace gr */

