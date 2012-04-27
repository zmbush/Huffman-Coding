#include <iostream>
#include <fstream>
#include <map>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::map;
using std::pair;

int main(int argc, char **argv){
  if(argc < 2){
    cerr << "You must specify a file" << endl;
    return 1;
  }

  ifstream file;
  file.open(argv[1], ios::in | ios::binary);

  file.seekg(0, ios::end);
  int length = file.tellg();
  file.seekg(0, ios::beg);
  
  char *data = new char[length];

  file.read(data, length);

  int dataStart = 0;

  map<int, char> lookup;
  char value;
  int code;
  for(int i = 0; i < length;){
    value = data[i];
    i += 1;
    code = *((int *)(data+i));
    i += 4;
    lookup[code] = value;
    cout << value << ": " << code << endl;
    if(value == '\0'){
      dataStart = i;
      break;
    }
  }

  for(map<int, char>::iterator i = lookup.begin(); i != lookup.end(); i++){
    cout << i->first << ": " << i->second << endl;
  }

  int selected = 0;
  for(int i = dataStart; i < length; i++){
    char current = data[i];
    for(int b = 7; b >= 0; b--){
      selected <<= 1;
      int bit = ((current >> b) & 1);
      // cout << "Bit: " << bit << endl;
      selected |= bit;
      // cout << "selected: " << selected << endl;
      if(lookup.find(selected) != lookup.end()){
        // cout << "Found character for selection: " << selected << endl;
        cout << lookup[selected];
        selected = 0;
      }
    }
  }
}
