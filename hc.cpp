#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::make_heap;
using std::map;
using std::ofstream;
using std::pop_heap;
using std::push_heap;
using std::string;
using std::vector;

struct tree_node{
  tree_node *left;
  tree_node *right;
  char character;
  int weight;
};

int compare_node(tree_node *i, tree_node *j){
  return (i->weight > j->weight);
}

struct huffman_bits{
  int value;
  int length;
};

string bitsToString(huffman_bits bits){
  int b = bits.value;
  string retval;
  for(int i = 0; i < bits.length; i++){
    if(b & 1 != 0){
      retval = "1" + retval;
    }else{
      retval = "0" + retval;
    }
    b >>= 1;
  }
  return retval;
}

void getBits(tree_node *root, huffman_bits soFar, map<char, huffman_bits> &values){
  if(root == NULL) return;
  if(root->character != '\0'){
    values[root->character] = soFar;
  }
  soFar.length++;
  soFar.value <<= 1;
  getBits(root->left, soFar, values);
  soFar.value |= 1;
  getBits(root->right, soFar, values);
}

int main(int argc, char **argv){
  if(argc < 2){
    cerr << "You must specify a file" << endl;
    return 1;
  }

  ifstream file;
  file.open(argv[1], ios::in);

  if(!file.is_open()){
    cerr << "File not found" << endl;
    return 1;
  }

  // Calculate Frequencies
  char current;
  map<char, int> counts;
  while(file.good()){
    current = file.get();
    if(file.good())
      if(counts.find(current) != counts.end())
        counts[current] += 1;
      else
        counts[current] = 1;
  }
  counts['\0'] = 0;

  vector<tree_node *> heap;
  for(map<char, int>::iterator i = counts.begin(); i != counts.end(); i++){
    tree_node *newTN = (tree_node *)malloc(sizeof(tree_node));
    newTN->left = NULL;
    newTN->right = NULL;
    newTN->character = i->first;
    newTN->weight = i->second;
    heap.push_back(newTN);
  }


  // Create huffman tree
  make_heap(heap.begin(), heap.end(), compare_node);
  
  while(heap.size() > 1){
    pop_heap(heap.begin(), heap.end(), compare_node);
    tree_node *first = heap.back();
    heap.pop_back();

    pop_heap(heap.begin(), heap.end(), compare_node);
    tree_node *second = heap.back();
    heap.pop_back();
    
    tree_node *newNode = (tree_node *)malloc(sizeof(tree_node));
    newNode->left = first;
    newNode->right = second;
    newNode->character = '\0';
    newNode->weight = first->weight + second->weight;
    heap.push_back(newNode);

    push_heap(heap.begin(), heap.end(), compare_node);
  }
  
  map<char, huffman_bits> codes;
  huffman_bits initial;
  initial.value = 0;
  initial.length = 0;

  getBits(heap[0], initial, codes);

  ofstream out;
  string filename = argv[1];
  filename += ".zzip";
  out.open(filename.c_str(), ios::out | ios::binary);
  
  file.close();
  file.open(argv[1], ios::in);
 
  out.write((char *)&codes['\0'].value, 4);
  for(map<char, huffman_bits>::iterator i = codes.begin(); i != codes.end(); i++){
    if(i->first != '\0'){
      out << i->first;
      int *v = &i->second.value;
      out.write((char *)v, 4);
    }
  }
  out << '\0';
  out.write((char *)&codes['\0'].value, 4);
  char currentData = 0;
  int currentLength = 0;
  while(file.good()){
    current = file.get();
    if(file.good()){
      huffman_bits these = codes[current];
      // cout << "These bits:" << bitsToString(these) << endl;
      if(currentLength + these.length <= sizeof(currentData) * 8){
        currentData <<= these.length;
        currentData |= these.value;
        currentLength += these.length;
      }else{
        int diff = sizeof(currentData) * 8 - currentLength;
        currentData <<= diff;
        int newData = these.value >> these.length - diff;
        currentData |= newData;
        out << currentData;
        newData = these.value;
        int shamt = (sizeof(currentData)*8) - these.length;
        currentData = (these.value << shamt) >> shamt;
        currentLength = these.length - diff;
      }

      if(currentLength == sizeof(currentData) * 8){
        out << currentData;
        currentLength = 0;
        currentData = 0;
      }
    }
  }
}
