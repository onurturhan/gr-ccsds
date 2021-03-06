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
#include <ccsds/pcm_encoder.h>


namespace gr {
namespace ccsds {

pcm_encoder::pcm_encoder(pcm_type_t type):
    d_type(type),
    d_prev_bit(0)
{
}

pcm_encoder::~pcm_encoder()
{
}

pcm_type_t
pcm_encoder::get_pcm_type(){
  return d_type;
}

ssize_t
pcm_encoder::encode_trunc(uint8_t *out, uint8_t *in, size_t length)
{
  ssize_t ret = pcm(out, in, length);
  return ret;
}

ssize_t
pcm_encoder::encode(uint8_t *out, uint8_t *in, size_t length){
  ssize_t ret = pcm(out, in, length);
  d_prev_bit = 0;
  return ret;
}

ssize_t
pcm_encoder::pcm(uint8_t *out, uint8_t *in, size_t length)
{
  ssize_t return_len = 0;
  switch(d_type){
    case NRZ_L:
    case UNCODED:
      memcpy(out, in, length*sizeof(uint8_t));
      return_len = length;
      break;
    case NRZ_M:
      for(size_t i=0; i< length; i++){
        if(in[i]){
          d_prev_bit = d_prev_bit ^ 1;
          out[i] = d_prev_bit;
        }
        else{
          out[i] = d_prev_bit;
        }

      }
      return_len = length;
      break;
    case NRZ_S:
      for(size_t i=0; i< length; i++){
        for(i=0; i< length; i++){
          if(in[i]){
            out[i] = d_prev_bit;
          }
          else{
          d_prev_bit = d_prev_bit ^ 1;
          out[i] = d_prev_bit;
          }
        }
      }
      return_len = length;
      break;
    case BPL:
      for(size_t i = 0; i< length; i++){
        if(in[i]){
          out[i] = 0x02;
        }
        else{
          out[i] = 0x01;
        }
      }
      return_len = length*2;
      break;
    default:
      return_len = -1;
      break;
  }
  return return_len;
}

} /* namespace ccsds */
} /* namespace gr */

