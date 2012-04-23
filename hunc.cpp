#include <iostream>
#include <fstream>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;

int main(int argc, char **argv){
  if(argc < 2){
    cerr << "You must specify a file" << endl;
    return 1;
  }

  ifstream file;
  file.open(argv[1], ios::in | ios::binary);
  
  int code;
  file.read((char *)&code, 4);

  while(true){
    char val;
    file >> val;
    file.read((char *)&code, 4);
    cout << val << ": " << code << endl;
  }
}
