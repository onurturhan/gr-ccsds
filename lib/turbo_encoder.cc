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
#include <ccsds/turbo_encoder.h>
#include <ccsds/utils.h>

namespace gr
{
namespace ccsds
{

const size_t turbo_encoder::d_rate2_tail_perm[8] =
  { 0, 1, 2, 11, 4, 5, 6, 15 };
const size_t turbo_encoder::d_rate3_tail_perm[12] =
  { 0, 1, 9, 2, 3, 11, 4, 5, 13, 6, 7, 15 };
const size_t turbo_encoder::d_rate4_tail_perm[16] =
  { 0, 1, 2, 13, 3, 4, 5, 15, 6, 7, 8, 17, 9, 10, 11, 19 };
const size_t turbo_encoder::d_rate6_tail_perm[24] =
  { 0, 1, 2, 3, 17, 18, 4, 5, 6, 7, 20, 21, 8, 9, 10, 11, 23, 24, 12, 13, 14,
      15, 26, 27 };
const size_t* turbo_encoder::d_tail_permutations[4] =
  { turbo_encoder::d_rate2_tail_perm, turbo_encoder::d_rate3_tail_perm,
      turbo_encoder::d_rate4_tail_perm, turbo_encoder::d_rate6_tail_perm };

encoder::encoder_sptr
turbo_encoder::make (coding_rate_t turbo_rate, block_len_t block_len,
                     size_t max_frame_len)
{
  return encoder::encoder_sptr (
      new turbo_encoder (turbo_rate, block_len, max_frame_len));
}

turbo_encoder::turbo_encoder (coding_rate_t turbo_rate, block_len_t block_len,
                              size_t max_frame_len) :
        d_turbo_rate (turbo_rate),
        d_block_len (block_len),
        d_buffer(nullptr)
{
  d_constr_len = 5;
  d_vec_len = 0;
  d_rate = 0;
  d_tail_size = 0;
  calc_permutations ();
  itpp::ivec gen_polys1;
  itpp::ivec gen_polys2;
  switch (d_turbo_rate)
    {
    case RATE_1_2:
      d_rate = 2;
    case RATE_1_3:
      gen_polys1.set_length (2, false);
      gen_polys1 (0) = 023;
      gen_polys1 (1) = 033;
      gen_polys2.set_length (2, false);
      gen_polys2 (0) = 023;
      gen_polys2 (1) = 033;
      if (d_rate == 0) {
        d_rate = 3;
      }
      break;
    case RATE_1_4:
      gen_polys1.set_length (3, false);
      gen_polys2.set_length (2, false);
      gen_polys1 (0) = 023;
      gen_polys1 (1) = 025;
      gen_polys1 (2) = 037;

      gen_polys2 (0) = 023;
      gen_polys2 (1) = 033;
      d_rate = 4;
      break;
    case RATE_1_6:
      gen_polys1.set_length (4, false);
      gen_polys2.set_length (3, false);
      gen_polys1 (0) = 023;
      gen_polys1 (1) = 033;
      gen_polys1 (2) = 025;
      gen_polys1 (3) = 037;

      gen_polys2 (0) = 023;
      gen_polys2 (1) = 033;
      gen_polys2 (2) = 037;
      d_rate = 6;
      break;
    }
  d_tail_size = 4 * d_rate;
  d_turbo_codec.set_parameters (gen_polys1, gen_polys2, d_constr_len,
                                d_permutations);

}

turbo_encoder::~turbo_encoder ()
{
}

void
turbo_encoder::calc_permutations ()
{
  std::vector<int> pq (8);
  pq[0] = 31;
  pq[1] = 37;
  pq[2] = 43;
  pq[3] = 47;
  pq[4] = 53;
  pq[5] = 59;
  pq[6] = 61;
  pq[7] = 67;
  const int k1 = 8;
  int k2 = 0;
  switch (d_block_len)
    {
    case LEN_1784:
      d_vec_len = 1784;
      k2 = 223;
      break;
    case LEN_3568:
      d_vec_len = 3568;
      k2 = 223 * 2;
      break;
    case LEN_7136:
      d_vec_len = 7136;
      k2 = 223 * 4;
      break;
    case LEN_8920:
      d_vec_len = 8920;
      k2 = 223 * 5;
      break;
    }
  int m, i, j, t, q, c, res;
  d_permutations.set_length (d_vec_len, false);
  for (int s = 1; s <= d_vec_len; s++) {
    m = (s - 1) % 2;
    i = (s - 1) / (2 * k2);
    j = (s - 1) / 2 - (i * k2);
    t = (19 * i + 1) % (k1 / 2);
    q = (t % 8) + 1;
    c = (pq[q - 1] * j + 21 * m) % k2;
    res = 2 * (t + c * (k1 / 2) + 1) - m;
    d_permutations.set (s - 1, res - 1);
  }
}
ssize_t
turbo_encoder::encode (uint8_t *out, const uint8_t *in, size_t len)
{
  return 0;
}

ssize_t
turbo_encoder::encode_trunc (uint8_t *out, const uint8_t *in, size_t len)
{
  if (len % d_vec_len != 0) {
    return -1;
  }

  itpp::bvec turbo_uncoded (0);
  itpp::bvec turbo_coded (0);
  itpp::bvec turbo_coded_temp (0);
  /*Convert to bvec*/
  for(size_t i=0; i< len; i++){
    turbo_uncoded = itpp::concat (turbo_uncoded, itpp::bin(in[i]));
  }
  d_turbo_codec.encode (turbo_uncoded, turbo_coded_temp);
  turbo_coded = turbo_coded_temp;
  size_t index = 0;
  /* Puncturing for rate 1/2 */
  if (d_turbo_rate == RATE_1_2) {
    handle_tail_perm (turbo_coded, turbo_coded_temp);
    for (size_t i = 0; i < d_vec_len * 3 + d_tail_size; i++) {
      /*The tail has already been punctured in the handle_tail_perm function*/
      if(i >= d_vec_len * 3){
        out[index] = turbo_coded[i].value ();
        index++;
        continue;
      }
      switch (i % 6)
        {
        case 3:
        case 5:
          break;
        default:
          out[index] = turbo_coded[i].value ();
        }
      index++;
    }
  }
  else {
    handle_tail_perm (turbo_coded, turbo_coded_temp);
    for (size_t i = 0; i < d_vec_len * 3 + d_tail_size; i++) {
      out[index] = turbo_coded[i].value ();
      index++;
    }
  }
  return turbo_coded.length ();
}

void
turbo_encoder::reset ()
{
  return;
}

void
turbo_encoder::handle_tail_perm (itpp::bvec &out, itpp::bvec &in)
{
  size_t start_index = 0;
  if (d_turbo_rate == RATE_1_2) {
    start_index = out.length () - 16;
  }
  else {
    start_index = out.length () - (d_tail_size + 4);
  }
  for (size_t it = start_index; it < start_index + d_tail_size; it++) {
    out.set(it,
            in.get(start_index + d_tail_permutations[d_turbo_rate][it - start_index]));
  }
  return;
}

} /* namespace ccsds */
} /* namespace gr */

