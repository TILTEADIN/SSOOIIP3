#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

int main() {
  // Create and open a text file
  char buffer [50];
  int n, a=1;
  n=sprintf (buffer, "./results/%d", a);
  printf ("[%s] is a string %d chars long\n",buffer,n);

  
  std::ofstream file(buffer);
  cout<<file.good();
  file << "????" << std::endl;
  // Close the file
  file.close();
}