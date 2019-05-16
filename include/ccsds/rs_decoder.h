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

#ifndef INCLUDED_CCSDS_RS_DECODER_H
#define INCLUDED_CCSDS_RS_DECODER_H

#include <ccsds/api.h>
#include <ccsds/decoder.h>
#include <vector>

namespace gr
{
namespace ccsds
{

/*!
 * \brief CCSDS Compatible RS decoder
 *
 */
class CCSDS_API rs_decoder : public decoder
{
public:
  /**
   * Error Correction capability mode
   */
  typedef enum {
    ECC_8 = 8,          //!< 8-bits parity
    ECC_16 = 16         //!< 16-bits parity
  } ecc_t;

  typedef enum {
    INTERLEAVER_DEPTH_1 = 1,
    INTERLEAVER_DEPTH_2 = 2,
    INTERLEAVER_DEPTH_3 = 3,
    INTERLEAVER_DEPTH_4 = 4,
    INTERLEAVER_DEPTH_5 = 5,
    INTERLEAVER_DEPTH_8 = 8
  } interleaver_t;

  static decoder::decoder_sptr
  make(rs_decoder::ecc_t ecc, rs_decoder::interleaver_t inter);

  rs_decoder (ecc_t ecc, interleaver_t inter);

  ~rs_decoder ();

  ssize_t
  decode (uint8_t *out, const int8_t *in, size_t len);

  ssize_t
  decode_trunc (uint8_t *out, const int8_t *in, size_t len);

  void
  reset();
private:
  size_t                        d_parity_bytes;
  size_t                        d_inter_depth;
  std::vector<uint8_t *>        d_buffers;

  void
  reset_buffers();
};

} // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_RS_DECODER_H */

