/* -*- c++ -*- */

#define CCSDS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "ccsds_swig_doc.i"

%{
#include "ccsds/encoder.h"
#include "ccsds/decoder.h"
%}


%include "ccsds/encoder.h"
%include "ccsds/decoder.h"
