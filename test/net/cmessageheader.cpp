#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <net/cmessageheader.h>

using namespace std;

int main(){
  
  CMessageHeader msgHeader("PING",0);
  
  cout << msgHeader.GetCommand() << endl;

  cout << "==============";

  return 0;
}
