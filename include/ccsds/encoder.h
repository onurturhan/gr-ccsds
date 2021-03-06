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

#ifndef INCLUDED_CCSDS_ENCODER_H
#define INCLUDED_CCSDS_ENCODER_H

#include <ccsds/api.h>
#include <cstdint>
#include <cstdlib>
#include <boost/shared_ptr.hpp>

namespace gr
{
namespace ccsds
{

/*!
 * \brief Abstract class that provided the API for the CCSDS encoders
 *
 * This is an abstract class providing the API for the CCSDS encoders.
 * The specification provides a set of different recommendations combining
 * different coding and whitening schemes.
 *
 * The gr-ccsds module tries to provide a unified encoding/decoding framework,
 * that provides to the user only a single encoding and decoding block.
 * Specialization is performed by passing to these block the appropriate
 * encoder/decoder class that implements this abstract class API.
 */
class CCSDS_API encoder
{
public:

  typedef boost::shared_ptr<encoder> encoder_sptr;
  encoder (size_t max_frame_len = 8192);
  virtual ~encoder ();

  /**
   * Encodes a buffer. The difference with the encode_trunc() is that this method
   * resets any internal state of the encoder to the initial states.
   *
   * @param out the output buffer that will hold the encoded data
   *
   * @param in the input buffer
   *
   * @param len the length of the input buffer in <b>bits</b>
   *
   * @return the number of encoded <b>bits</b>. Due to the fact that
   * some coding schemes may not produce an output that is a multiple of
   * 8 bits, it is necessary to count in a per bit basis. If the result
   * is not a multiple of 8 bits, the LS bits are padded with zeros.
   * If an error occurred an appropriate negative error code is returned
   */
  virtual ssize_t
  encode(uint8_t *out, const uint8_t *in, size_t len) = 0;

  /**
   * Encodes a buffer. The difference with the encode() is that
   * this method does not reset the internal state of the encoder.
   * Therefore, it can be called again and continue the encoding when
   * data are available.
   *
   * @param out the output buffer that will hold the encoded data
   *
   * @param in the input buffer
   *
   * @param len the length of the input buffer in <b>bits</b>
   *
   * @return the number of encoded <b>bits</b>. Due to the fact that
   * some coding schemes may not produce an output that is a multiple of
   * 8 bits, it is necessary to count in a per bit basis. If the result
   * is not a multiple of 8 bits, the LS bits are padded with zeros.
   * If an error occurred an appropriate negative error code is returned
   */
  virtual ssize_t
  encode_trunc(uint8_t *out, const uint8_t *in, size_t len) = 0;

  /**
   * This method should be called at the end of the encoding to
   * flush any outstanding bits.
   *
   * @param out the output buffer
   *
   * @return the number of encoded <b>bits</b>. Due to the fact that
   * some coding schemes may not produce an output that is a multiple of
   * 8 bits, it is necessary to count in a per bit basis. If the result
   * is not a multiple of 8 bits, the LS bits are padded with zeros.
   * If an error occurred an appropriate negative error code is returned.
   * Some encoding schemes, may not support flushing of outstanding bits,
   * therefore the default implementation of the method returns 0
   */
  virtual ssize_t
  finalize(uint8_t *out);

  /**
   * Resets the internal state of the encoder to the initial defaults
   *
   */
  virtual void
  reset() = 0;

  size_t
  max_frame_len() const;
protected:
  const size_t          d_max_frame_len;
private:

};

} // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_ENCODER_H */

