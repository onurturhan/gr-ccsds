
#include <gnuradio/attributes.h>
#include <cppunit/TestAssert.h>
#include "qa_conv_decoder.h"
#include <ccsds/utils.h>
#include <ccsds/conv_encoder.h>
#include <ccsds/conv_decoder.h>
#include <cstring>
#include <random>

namespace gr
{
namespace ccsds
{

void
qa_conv_decoder::test_simple_decode_1_2 ()
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  decoder::decoder_sptr dec = conv_decoder::make(conv_decoder::RATE_1_2, 2048);
  encoder::encoder_sptr  conv = conv_encoder::make(conv_encoder::RATE_1_2,
                                                   2048);
  uint8_t raw_packed[255];
  uint8_t raw_unpacked[255 * 8];
  uint8_t coded_unpacked_hard[255 * 8];
  int8_t  coded_unpacked_soft[255 * 8];
  uint8_t decoded_unpacked[255 * 8];
  uint8_t decoded_packed[255];


  for(uint32_t i = 0; i < 48; i++) {
    raw_packed[i] = uni(mt);
  }
  packed_to_unpacked(raw_unpacked, raw_packed, 48);

  ssize_t ret = conv->encode (coded_unpacked_hard, raw_unpacked, 48 * 8);
  hard_to_soft_int8(coded_unpacked_soft, coded_unpacked_hard, ret);

  ret = dec->decode(decoded_unpacked, coded_unpacked_soft, ret );
  unpacked_to_packed(decoded_packed, decoded_unpacked, ret);
  for(uint32_t i = 0; i < 48; i++) {
    CPPUNIT_ASSERT(raw_packed[i] == decoded_packed[i]);
  }
}


void
qa_conv_decoder::test_simple_decode_2_3 ()
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> uni(0, 255);
  decoder::decoder_sptr dec = conv_decoder::make(conv_decoder::RATE_2_3, 2048);
  encoder::encoder_sptr  conv = conv_encoder::make(conv_encoder::RATE_2_3,
                                                   2048);

  uint8_t raw_packed[255];
  uint8_t raw_unpacked[255 * 8];
  uint8_t coded_unpacked_hard[255 * 8];
  int8_t  coded_unpacked_soft[255 * 8];
  uint8_t decoded_unpacked[255 * 8];
  uint8_t decoded_packed[255];


  for(uint32_t i = 0; i < 48; i++) {
    raw_packed[i] = uni(mt);
  }
  packed_to_unpacked(raw_unpacked, raw_packed, 48);

  ssize_t ret = conv->encode (coded_unpacked_hard, raw_unpacked, 48 * 8);
  hard_to_soft_int8(coded_unpacked_soft, coded_unpacked_hard, ret);

  ret = dec->decode(decoded_unpacked, coded_unpacked_soft, ret );
  unpacked_to_packed(decoded_packed, decoded_unpacked, ret);
  for(uint32_t i = 0; i < 48; i++) {
    CPPUNIT_ASSERT(raw_packed[i] == decoded_packed[i]);
  }
}


void
qa_conv_decoder::test_decode (size_t msg_len, conv_decoder::coding_rate_t rate)
{
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> randb(0, 255);
  decoder::decoder_sptr dec = conv_decoder::make(rate, (msg_len + 2)* 8 * 2);
  encoder::encoder_sptr conv = conv_encoder::make (
      (conv_encoder::coding_rate_t) rate, (msg_len + 2) * 8 * 2);

  uint8_t *raw_packed = new uint8_t[msg_len];
  uint8_t *raw_unpacked = new uint8_t[msg_len * 8];
  uint8_t *coded_unpacked_hard = new uint8_t[(msg_len + 2) * 8 * 2];
  int8_t  *coded_unpacked_soft = new int8_t[(msg_len + 2) * 8 * 2];
  uint8_t *decoded_unpacked = new uint8_t[msg_len * 8];
  uint8_t *decoded_packed = new uint8_t[msg_len];


  for(uint32_t i = 0; i < msg_len; i++) {
    raw_packed[i] = randb (mt);
  }
  packed_to_unpacked(raw_unpacked, raw_packed, msg_len);

  ssize_t ret = conv->encode (coded_unpacked_hard, raw_unpacked, msg_len * 8);
  hard_to_soft_int8(coded_unpacked_soft, coded_unpacked_hard, ret);

  ret = dec->decode(decoded_unpacked, coded_unpacked_soft, ret );
  unpacked_to_packed(decoded_packed, decoded_unpacked, ret);

  for(uint32_t i = 0; i < msg_len; i++) {
    CPPUNIT_ASSERT(raw_packed[i] == decoded_packed[i]);
  }

  delete [] raw_packed;
  delete [] raw_unpacked;
  delete [] coded_unpacked_hard;
  delete [] coded_unpacked_soft;
  delete [] decoded_unpacked;
  delete [] decoded_packed;
}


void
qa_conv_decoder::test_various_msg_len_1_2 ()
{
  for(uint32_t i = 4; i < 1024; i++) {
    test_decode(i, conv_decoder::RATE_1_2);
  }
}

void
qa_conv_decoder::test_various_msg_len_2_3 ()
{
  for(uint32_t i = 4; i < 1024; i++) {
    test_decode(i, conv_decoder::RATE_2_3);
  }
}

} /* namespace ccsds */
} /* namespace gr */

