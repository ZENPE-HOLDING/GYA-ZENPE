/**
 *  @file
 *  @copyright defined in gya/LICENSE.txt
 */
#pragma once

#include <gyaiolib/system.h>

extern "C" {
  /**
   *  @defgroup compiler builtins API
   *  @brief Declares int128 helper builtins generated by the toolchain. 
   *  @ingroup compilerbuiltinsapi
   *
   *  @{
   */

 /**
  * Multiply two 128 bit integers split as two unsigned 64 bit integers and assign the value to the first parameter.
  * @brief Multiply two 128 unsigned bit integers (which are represented as two 64 bit unsigned integers. 
  * @param res  It will be replaced with the result product.
  * @param la   Low 64 bits of the first 128 bit factor.
  * @param ha   High 64 bits of the first 128 bit factor.
  * @param lb   Low 64 bits of the second 128 bit factor.
  * @param hb   High 64 bits of the second 128 bit factor.
  * Example:
  * @code
  * __int128 res = 0;
  * __int128 a = 100;
  * __int128 b = 100;
  * __multi3(res, a, (a >> 64), b, (b >> 64));
  * printi128(res); // Output: 10000
  * @endcode
  */
  void __multi3(__int128& res, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb);

  void __divti3(__int128& res, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb);
  void __udivti3(unsigned __int128& res, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb);

  void __modti3(__int128& res, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb);
  void __umodti3(unsigned __int128& res, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb);

  void __lshlti3(__int128& res, uint64_t lo, uint64_t hi, uint32_t shift);
  void __lshrti3(__int128& res, uint64_t lo, uint64_t hi, uint32_t shift);

  void __ashlti3(__int128& res, uint64_t lo, uint64_t hi, uint32_t shift);
  void __ashrti3(__int128& res, uint64_t lo, uint64_t hi, uint32_t shift);

  void __addtf3( long double& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb );
  void __subtf3( long double& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ); 
  void __multf3( long double& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ); 
  void __divtf3( long double& ret, uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ); 
  int __eqtf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ); 
  int __netf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ); 
  int __getf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ); 
  int __gttf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ); 
  int __letf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ); 
  int __lttf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ); 
  int __cmptf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ); 
  int __unordtf2( uint64_t la, uint64_t ha, uint64_t lb, uint64_t hb ); 
  void __extendsftf2( long double& ret, float f ); 
  void __extenddftf2( long double& ret, double f ); 
  int64_t __fixtfdi( uint64_t l, uint64_t h ); 
  int32_t __fixtfsi( uint64_t l, uint64_t h ); 
  uint64_t __fixunstfdi( uint64_t l, uint64_t h ); 
  uint32_t __fixunstfsi( uint64_t l, uint64_t h ); 
  double __trunctfdf2( uint64_t l, uint64_t h ); 
  float __trunctfsf2( uint64_t l, uint64_t h ); 

  void __break_point();

} // extern "C"
