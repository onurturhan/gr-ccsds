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

#include <ccsds/conv_encoder.h>

namespace gr
{
namespace ccsds
{
encoder::encoder_sptr
conv_encoder::make (conv_encoder::coding_rate_t cc_rate,
                    size_t max_frame_len)
{
  return encoder::encoder_sptr (
  new conv_encoder (cc_rate, max_frame_len));
}

conv_encoder::conv_encoder (coding_rate_t cc_rate, size_t max_frame_len):
    encoder (max_frame_len)
{
  d_cc_rate = cc_rate;
  d_cc_generator.set_length (2);
  d_cc_generator[0] = 0171;
  d_cc_generator[1] = 0133;
  d_conv_code.set_generator_polynomials (d_cc_generator, 7); // Constraint length = 7
  d_conv_code.init_encoder ();
  d_conv_code.set_start_state (0);
  d_buffer = new uint8_t[max_frame_len];
  d_punctured_index = 0;
}

conv_encoder::~conv_encoder ()
{
  delete [] d_buffer;
  return;
}

ssize_t
conv_encoder::encode (uint8_t *out, const uint8_t *in,
                           size_t len)
{
  itpp::bvec unencoded(0);
  itpp::bvec cc_encoded(0);
  size_t return_length = 0;
  bytes_to_bvec (unencoded, in, len);
  d_conv_code.encode_tail (unencoded, cc_encoded);
  bvec_to_bytes (d_buffer, cc_encoded);
  return_length = puncture(out, len+6);
  memset(d_buffer, 0, max_frame_len()*sizeof(uint8_t));
  return return_length;
}

ssize_t
conv_encoder::encode_trunc (uint8_t *out, const uint8_t *in, size_t len)
{
  itpp::bvec unencoded(0);
  itpp::bvec cc_encoded(0);
  size_t return_length = 0;
  bytes_to_bvec (unencoded, in, len);
  d_conv_code.encode_trunc (unencoded, cc_encoded);
  bvec_to_bytes (d_buffer, cc_encoded);
  return_length = puncture(out, len);
  memset(d_buffer, 0, max_frame_len()*sizeof(uint8_t));
  return return_length;
}

ssize_t
conv_encoder::finalize(uint8_t* out){
  size_t len = 6;
  itpp::bvec unencoded(0);
  itpp::bvec cc_encoded(0);
  unencoded.set_length(6,false);
  unencoded.zeros();
  size_t return_length = 0;
  d_conv_code.encode_trunc (unencoded, cc_encoded);
  bvec_to_bytes (d_buffer, cc_encoded);
  return_length = puncture(out, 6);
  memset(d_buffer, 0, max_frame_len()*sizeof(uint8_t));
  reset();
  return return_length;
}

void
conv_encoder::reset ()
{
  d_conv_code.set_start_state(0);
  d_punctured_index = 0;
  return;
}

void
conv_encoder::bytes_to_bvec(itpp::bvec &out, const uint8_t* buffer, size_t len)
{
  itpp::bvec byte;
  for (int i = 0; i < len/8; i++) {
    byte = itpp::dec2bin (8, buffer[i]);
    out = itpp::concat (out, byte);
  }
}

void
conv_encoder::bvec_to_bytes(uint8_t *out, itpp::bvec in)
{
  int len = in.length ();
  itpp::bvec byte(0);
  for (int i = 0; i < len / 8; i++) {
     byte = in.get (i * 8, (i + 1) * 8 - 1);
     out[i] = itpp::bin2dec (byte, true);
  }
  for(int i = (len / 8) * 8; i < len; i++) {
    out[i/8] = (out[i/8] << 1) | (in.get(i).value() & 0x1);
  }
  out[len/8] <<= 8 - (len - (len/8) * 8);
}

size_t
conv_encoder::puncture(uint8_t* out, size_t init_length){
  size_t i;
  size_t return_length = 0;
  if(d_cc_rate == RATE_1_2){
    return_length = 2 * init_length;
  /* Invert output of G2*/
    memcpy(out, d_buffer, (return_length / 8 + 1)*sizeof(uint8_t));
    for(i=0; i< return_length/8; i++){
      out[i] ^= 0x40;
      out[i] ^= 0x10;
      out[i] ^= 0x04;
      out[i] ^= 0x01;
    }
    if(return_length%8 != 0){
      switch(return_length%8){
        case 2:
          out[return_length/8] ^= 0x40;
          break;
        case 4:
          out[return_length/8] ^= 0x40;
          out[return_length/8] ^= 0x10;
          break;
        case 6:
          out[return_length/8] ^= 0x40;
          out[return_length/8] ^= 0x10;
          out[return_length/8] ^= 0x04;
          break;
      }
    }
  }
  else if(d_cc_rate == RATE_2_3){
    size_t out_buff_index = 0;
    int byte_index = 7;
    for(i = d_punctured_index; i <d_punctured_index + init_length*2; i++){
      if( (i % 4 == 2))
      {
        continue;
      }
      else{
        out[out_buff_index] |= ((d_buffer[(i-d_punctured_index)/8] >> (7 - (i-d_punctured_index)%8)) & 1) << byte_index;
        byte_index--;
        if(byte_index < 0){
          byte_index = 7;
          out_buff_index++;
          out[out_buff_index] = 0;
        }
      }
    }
    return_length = init_length + init_length/2;
  }
  else if(d_cc_rate == RATE_3_4){
    size_t out_buff_index = 0;
    int byte_index = 7;
    for(i = d_punctured_index; i < d_punctured_index + init_length*2; i++){
      if( (i % 6 == 2) ||
          (i % 6 == 5))
      {
        continue;
      }
      else{
        out[out_buff_index] |= ((d_buffer[(i-d_punctured_index)/8] >> (7 - (i-d_punctured_index)%8)) & 1) << byte_index;
        byte_index--;
        if(byte_index < 0){
          byte_index = 7;
          out_buff_index++;
          out[out_buff_index] = 0;
        }
      }
    }
    if(init_length % 3 == 0){
      return_length = init_length + init_length/3;
    }
    else{
      return_length = init_length + init_length/3 + (init_length%3) + 1;
    }
  }
  else if(d_cc_rate == RATE_5_6){
    size_t out_buff_index = 0;
    int byte_index = 7;
    for(i = d_punctured_index; i < d_punctured_index + init_length*2; i++){
      if( (i % 10 == 2) ||
          (i % 10 == 5) ||
          (i % 10 == 6) ||
          (i % 10 == 9))
      {
        continue;
      }
      else{
        out[out_buff_index] |= ((d_buffer[(i-d_punctured_index)/8] >> (7 - (i-d_punctured_index)%8)) & 1) << byte_index;
        byte_index--;
        if(byte_index < 0){
          byte_index = 7;
          out_buff_index++;
          out[out_buff_index] = 0;
        }
      }
    }
    if(init_length % 5 == 0){
      return_length = init_length + init_length/5;
    }
    else{
      return_length = init_length + init_length/5 + (init_length%5) + 1;
    }
  }
  else if(d_cc_rate == RATE_7_8){
    size_t out_buff_index = 0;
    int byte_index = 7;
    for(i = d_punctured_index; i < d_punctured_index + init_length*2; i++){
      if( (i % 14 == 2) ||
          (i % 14 == 4) ||
          (i % 14 == 6) ||
          (i % 14 == 9) ||
          (i % 14 == 10)||
          (i % 14 == 13))
      {
        continue;
      }
      else{
        out[out_buff_index] |= ((d_buffer[(i-d_punctured_index)/8] >> (7 - (i-d_punctured_index)%8)) & 1) << byte_index;
        byte_index--;
        if(byte_index < 0){
          byte_index = 7;
          out_buff_index++;
          out[out_buff_index] = 0;
        }
      }
      if(init_length % 7 == 0){
        return_length = init_length + init_length/7;
      }
      else{
        return_length = init_length + init_length/7 + (init_length%7) + 1;
      }
    }
  }
  d_punctured_index = i;
  return return_length;
}

}
}

