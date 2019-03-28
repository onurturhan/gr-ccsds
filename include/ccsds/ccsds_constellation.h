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

#ifndef INCLUDED_CCSDS_CCSDS_CONSTELLATION_H
#define INCLUDED_CCSDS_CCSDS_CONSTELLATION_H

#include <ccsds/api.h>
#include <boost/enable_shared_from_this.hpp>
#include <gnuradio/block.h>
#include <vector>

namespace gr {
namespace ccsds {

/*!
* \brief <+description+>
*
*/

class ccsds_constellation;
typedef boost::shared_ptr<ccsds_constellation> ccsds_constellation_sptr;
typedef enum{
  BPSK = 0,
  QPSK,
  OQPSK,
  GMSK
}modulation_t;

class CCSDS_API ccsds_constellation
: public boost::enable_shared_from_this<ccsds_constellation>
{
public:
  ccsds_constellation(modulation_t mod_type);
  ~ccsds_constellation();

  ccsds_constellation_sptr base()
  {
    return shared_from_this();
  }
  modulation_t
  get_mod_type(){
    return d_modulation_type;
  }

  std::vector<gr_complex>
  get_const_pts(){
    return d_constellation_pts;
  }

protected:
  std::vector<gr_complex> d_constellation_pts;
  modulation_t d_modulation_type;

};

class CCSDS_API ccsds_constellation_bpsk : public ccsds_constellation
{
public:
  typedef boost::shared_ptr<ccsds_constellation_bpsk> sptr;

  // public constructor
  static sptr make();

  ~ccsds_constellation_bpsk();


protected:
  ccsds_constellation_bpsk();
};

class CCSDS_API ccsds_constellation_qpsk : public ccsds_constellation
{
public:
  typedef boost::shared_ptr<ccsds_constellation_qpsk> sptr;

  // public constructor
  static sptr make();

  ~ccsds_constellation_qpsk();


protected:
  ccsds_constellation_qpsk();
};

class CCSDS_API ccsds_constellation_oqpsk : public ccsds_constellation
{
public:
  typedef boost::shared_ptr<ccsds_constellation_oqpsk> sptr;

  // public constructor
  static sptr make();

  ~ccsds_constellation_oqpsk();


protected:
  ccsds_constellation_oqpsk();
};
} // namespace ccsds
} // namespace gr

#endif /* INCLUDED_CCSDS_CCSDS_CONSTELLATION_H */

