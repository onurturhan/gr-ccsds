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
rs_encoder::make (ecc_t ecc, interleaver_t inter_depth)
{
  return encoder::encoder_sptr (
     new rs_encoder (ecc, inter_depth));
}

rs_encoder::rs_encoder(ecc_t ecc, interleaver_t inter_depth):
    d_buffers(inter_depth, new uint8_t[255])
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
  d_rs_data_per_codeblock = 255 - d_rs_parity_size;

  switch(inter_depth) {
    case INTERLEAVER_DEPTH_1:
    case INTERLEAVER_DEPTH_2:
    case INTERLEAVER_DEPTH_3:
    case INTERLEAVER_DEPTH_4:
    case INTERLEAVER_DEPTH_5:
    case INTERLEAVER_DEPTH_8:
      d_inter_depth = inter_depth;
      break;
    default:
      throw std::invalid_argument("rs_decoder: Invalid interleaving depth");
  }
  std::vector<uint8_t*> temp(d_inter_depth);
  for (int i=0; i< d_inter_depth; i++){
    temp[i] = new uint8_t[d_rs_parity_size];
  }
  d_parity = temp;
  d_max_frame_len = 255*d_inter_depth; // As stated in CCSDS, max frame length will be 255* inteleaving_depth
  d_rs_code = NULL;
  init_rs_code (0);
}

rs_encoder::~rs_encoder()
{
  for(uint8_t *i : d_buffers) {
    delete [] i;
  }
  for(uint8_t *i : d_parity) {
    delete [] i;
  }

  free_rs_char(d_rs_code);
}

uint8_t
rs_encoder::init_rs_code (int fill)
{
  if(d_rs_code){
    free_rs_char(d_rs_code);
  }
  d_rs_code = init_rs_char (8, 0x187, 112, 11, d_rs_parity_size, fill);
  if (d_rs_code)
    return 1;
  else
    return 0;
}



ssize_t
rs_encoder::encode (uint8_t *out, const uint8_t *in, size_t len)
{
  size_t vfill = 0;
  if(len%8 != 0 ){
    return -1;
  }
  size_t byte_len = len*8;
  if(byte_len%((d_rs_data_per_codeblock)*d_inter_depth) != 0){
    if( (byte_len%((d_rs_data_per_codeblock)*d_inter_depth))%d_inter_depth != 0 ){
      return -1;
    }
    else{
      vfill = (byte_len%((d_rs_data_per_codeblock)*d_inter_depth))/d_inter_depth;
    }
  }
  size_t rs_bytes = 0;
  size_t total_codeblocks = byte_len/((d_rs_data_per_codeblock)*d_inter_depth);
  init_rs_code (0);
  int i = 0, j = 0, t = 0, s = 0;
  for(i =0; i<total_codeblocks ; i++){
    for(t = 0; t < d_rs_data_per_codeblock; t++){
      d_buffers[s][t] = in[i*255*d_inter_depth + t];
      s = (s+1)%d_inter_depth;
    }
    t= 0;
    for (uint8_t *i : d_buffers) {
      encode_rs_char (d_rs_code, i, (unsigned char*)d_parity[t]);
      t++;
    }
    /*Copy input data to output*/
    memcpy(&out[i*255*d_inter_depth],
           &in[i*d_rs_data_per_codeblock*d_inter_depth],
           d_rs_data_per_codeblock*d_inter_depth*sizeof(uint8_t));
    /*Copy parity to output*/
    s=0;
    for(t = 0; t< d_inter_depth*d_rs_parity_size; t++){
      out[i*d_rs_data_per_codeblock*d_inter_depth + d_rs_data_per_codeblock*d_inter_depth + t] = d_parity[s][t/d_rs_parity_size];
      s = (s+1)%d_inter_depth;
    }
  }
  if(vfill){
    init_rs_code (vfill);
    s = 0;
    i = total_codeblocks;
    for(t = 0; t < d_rs_data_per_codeblock - vfill; t++){
      d_buffers[s][t] = in[i*255*d_inter_depth + t];
      s = (s+1)%d_inter_depth;
    }
    t = 0;
    for (uint8_t *i : d_buffers) {
      encode_rs_char (d_rs_code, i, (unsigned char*)d_parity[t]);
      t++;
    }
    /*Copy input data to output*/
    memcpy(&out[i*255*d_inter_depth],
           &in[i*(d_rs_data_per_codeblock - vfill)*d_inter_depth],
           (d_rs_data_per_codeblock - vfill)*d_inter_depth*sizeof(uint8_t));
    /*Copy parity to output*/
    s = 0;
    for(t = 0; t< d_inter_depth*d_rs_parity_size; t++){
      out[i*(d_rs_data_per_codeblock - vfill)*d_inter_depth + (d_rs_data_per_codeblock - vfill)*d_inter_depth + t] = d_parity[s][t/d_rs_parity_size];
      s = (s+1)%d_inter_depth;
    }
  }
  if(vfill){
    rs_bytes = total_codeblocks*255*d_inter_depth + d_inter_depth*(255 - vfill);
  }
  else{
    rs_bytes = total_codeblocks*255*d_inter_depth;
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

