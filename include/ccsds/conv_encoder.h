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

#ifndef INCLUDED_CCSDS_CONV_ENCODER_H
#define INCLUDED_CCSDS_CONV_ENCODER_H

#include <ccsds/api.h>
#include <ccsds/encoder.h>
#include <cstdint>
#include <cstdlib>
#include <itpp/base/array.h>
#include <itpp/comm/convcode.h>
#include <itpp/comm/reedsolomon.h>

namespace gr
{
namespace ccsds
{
//using namespace itpp;

class CCSDS_API conv_encoder : public encoder
{
/**
* \brief Return a shared_ptr to a new instance of encoder::conv_encoder
* @param cc_rate Convolutional Code rate.
*/
public:
  typedef enum
  {
    RATE_1_2 = 0,
    RATE_2_3,
    RATE_3_4,
    RATE_5_6,
    RATE_7_8
  } coding_rate_t;

  static encoder::encoder_sptr
  make (coding_rate_t cc_rate, size_t max_frame_len);

  conv_encoder(coding_rate_t cc_rate, size_t max_frame_len);
  ~conv_encoder();

  ssize_t
  encode (uint8_t *out, const uint8_t *in, size_t len);

  ssize_t
  encode_trunc (uint8_t *out, const uint8_t *in, size_t len);

  void
  reset ();

  ssize_t
  finalize(uint8_t * out);

private:
  coding_rate_t                 d_cc_rate;
  itpp::Convolutional_Code      d_conv_code;
  itpp::ivec                    d_cc_generator;

  size_t
  inv_and_puncture(uint8_t* out, const itpp::bvec &in);
};
}
}

#endif /* INCLUDED_CCSDS_CONV_RS_ENCODER_H */
