#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <windows.h>
#include <thread/sync.h>

using namespace std;

CCriticalSection cs_numberSet; 
void add(set<int> &numberSet, int n){
  CRITICAL_BLOCK(cs_numberSet){
      cout << "add " << n << endl;
      numberSet.insert(n);
  }
}

int main(){
  
  set<int> numberSet;
  
  for(int i=0; i<10; i++){
    add(numberSet,i);
  }

  std::cout << "s = { ";
  for (int n : numberSet) {
      std::cout << n << ", ";
  }
  std::cout << "}; \n";

  cout << "==============\n";
  
  return 0;
}
