#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <encode/base58.h>

using namespace std;
int main(){
  
  vector<unsigned char> vch = {1,2,3,4,5,6,7,8};
  EncodeBase58Check(vch);
  
  cout << "EncodeBase58Check: " << EncodeBase58Check(vch) << endl;

  cout << "==============\n";

  return 0;
}
