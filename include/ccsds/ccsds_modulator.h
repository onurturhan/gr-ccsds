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

#ifndef INCLUDED_CCSDS_CCSDS_MODULATOR_H
#define INCLUDED_CCSDS_CCSDS_MODULATOR_H

#include <ccsds/api.h>
#include <gnuradio/sync_block.h>
#include <ccsds/ccsds_constellation.h>

namespace gr {
namespace ccsds {

/*!
* \brief <+description of block+>
* \ingroup ccsds
*
*/
class CCSDS_API ccsds_modulator : virtual public gr::sync_block
{
public:
  typedef boost::shared_ptr<ccsds_modulator> sptr;

  /*!
  * \brief Return a shared_ptr to a new instance of ccsds::ccsds_modulator.
  *
  * To avoid accidental use of raw pointers, ccsds::ccsds_modulator's
  * constructor is in a private implementation
  * class. ccsds::ccsds_modulator::make is the public interface for
  * creating new instances.
  */

  static sptr make(ccsds::ccsds_constellation_sptr constellation, uint8_t pcm);
};

} // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CCSDS_MODULATOR_H */

