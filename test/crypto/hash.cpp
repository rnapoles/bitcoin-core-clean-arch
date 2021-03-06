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

  uint256 hash256 = Hash(vchIn.begin(), vchIn.end());
  uint160 hash160 = Hash160(vchIn);

  cout << "hash256 " << hash256.GetHex() << endl;
  cout << "hash160 " << hash160.GetHex() << endl;

  cout << "==============\n";

  return 0;
}
