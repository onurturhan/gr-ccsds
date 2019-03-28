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

%template(ccsds_constellation_sptr) boost::shared_ptr<gr::ccsds::ccsds_constellation>;

%template(ccsds_constellation_bpsk_sptr) boost::shared_ptr<gr::ccsds::ccsds_constellation_bpsk>;
%pythoncode %{
ccsds_constellation_bpsk_sptr.__repr__ = lambda self: "<CCSDS constellation BPSK>"
ccsds_constellation_bpsk = ccsds_constellation_bpsk.make;
%}

%template(ccsds_constellation_qpsk_sptr) boost::shared_ptr<gr::ccsds::ccsds_constellation_qpsk>;
%pythoncode %{
ccsds_constellation_qpsk_sptr.__repr__ = lambda self: "<CCSDS constellation QPSK>"
ccsds_constellation_qpsk = ccsds_constellation_qpsk.make;
%}

%template(ccsds_constellation_oqpsk_sptr) boost::shared_ptr<gr::ccsds::ccsds_constellation_oqpsk>;
%pythoncode %{
ccsds_constellation_oqpsk_sptr.__repr__ = lambda self: "<CCSDS constellation OQPSK>"
ccsds_constellation_oqpsk = ccsds_constellation_oqpsk.make;
%}