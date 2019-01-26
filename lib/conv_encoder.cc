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
conv_encoder::make (coding_rate_t cc_rate,
                     size_t max_frame_len)
{
  return encoder::encoder_sptr (
  new conv_encoder (cc_rate, max_frame_len));
}

conv_encoder::conv_encoder (coding_rate_t cc_rate, size_t max_frame_len)
{
  d_cc_rate = cc_rate;
  d_cc_generator.set_length (2);
  d_cc_generator[0] = 0171;
  d_cc_generator[1] = 0133;
  d_conv_code.set_generator_polynomials (d_cc_generator, 7); // Constraint length = 7
  d_conv_code.init_encoder ();
  d_conv_code.set_start_state (0);
}

conv_encoder::~conv_encoder ()
{
  return;
}

ssize_t
conv_encoder::encode_once (uint8_t *out, const uint8_t *in,
                           size_t len)
{
  return 0;
}

ssize_t
conv_encoder::encode (uint8_t *out, const uint8_t *in, size_t len)
{
  itpp::bvec unencoded (0);
  itpp::bvec cc_encoded (0);
  d_conv_code.set_start_state(0);
  bytes_to_bvec (unencoded, in, len);
  d_conv_code.encode_trunc (unencoded, cc_encoded);
  bvec_to_bytes (out, cc_encoded);
  return len*2; //Fixed rate 1/2 for Convolutional Encoding
}

void
conv_encoder::reset ()
{
  return;
}

void
conv_encoder::bytes_to_bvec (itpp::bvec &out, const uint8_t* buffer, size_t len)
{
  itpp::bvec byte;
  for (int i = 0; i < len; i++) {
    byte = itpp::dec2bin (8, buffer[i]);
    itpp::concat (out, byte);
  }
}

void
conv_encoder::bvec_to_bytes (uint8_t *out, itpp::bvec in)
{
  size_t len = in.length ();
  itpp::bvec byte;
  for (int i = 0; i < len / 8; i++) {
     byte = in.get (i * 8, (i + 1) * 8);
     out[i] = itpp::bin2dec (byte, true);
   }
}

}
}

