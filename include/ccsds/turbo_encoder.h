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

#ifndef INCLUDED_CCSDS_TURBO_ENCODER_H
#define INCLUDED_CCSDS_TURBO_ENCODER_H

#include <ccsds/api.h>
#include <ccsds/encoder.h>
#include <itpp/base/array.h>
#include <itpp/comm/turbo.h>
#include <vector>

namespace gr {
namespace ccsds {

  /*!
   * \brief <+description+>
   *
   */
class CCSDS_API turbo_encoder : public encoder
{
public:
  typedef enum
  {
    RATE_1_2 = 0,
    RATE_1_3,
    RATE_1_4,
    RATE_1_6
  } coding_rate_t;

  typedef enum
  {
    LEN_1784 = 0,
    LEN_3568,
    LEN_7136,
    LEN_8920
  } block_len_t;

  turbo_encoder(coding_rate_t turbo_rate, block_len_t block_len,
                size_t max_frame_len);
  ~turbo_encoder();

  static encoder::encoder_sptr
  make(coding_rate_t turbo_rate, block_len_t block_len, size_t max_frame_len);

  ssize_t
  encode (uint8_t *out, const uint8_t *in, size_t len);

  ssize_t
  encode_trunc (uint8_t *out, const uint8_t *in, size_t len);

  void
  reset ();

private:
  coding_rate_t     d_turbo_rate;
  block_len_t       d_block_len;
  itpp::Turbo_Codec d_turbo_codec;
  itpp::ivec        d_permutations;
  size_t            d_constr_len;
  uint8_t*          d_buffer;
  size_t            d_vec_len;
  size_t            d_rate;
  size_t            d_tail_size;
 // const size_t** d_tail_permutations = new size_t*[4];
  static const size_t* d_tail_permutations[4];
  static const size_t d_rate2_tail_perm[8];
  static const size_t d_rate3_tail_perm[12];
  static const size_t d_rate4_tail_perm[16];
  static const size_t d_rate6_tail_perm[24];

  void calc_permutations();
  void bytes_to_bvec (itpp::bvec &out, const uint8_t* buffer, size_t len);
  void bvec_to_bytes (uint8_t *out, itpp::bvec in);
  /**
   * Due to an incompatibility of the itpp library for the turbo encoding and
   * the CCSDS recommendation, a permutation of the tail bits introduced
   * by itpp needs to take place. itpp inserts the tailbits of the first
   * encoder and then the ones of the second decoder instead of interleaving
   * them as ccsds requires
   */
  void handle_tail_perm(itpp::bvec &out, itpp::bvec &in);

};

} // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_TURBO_ENCODER_H */

