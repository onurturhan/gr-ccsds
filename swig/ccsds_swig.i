/* -*- c++ -*- */

#define CCSDS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "ccsds_swig_doc.i"

%nodefaultctor gr::ccsds::encoder;
%template(encoder_sptr) boost::shared_ptr<gr::ccsds::encoder>;

%nodefaultctor gr::ccsds::decoder;
%template(decoder_sptr) boost::shared_ptr<gr::ccsds::decoder>;

%{
#include "ccsds/encoder.h"
#include "ccsds/decoder.h"
#include "ccsds/conv_decoder.h"
#include "ccsds/conv_encoder.h"
#include "ccsds/rs_encoder.h"
#include "ccsds/rs_decoder.h"
%}


%include "ccsds/encoder.h"
%include "ccsds/decoder.h"
%include "ccsds/conv_decoder.h"
%include "ccsds/conv_encoder.h"
%include "ccsds/rs_encoder.h"
%include "ccsds/rs_decoder.h"
