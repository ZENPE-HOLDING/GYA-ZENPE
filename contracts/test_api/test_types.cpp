/**
 *  @file
 *  @copyright defined in gya/LICENSE.txt
 */
#include <gyaiolib/gyaio.hpp>

#include "test_api.hpp"

void test_types::types_size() {

   gyaio_assert( sizeof(int64_t) == 8, "int64_t size != 8");
   gyaio_assert( sizeof(uint64_t) ==  8, "uint64_t size != 8");
   gyaio_assert( sizeof(uint32_t) ==  4, "uint32_t size != 4");
   gyaio_assert( sizeof(int32_t) ==  4, "int32_t size != 4");
   gyaio_assert( sizeof(uint128_t) == 16, "uint128_t size != 16");
   gyaio_assert( sizeof(int128_t) == 16, "int128_t size != 16");
   gyaio_assert( sizeof(uint8_t) ==  1, "uint8_t size != 1");

   gyaio_assert( sizeof(account_name) ==  8, "account_name size !=  8");
   gyaio_assert( sizeof(table_name) ==  8, "table_name size !=  8");
   gyaio_assert( sizeof(time) ==  4, "time size !=  4");
   gyaio_assert( sizeof(gyaio::key256) == 32, "key256 size != 32" );
}

void test_types::char_to_symbol() {

   gyaio_assert( gyaio::char_to_symbol('1') ==  1, "gyaio::char_to_symbol('1') !=  1");
   gyaio_assert( gyaio::char_to_symbol('2') ==  2, "gyaio::char_to_symbol('2') !=  2");
   gyaio_assert( gyaio::char_to_symbol('3') ==  3, "gyaio::char_to_symbol('3') !=  3");
   gyaio_assert( gyaio::char_to_symbol('4') ==  4, "gyaio::char_to_symbol('4') !=  4");
   gyaio_assert( gyaio::char_to_symbol('5') ==  5, "gyaio::char_to_symbol('5') !=  5");
   gyaio_assert( gyaio::char_to_symbol('a') ==  6, "gyaio::char_to_symbol('a') !=  6");
   gyaio_assert( gyaio::char_to_symbol('b') ==  7, "gyaio::char_to_symbol('b') !=  7");
   gyaio_assert( gyaio::char_to_symbol('c') ==  8, "gyaio::char_to_symbol('c') !=  8");
   gyaio_assert( gyaio::char_to_symbol('d') ==  9, "gyaio::char_to_symbol('d') !=  9");
   gyaio_assert( gyaio::char_to_symbol('e') == 10, "gyaio::char_to_symbol('e') != 10");
   gyaio_assert( gyaio::char_to_symbol('f') == 11, "gyaio::char_to_symbol('f') != 11");
   gyaio_assert( gyaio::char_to_symbol('g') == 12, "gyaio::char_to_symbol('g') != 12");
   gyaio_assert( gyaio::char_to_symbol('h') == 13, "gyaio::char_to_symbol('h') != 13");
   gyaio_assert( gyaio::char_to_symbol('i') == 14, "gyaio::char_to_symbol('i') != 14");
   gyaio_assert( gyaio::char_to_symbol('j') == 15, "gyaio::char_to_symbol('j') != 15");
   gyaio_assert( gyaio::char_to_symbol('k') == 16, "gyaio::char_to_symbol('k') != 16");
   gyaio_assert( gyaio::char_to_symbol('l') == 17, "gyaio::char_to_symbol('l') != 17");
   gyaio_assert( gyaio::char_to_symbol('m') == 18, "gyaio::char_to_symbol('m') != 18");
   gyaio_assert( gyaio::char_to_symbol('n') == 19, "gyaio::char_to_symbol('n') != 19");
   gyaio_assert( gyaio::char_to_symbol('o') == 20, "gyaio::char_to_symbol('o') != 20");
   gyaio_assert( gyaio::char_to_symbol('p') == 21, "gyaio::char_to_symbol('p') != 21");
   gyaio_assert( gyaio::char_to_symbol('q') == 22, "gyaio::char_to_symbol('q') != 22");
   gyaio_assert( gyaio::char_to_symbol('r') == 23, "gyaio::char_to_symbol('r') != 23");
   gyaio_assert( gyaio::char_to_symbol('s') == 24, "gyaio::char_to_symbol('s') != 24");
   gyaio_assert( gyaio::char_to_symbol('t') == 25, "gyaio::char_to_symbol('t') != 25");
   gyaio_assert( gyaio::char_to_symbol('u') == 26, "gyaio::char_to_symbol('u') != 26");
   gyaio_assert( gyaio::char_to_symbol('v') == 27, "gyaio::char_to_symbol('v') != 27");
   gyaio_assert( gyaio::char_to_symbol('w') == 28, "gyaio::char_to_symbol('w') != 28");
   gyaio_assert( gyaio::char_to_symbol('x') == 29, "gyaio::char_to_symbol('x') != 29");
   gyaio_assert( gyaio::char_to_symbol('y') == 30, "gyaio::char_to_symbol('y') != 30");
   gyaio_assert( gyaio::char_to_symbol('z') == 31, "gyaio::char_to_symbol('z') != 31");

   for(unsigned char i = 0; i<255; i++) {
      if((i >= 'a' && i <= 'z') || (i >= '1' || i <= '5')) continue;
      gyaio_assert( gyaio::char_to_symbol((char)i) == 0, "gyaio::char_to_symbol() != 0");
   }
}

