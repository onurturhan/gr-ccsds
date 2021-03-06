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
#include <ccsds/decoder.h>

namespace gr
{
namespace ccsds
{

int decoder::base_unique_id = 1;

decoder::decoder(size_t max_frame_len)
  : d_max_frame_len(max_frame_len),
    d_id(base_unique_id++)
{
}

decoder::~decoder()
{
}

/**
 * Unique ID of the decoder object. Internally used to declare variables
 * in GRC
 * @return the unique ID of the decoder
 */
int
decoder::unique_id ()
{
  return d_id;

}

/**
 *
 * @return the maximum allowed frame length
 */
size_t
decoder::max_frame_len() const
{
  return d_max_frame_len;
}

ssize_t
decoder::finalize(uint8_t *out)
{
  return 0;
}

} /* namespace ccsds */
} /* namespace gr */
