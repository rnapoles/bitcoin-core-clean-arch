#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <serialize.h>
#include <utils/log.h>
#include <utils/blockfile.h>

using namespace std;

int main(){
  
  // Open history file to read
  CAutoFile filein = OpenBlockFile(1, 1, "rb");
  if (!filein){
      return error("CBlock::ReadFromDisk() : OpenBlockFile failed");
  }

  cout << "==============\n";
  
  return 0;
}
