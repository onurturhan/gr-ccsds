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

#ifndef INCLUDED_CCSDS_API_H
#define INCLUDED_CCSDS_API_H

#include <gnuradio/attributes.h>

#ifdef gnuradio_ccsds_EXPORTS
#  define CCSDS_API __GR_ATTR_EXPORT
#else
#  define CCSDS_API __GR_ATTR_IMPORT
#endif

  typedef enum{
    NRZ_L = 0,
    NRZ_M,
    NRZ_S,
    BPL,
    UNCODED
  }pcm_type_t;

#endif /* INCLUDED_CCSDS_API_H */
