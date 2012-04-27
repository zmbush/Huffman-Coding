#include <iostream>
#include <fstream>
#include <map>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::map;
using std::pair;
using std::string;
using std::ofstream;

string toBits(char c){
  string retval;
  for(int i = 0; i < 8*sizeof(c); i++){
    if(c & 1){
      retval = "1" + retval;
    }else{
      retval = "0" + retval;
    }
    c >>= 1;
  }
  return retval;
}

string toBits(int c){
  string retval;
  for(int i = 0; i < 8*sizeof(c); i++){
    if(c & 1){
      retval = "1" + retval;
    }else{
      retval = "0" + retval;
    }
    c >>= 1;
  }
  return retval;
}

int main(int argc, char **argv){
  if(argc < 2){
    cerr << "You must specify a file" << endl;
    return 1;
  }

  ifstream file;
  file.open(argv[1], ios::in | ios::binary);

  ofstream output;
  bool so = true;
  if(argc > 2){
    so = false;
    output.open(argv[2], ios::out);
  }

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
    // cout << value << ": " << toBits(code) << endl;
    if(value == '\0'){
      dataStart = i;
      break;
    }
  }

  for(map<int, char>::iterator i = lookup.begin(); i != lookup.end(); i++){
    // cout << i->first << ": " << i->second << endl;
  }

  int selected = 0;
  for(int i = dataStart; i < length; i += 4){
    int current = *((int *)(data+i));
    // cout << "Got: " << toBits(current) << endl;
    for(int b = (sizeof(current)*8 - 1); b >= 0; b--){
      selected <<= 1;
      int bit = ((current >> b) & 1);
      // cout << "Bit: " << bit << endl;
      selected |= bit;
      // cout << "selected: " << selected << endl;
      if(lookup.find(selected) != lookup.end()){
        // cout << "Found character for selection: " << selected << endl;
        if(so)
          cout << lookup[selected];
        else
          output << lookup[selected];
        selected = 0;
      }
    }
  }
}
