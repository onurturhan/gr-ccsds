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

  rs_encoder(ecc_t ecc, size_t max_frame_len);
  ~rs_encoder();

  static encoder::encoder_sptr
  make (ecc_t ecc, size_t max_frame_len);

  ssize_t
  encode_once (uint8_t *out, const uint8_t *in, size_t len);

  ssize_t
  encode (uint8_t *out, const uint8_t *in, size_t len);

  void
  reset ();

private:

  void * d_rs_code;
  uint8_t* d_rs_parity_buffer;
  uint8_t d_rs_ecc;
  size_t d_rs_codeblock_size;
  size_t d_rs_parity_size;

  uint8_t init_rs_code(int fill);
};

} // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_RS_ENCODER_H */

