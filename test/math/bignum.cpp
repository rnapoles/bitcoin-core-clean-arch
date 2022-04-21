#include <math/bignum.h>
#include <cstdio>

int main(){
  
  CBigNum big1(123);
  CBigNum big2(123);
  CBigNum big3 = big1 + big2;
  
  printf("%d\n",big3.getint());

  cout << "==============\n";

  return 0;
}