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
#include "ccsds/turbo_encoder.h"
#include "ccsds/pcm_encoder.h"
#include "ccsds/pcm_decoder.h"
#include "ccsds/ccsds_modulator.h"
#include "ccsds/ccsds_constellation.h"
#include "ccsds/ccsds_demodulator.h"
%}


%include "ccsds/encoder.h"
%include "ccsds/decoder.h"
%include "ccsds/conv_decoder.h"
%include "ccsds/conv_encoder.h"
%include "ccsds/rs_encoder.h"
%include "ccsds/rs_decoder.h"
%include "ccsds/turbo_encoder.h"
%include "ccsds/pcm_encoder.h"
%include "ccsds/pcm_decoder.h"
%include "ccsds/ccsds_modulator.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, ccsds_modulator);
%include "ccsds/ccsds_constellation.h"

%include "constellation.i"%include "ccsds/ccsds_demodulator.h"
GR_SWIG_BLOCK_MAGIC2(ccsds, ccsds_demodulator);
