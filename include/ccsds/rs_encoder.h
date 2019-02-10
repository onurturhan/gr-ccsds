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
#ifndef INCLUDED_CCSDS_RS_ENCODER_H
#define INCLUDED_CCSDS_RS_ENCODER_H

#include <ccsds/api.h>
#include <ccsds/encoder.h>
#include <cstring>
#include <vector>


namespace gr {
namespace ccsds {

  /*!
* \brief Return a shared_ptr to a new instance of encoder::rs_encoder
* @param rs_ecc Reed-Solomon Error Correcting Capability.
   */
class CCSDS_API rs_encoder : public encoder
{
public:
  /**
   * Error Correction capability mode
   */
  typedef enum{
    ECC_8=0,//!< ECC_8
    ECC_16  //!< ECC_16
  }ecc_t;

  typedef enum {
    INTERLEAVER_DEPTH_1 = 1,
    INTERLEAVER_DEPTH_2 = 2,
    INTERLEAVER_DEPTH_3 = 3,
    INTERLEAVER_DEPTH_4 = 4,
    INTERLEAVER_DEPTH_5 = 5,
    INTERLEAVER_DEPTH_8 = 8
  } interleaver_t;

  rs_encoder(ecc_t ecc, interleaver_t inter_depth);
  ~rs_encoder();

  static encoder::encoder_sptr
  make (ecc_t ecc, interleaver_t inter_depth);

  ssize_t
  encode_once (uint8_t *out, const uint8_t *in, size_t len);

  ssize_t
  encode (uint8_t *out, const uint8_t *in, size_t len);

  void
  reset ();

private:

  void *                        d_rs_code;
  uint8_t                       d_rs_ecc;
  size_t                        d_data_per_cdblk;
  size_t                        d_rs_parity;
  interleaver_t                 d_inter_depth;
  size_t                        d_max_frame_len;
  std::vector<uint8_t *>        d_buffers;

  uint8_t init_rs_code(int fill);
};

} // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_RS_ENCODER_H */

