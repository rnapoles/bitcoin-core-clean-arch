#include <winsock.h>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <serialize.h>
#include <net/cinv.h>
#include <net/crequesttracker.h>
#include <net/cnode.h>

using namespace std;

int main(){
  
  CInv inv;
  CRequestTracker reqTracker;
  CNode node(INVALID_SOCKET, CAddress("127.0.0.1", 0));
  
  cout << "" << endl;

  cout << "==============\n";

  return 0;
}
