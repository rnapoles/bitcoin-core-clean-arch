#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <net/net.h>
#include <net/caddress.h>

using namespace std;

int main(){
  
  CAddress addr;
  cout << addr.ToString() << endl;

  cout << "==============\n";

  return 0;
}
