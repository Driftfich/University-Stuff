#include "stringg.h"
#include <iostream>

using namespace std;


int main()
{
  STRING M1="Max";
  STRING M2("Moritz");
  STRING M3;
  M3=M1+" und "+M2;
  cout << "Wilhelm Busch:\n"<< M3<<"\n";
  M1="______________";
  M1[0]='+';
  M1[M1.GetLength()-1]=M1[0];
  cout << M1<<'\n';
  return 0;
}