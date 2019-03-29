
#include <gnuradio/attributes.h>
#include <cppunit/TestAssert.h>
#include "qa_conv_decoder.h"
#include <ccsds/conv_encoder.h>
#include <ccsds/conv_decoder.h>
#include <cstring>

namespace gr
{
namespace ccsds
{

void
qa_conv_decoder::t1 ()
{
  decoder::decoder_sptr dec = conv_decoder::make(conv_decoder::RATE_1_2, 2048);
  encoder::encoder_sptr  conv = conv_encoder::make(conv_encoder::RATE_1_2,
                                                   2048);
  const char *tx = "ABCDEFGHIJKLMNOPQRSTUVWXYZIJKLMNOPQRSTUVWXYZ1234";
  uint8_t rx[255];
  uint8_t rx2[255];
  memset (rx, 0, 255);
  memset (rx2, 0, 255);

  ssize_t ret = conv->encode_trunc (rx, (uint8_t *)tx, strlen(tx) * 8);

  dec->decode(rx2, rx, strlen(tx) * 8 * 2 );
  for(uint32_t i = 0; i < strlen(tx) - 1; i++) {
    CPPUNIT_ASSERT(tx[i] == rx2[i]);
  }

}

void
qa_conv_decoder::t2 ()
{

}

} /* namespace ccsds */
} /* namespace gr */

