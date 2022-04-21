#include <iostream>
#include <vector>
#include <cstdint>
#include <crypto/hash.h>
#include <uint256.h>

using namespace std;
int main(){
  
  vector<uint8_t> vchIn;
  vchIn.push_back('A');
  vchIn.push_back('B');
  vchIn.push_back('C');
  vchIn.push_back('D');

  uint256 hash = Hash(vchIn.begin(), vchIn.end());

  cout << hash.GetHex() << endl;

  return 0;
}
