#include <math/bignum.h>
#include <cstdio>

int main(){
  
  CBigNum big1(123);
  CBigNum big2(123);
  CBigNum big3 = big1 + big2;
  
  printf("%d",big3.getint());
  
  return 0;
}