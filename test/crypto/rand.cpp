#include <iostream>
#include <crypto/rand.h>


using namespace std;
int main(){
  
  RandAddSeed();
  cout << "GetRand: " << GetRand(100) << endl;
  
  return 0;
}
