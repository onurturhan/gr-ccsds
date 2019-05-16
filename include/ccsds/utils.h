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

#ifndef INCLUDE_CCSDS_UTILS_H_
#define INCLUDE_CCSDS_UTILS_H_

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>



namespace gr
{

namespace ccsds
{

static size_t
packed_to_unpacked(uint8_t *out, const uint8_t *in, size_t len)
{
  if(!out || !in || !len) {
    return 0;
  }
  for(size_t i = 0; i < len; i++) {
    *out++ = (in[i] >> 7) & 0x1;
    *out++ = (in[i] >> 6) & 0x1;
    *out++ = (in[i] >> 5) & 0x1;
    *out++ = (in[i] >> 4) & 0x1;
    *out++ = (in[i] >> 3) & 0x1;
    *out++ = (in[i] >> 2) & 0x1;
    *out++ = (in[i] >> 1) & 0x1;
    *out++ = (in[i] ) & 0x1;
  }
  return len * 8;
}

static size_t
packed_to_unpacked_soft(int8_t *out, const uint8_t *in, size_t len)
{
  if(!out || !in || !len) {
    return 0;
  }
  for(size_t i = 0; i < len; i++) {
    *out++ = (((in[i] >> 7) & 0x1) * 255) - 128;
    *out++ = (((in[i] >> 6) & 0x1) * 255) - 128;
    *out++ = (((in[i] >> 5) & 0x1) * 255) - 128;
    *out++ = (((in[i] >> 4) & 0x1) * 255) - 128;
    *out++ = (((in[i] >> 3) & 0x1) * 255) - 128;
    *out++ = (((in[i] >> 2) & 0x1) * 255) - 128;
    *out++ = (((in[i] >> 1) & 0x1) * 255) - 128;
    *out++ = (((in[i] ) & 0x1) * 255) - 128;
  }
  return len * 8;
}

static size_t
unpacked_to_packed(uint8_t *out, const uint8_t *in, size_t len)
{
  if(!out || !in || !len) {
    return 0;
  }
  struct h {
    uint8_t shift:3;
  };
  struct h x;
  x.shift = 7;
  memset (out, 0, static_cast<size_t> (std::ceil (len / 8.0)));
  for(size_t i = 0; i < len; i++) {
    out[i/8] |= ((in[i] & 0x1) << x.shift--);
  }
  return static_cast<size_t> (std::ceil (len / 8.0));
}

static size_t
unpacked_to_packed_soft(uint8_t *out, const int8_t *in, size_t len)
{
  if(!out || !in || !len) {
    return 0;
  }
  struct h {
    uint8_t shift:3;
  };
  struct h x;
  x.shift = 7;
  memset (out, 0, static_cast<size_t> (std::ceil (len / 8.0)));
  for(size_t i = 0; i < len; i++) {
    uint8_t bit = in[i] > 0 ? 1 : 0;
    out[i/8] |= (bit << x.shift--);
  }
  return static_cast<size_t> (std::ceil (len / 8.0));
}

static void
hard_to_soft_int8(int8_t *out, const uint8_t *in, size_t len)
{
  for(size_t i = 0; i < len; i++) {
    out[i] = ((in[i] & 0x1) * 255) -128;
  }
}


}  // namespace ccsds


}  // namespace gr



#endif /* INCLUDE_CCSDS_UTILS_H_ */
