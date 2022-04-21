#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <crypto/key.h>
#include <utils/address.h>
#include <utils/string.h>

using namespace std;

int main(){
  
  CKey pki;
  pki.MakeNewKey();
  CPrivKey priv = pki.GetPrivKey();
  vector<unsigned char> pub = pki.GetPubKey();

  cout << "PrivKey:\n " << HexNumStr(priv.begin(), priv.end()) << endl;
  cout << "PublKey:\n " << HexNumStr(pub.begin(), pub.end()) << endl;
  cout << "Address:\n " << PubKeyToAddress(pub) << endl;

  cout << "==============\n";

  return 0;
}
