/*
 * hash_pseudo_random.cpp
 *
 *  Created on: 2014.01.20.
 *      Author: mrita
 */

#include "pseudo_random.h"
#include "hash_pseudo_random.h"

HashPseudoRandom::HashPseudoRandom(int seed_){
  seed = seed_;
 
  a = 0x2058cc50L;
  b = 0xcb19137eL; 
  c = 0x2cb6b6fdL;
  d=  0x7e4c3b0fL;
  zz = 0xbea0107e5067d19dL;
  LLL = ((1L<<32)-1);
};

 
int HashPseudoRandom::get(short fp, short step, long node){
  long h0 = fp & LLL;
  long h1 = step & LLL;
  long h2 = node & LLL;
  long h3 = seed & LLL;

  return (int)(((a*h0 + b*h1 + c*h2 + d*h3 )*zz) >> 32);
};