void test_types::string_to_name() {

   gyaio_assert( gyaio::string_to_name("a") == N(a) , "gyaio::string_to_name(a)" );
   gyaio_assert( gyaio::string_to_name("ba") == N(ba) , "gyaio::string_to_name(ba)" );
   gyaio_assert( gyaio::string_to_name("cba") == N(cba) , "gyaio::string_to_name(cba)" );
   gyaio_assert( gyaio::string_to_name("dcba") == N(dcba) , "gyaio::string_to_name(dcba)" );
   gyaio_assert( gyaio::string_to_name("edcba") == N(edcba) , "gyaio::string_to_name(edcba)" );
   gyaio_assert( gyaio::string_to_name("fedcba") == N(fedcba) , "gyaio::string_to_name(fedcba)" );
   gyaio_assert( gyaio::string_to_name("gfedcba") == N(gfedcba) , "gyaio::string_to_name(gfedcba)" );
   gyaio_assert( gyaio::string_to_name("hgfedcba") == N(hgfedcba) , "gyaio::string_to_name(hgfedcba)" );
   gyaio_assert( gyaio::string_to_name("ihgfedcba") == N(ihgfedcba) , "gyaio::string_to_name(ihgfedcba)" );
   gyaio_assert( gyaio::string_to_name("jihgfedcba") == N(jihgfedcba) , "gyaio::string_to_name(jihgfedcba)" );
   gyaio_assert( gyaio::string_to_name("kjihgfedcba") == N(kjihgfedcba) , "gyaio::string_to_name(kjihgfedcba)" );
   gyaio_assert( gyaio::string_to_name("lkjihgfedcba") == N(lkjihgfedcba) , "gyaio::string_to_name(lkjihgfedcba)" );
   gyaio_assert( gyaio::string_to_name("mlkjihgfedcba") == N(mlkjihgfedcba) , "gyaio::string_to_name(mlkjihgfedcba)" );
   gyaio_assert( gyaio::string_to_name("mlkjihgfedcba1") == N(mlkjihgfedcba2) , "gyaio::string_to_name(mlkjihgfedcba2)" );
   gyaio_assert( gyaio::string_to_name("mlkjihgfedcba55") == N(mlkjihgfedcba14) , "gyaio::string_to_name(mlkjihgfedcba14)" );

   gyaio_assert( gyaio::string_to_name("azAA34") == N(azBB34) , "gyaio::string_to_name N(azBB34)" );
   gyaio_assert( gyaio::string_to_name("AZaz12Bc34") == N(AZaz12Bc34) , "gyaio::string_to_name AZaz12Bc34" );
   gyaio_assert( gyaio::string_to_name("AAAAAAAAAAAAAAA") == gyaio::string_to_name("BBBBBBBBBBBBBDDDDDFFFGG") , "gyaio::string_to_name BBBBBBBBBBBBBDDDDDFFFGG" );
}

void test_types::name_class() {

   gyaio_assert( gyaio::name{gyaio::string_to_name("azAA34")}.value == N(azAA34), "gyaio::name != N(azAA34)" );
   gyaio_assert( gyaio::name{gyaio::string_to_name("AABBCC")}.value == 0, "gyaio::name != N(0)" );
   gyaio_assert( gyaio::name{gyaio::string_to_name("AA11")}.value == N(AA11), "gyaio::name != N(AA11)" );
   gyaio_assert( gyaio::name{gyaio::string_to_name("11AA")}.value == N(11), "gyaio::name != N(11)" );
   gyaio_assert( gyaio::name{gyaio::string_to_name("22BBCCXXAA")}.value == N(22), "gyaio::name != N(22)" );
   gyaio_assert( gyaio::name{gyaio::string_to_name("AAAbbcccdd")} == gyaio::name{gyaio::string_to_name("AAAbbcccdd")}, "gyaio::name == gyaio::name" );

   uint64_t tmp = gyaio::name{gyaio::string_to_name("11bbcccdd")};
   gyaio_assert(N(11bbcccdd) == tmp, "N(11bbcccdd) == tmp");
}
