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
#ifndef INCLUDED_CCSDS_CONV_DECODER_H
#define INCLUDED_CCSDS_CONV_DECODER_H

#include <ccsds/api.h>
#include <ccsds/decoder.h>

namespace gr
{
namespace ccsds
{

/*!
 * \brief CCSDS compatible convolutional decoder with puncturing support
 *
 */
class CCSDS_API conv_decoder : public decoder
{
public:
  typedef enum
  {
    RATE_1_2 = 0,
    RATE_2_3,
    RATE_3_4,
    RATE_5_6,
    RATE_7_8
  } coding_rate_t;

  static decoder::decoder_sptr
  make(coding_rate_t coding_rate, size_t max_frame_len);

  conv_decoder (coding_rate_t coding_rate, size_t max_frame_len);
  virtual ~conv_decoder ();

  void
  reset();

  ssize_t
  decode (uint8_t *out, const uint8_t *in, size_t len);

  ssize_t
  decode_once (uint8_t *out, const uint8_t *in, size_t len);

private:
  const coding_rate_t   d_rate;
  size_t                d_trunc_depth;
  uint8_t               *d_syms;
  uint8_t               *d_unpacked;
  uint8_t               d_packed_b;
  bool                  d_first_block;
  void                  *d_vp;
  uint32_t              d_last_state;

  size_t
  decode_block(uint8_t *out, const uint8_t *in);
};

} // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CONV_DECODER_H */

