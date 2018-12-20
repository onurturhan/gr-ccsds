/* -*- c++ -*- */
/*
 * gr-ccsds: CCSDS Telemetry and Telecommand Transceivers
 *
 *  Copyright (C) 2018
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
#include <ccsds/encoder.h>

namespace gr
{
namespace ccsds
{

/**
 * @brief Construct a new encoder::encoder object
 *
 * @param max_frame_len the maximum allowed input frame length
 */
encoder::encoder(size_t max_frame_len)
  : d_max_frame_len(max_frame_len)
{
}


encoder::~encoder()
{
}

/**
 *
 * @return the maximum allowed frame length
 */
size_t
encoder::max_frame_len() const
{
  return d_max_frame_len;
}

ssize_t
encoder::finalize(uint8_t *out)
{
  return 0;
}

} /* namespace ccsds */
} /* namespace gr */

