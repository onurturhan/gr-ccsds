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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <ccsds/ccsds_constellation.h>

namespace gr {
namespace ccsds {

#define SQRT_TWO 0.707107

ccsds_constellation::ccsds_constellation(modulation_t mod_type)
{
  d_modulation_type = mod_type;
}

ccsds_constellation::~ccsds_constellation()
{
}

ccsds_constellation_bpsk::sptr
ccsds_constellation_bpsk::make()
{
  return ccsds_constellation_bpsk::sptr(new ccsds_constellation_bpsk());
}


ccsds_constellation_bpsk::ccsds_constellation_bpsk()
:ccsds_constellation(BPSK)
{
  d_constellation_pts.resize(2);
  d_constellation_pts[0] = gr_complex(-1, 0);
  d_constellation_pts[1] = gr_complex(1, 0);
}

ccsds_constellation_bpsk::~ccsds_constellation_bpsk()
{
}

ccsds_constellation_qpsk::sptr
ccsds_constellation_qpsk::make()
{
  return ccsds_constellation_qpsk::sptr(new ccsds_constellation_qpsk());
}
/**
 * Big Endian notation:
       10 | 00
       -------
       11 | 01
 */
ccsds_constellation_qpsk::ccsds_constellation_qpsk()
:ccsds_constellation(QPSK)
{
  d_constellation_pts.resize(4);
  d_constellation_pts[0] = gr_complex(SQRT_TWO, SQRT_TWO);
  d_constellation_pts[1] = gr_complex(SQRT_TWO, -SQRT_TWO);
  d_constellation_pts[2] = gr_complex(-SQRT_TWO, SQRT_TWO);
  d_constellation_pts[3] = gr_complex(-SQRT_TWO, -SQRT_TWO);
}

ccsds_constellation_qpsk::~ccsds_constellation_qpsk()
{
}

ccsds_constellation_oqpsk::sptr
ccsds_constellation_oqpsk::make()
{
  return ccsds_constellation_oqpsk::sptr(new ccsds_constellation_oqpsk());
}
/**
 * Big Endian notation:
       10 | 00
       -------
       11 | 01
 */
ccsds_constellation_oqpsk::ccsds_constellation_oqpsk()
:ccsds_constellation(OQPSK)
{
  d_constellation_pts.resize(4);
  d_constellation_pts[0] = gr_complex(SQRT_TWO, SQRT_TWO);
  d_constellation_pts[1] = gr_complex(SQRT_TWO, -SQRT_TWO);
  d_constellation_pts[2] = gr_complex(-SQRT_TWO, SQRT_TWO);
  d_constellation_pts[3] = gr_complex(-SQRT_TWO, -SQRT_TWO);
}

ccsds_constellation_oqpsk::~ccsds_constellation_oqpsk()
{
}

} /* namespace ccsds */
} /* namespace gr */

