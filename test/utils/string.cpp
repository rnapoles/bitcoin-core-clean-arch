#include <iostream>
#include <vector>
#include <utils/string.h>

using namespace std;
int main(){

  vector<string> vWords;
  string strLine = "1 2 3";
  ParseString(strLine, ' ', vWords);

  for (auto x: vWords) {
    cout << x << ' ' << endl;
  }

  vector<unsigned char> vch = {1,2,3,4,5,6};
  cout << HexNumStr(vch.begin(), vch.end()) << endl;
  
  return 0;
}
