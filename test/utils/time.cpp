#include <iostream>
#include <utils/time.h>

using namespace std;
int main(){
  
  cout << "GetTime() " << GetTime() << endl;
  cout << "1- " << "GetAdjustedTime() " << GetAdjustedTime() << endl;
  AddTimeData(1,1);
  cout << "2- " << "GetAdjustedTime() " << GetAdjustedTime() << endl;
  AddTimeData(2,2);
  cout << "3- " << "GetAdjustedTime() " << GetAdjustedTime() << endl;
  AddTimeData(3,3);
  cout << "4- " << "GetAdjustedTime() " << GetAdjustedTime() << endl;
  AddTimeData(4,4);
  cout << "5- " << "GetAdjustedTime() " << GetAdjustedTime() << endl;
  
  cout << "==============\n";

  return 0;
}
