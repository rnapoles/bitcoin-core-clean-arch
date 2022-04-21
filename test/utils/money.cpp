#include <iostream>
#include <types.h>
#include <utils/money.h>

using namespace std;
int main(){
  
  int64 n = 10000000000;
  
  cout << n << " FormatMoney " << FormatMoney(n) << endl;
  cout << n << " FormatMoney " << FormatMoney(n,true) << endl;
  
  int64 n1 = 0;
  int64 n2 = 0;

  ParseMoney("100.00",n1);
  ParseMoney("200.00",n2);

  cout << "ParseMoney('100.00',n1) " << n1 << endl;
  cout << "ParseMoney('200.00',n1) " << n2 << endl;


  return 0;
}
