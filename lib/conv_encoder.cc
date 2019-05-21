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
conv_encoder::make (conv_encoder::coding_rate_t cc_rate, size_t max_frame_len)
{
  return encoder::encoder_sptr (new conv_encoder (cc_rate, max_frame_len));
}

conv_encoder::conv_encoder (coding_rate_t cc_rate, size_t max_frame_len) :
        encoder (max_frame_len)
{
  d_cc_rate = cc_rate;
  d_cc_generator.set_length (2);
  d_cc_generator[0] = 0171;
  d_cc_generator[1] = 0133;
  d_conv_code.set_generator_polynomials (d_cc_generator, 7); // Constraint length = 7
  d_conv_code.init_encoder ();
  d_conv_code.set_start_state (0);
  d_frame_idx = 0;
}

conv_encoder::~conv_encoder ()
{
}

ssize_t
conv_encoder::encode (uint8_t *out, const uint8_t *in, size_t len)
{
  itpp::bvec unencoded (0);
  itpp::bvec cc_encoded (0);
  d_frame_idx = 0;
  for(size_t i = 0; i < len; i++) {
    unencoded = itpp::concat(unencoded, itpp::bin(in[i]));
  }
  d_conv_code.encode_tail (unencoded, cc_encoded);
  reset();
  return inv_and_puncture (out, cc_encoded);
}

ssize_t
conv_encoder::encode_trunc (uint8_t *out, const uint8_t *in, size_t len)
{
  itpp::bvec unencoded (0);
  itpp::bvec cc_encoded (0);
  for(size_t i = 0; i < len; i++) {
    unencoded = itpp::concat(unencoded, itpp::bin(in[i]));
  }
  d_conv_code.encode_trunc (unencoded, cc_encoded);
  return inv_and_puncture (out, cc_encoded);
}

ssize_t
conv_encoder::finalize (uint8_t* out)
{
  itpp::bvec unencoded (0);
  itpp::bvec cc_encoded (0);
  unencoded.set_length (6, false);
  unencoded.zeros ();
  size_t return_length = 0;
  d_conv_code.encode_trunc (unencoded, cc_encoded);
  return_length = inv_and_puncture (out, cc_encoded);
  reset ();
  return return_length;
}

void
conv_encoder::reset ()
{
  d_conv_code.set_start_state (0);
  d_frame_idx = 0;
}

size_t
conv_encoder::inv_and_puncture (uint8_t* out, const itpp::bvec &in)
{
  size_t cnt = 0;
  switch(d_cc_rate) {
    case RATE_1_2:
      for(int i = 0; i < in.length() / 2; i++) {
        out[2 * i] = in[2*i].value();
        out[2 * i + 1] = !in[2*i + 1].value();
      }
      return in.length();
    /* Output B inversion is not applicable for punctured codes */
    case RATE_2_3:
      for(int i = 0; i < in.length(); i++) {
        if (d_frame_idx % 4 != 2) {
          out[cnt++] = in[i].value();
        }
        d_frame_idx++;
      }
      return cnt;
    case RATE_3_4:
      for (int i = 0; i < in.length (); i++) {
        if(d_frame_idx % 4 == 1 || d_frame_idx % 4 == 3) {
          d_frame_idx++;
          continue;
        }
        out[cnt++] = in[i].value();
        d_frame_idx++;
      }
      return cnt;
    default:
      throw std::runtime_error("conv_encoder: invalid coding rate");
  }
}

}
}

