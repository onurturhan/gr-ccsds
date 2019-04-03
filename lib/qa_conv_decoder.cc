
#include <gnuradio/attributes.h>
#include <cppunit/TestAssert.h>
#include "qa_conv_decoder.h"
#include <ccsds/conv_encoder.h>
#include <ccsds/conv_decoder.h>
#include <cstring>
#include <random>

namespace gr
{
namespace ccsds
{

void
qa_conv_decoder::simple_decode ()
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  decoder::decoder_sptr dec = conv_decoder::make(conv_decoder::RATE_1_2, 2048);
  encoder::encoder_sptr  conv = conv_encoder::make(conv_encoder::RATE_1_2,
                                                   2048);
  uint8_t tx[255];
  uint8_t rx[255];
  uint8_t decoded[255];


  for(uint32_t i = 0; i < 48; i++) {
    tx[i] = uni(mt);
  }


  ssize_t ret = conv->encode (rx, tx, 48 * 8);

  dec->decode(decoded, rx, ret );
  for(uint32_t i = 0; i < 48; i++) {
    CPPUNIT_ASSERT(tx[i] == decoded[i]);
  }

}

void
qa_conv_decoder::test_decode (size_t msg_len)
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> randb(0, 255);
  decoder::decoder_sptr dec = conv_decoder::make(conv_decoder::RATE_1_2, 2048);
  encoder::encoder_sptr  conv = conv_encoder::make(conv_encoder::RATE_1_2,
                                                   2048);

  uint8_t *msg = new uint8_t[msg_len];
  uint8_t *tx = new uint8_t[msg_len * 2 + 4];
  uint8_t *decoded = new uint8_t[msg_len];

  for (uint32_t i = 0; i < msg_len; i++) {
    msg[i] = randb (mt);
  }

  ssize_t ret = conv->encode (tx, msg, msg_len * 8);
  dec->decode(decoded, tx, ret );
  for(uint32_t i = 0; i < msg_len; i++) {
    CPPUNIT_ASSERT(msg[i] == decoded[i]);
  }
  delete [] msg;
  delete [] tx;
  delete [] decoded;
}


void
qa_conv_decoder::decode_random_size ()
{
  for(uint32_t i = 4; i < 1024; i++) {
    test_decode(i);
  }
}

} /* namespace ccsds */
} /* namespace gr */

