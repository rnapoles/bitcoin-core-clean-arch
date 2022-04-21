#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <utils/address.h>
#include <crypto/hash.h>

using namespace std;

int main(){
  
  vector<uint8_t> vchIn;
  vchIn.push_back('A');
  vchIn.push_back('B');
  vchIn.push_back('C');
  vchIn.push_back('D');
  
  uint160 hash160 = Hash160(vchIn);

  cout << "hash160:          " << hash160.GetHex() << endl;
  cout << "Hash160ToAddress: " << Hash160ToAddress(hash160) << endl;

  cout << "==============\n";

  return 0;
}
