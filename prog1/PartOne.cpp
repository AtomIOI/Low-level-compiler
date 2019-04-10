#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
using namespace std;

int main(int argc, char const *argv[]) {
  bool quiet = false; bool squish = false; bool censor = false; bool stats = false; bool length = false;
  ifstream inFile;
  int fileNum = 0;

  if(argc < 2){
    return 0;
  }
  //check all flags
  for (int i = 1; i < argc; i++) {
    if(argv[i][0] == '-' ){
      if (strlen(argv[i]) != 2 ) {
        cerr << argv[i] << " INVALID FLAG" << '\n';
        return -1;
      }
      switch (argv[i][1]) {
        case 'q':
          quiet = true;
          break;
        case 's':
          squish = true;
          break;
        case 'c':
          censor = true;
          break;
        case 'p':
          stats = true;
          break;
        case 'l':
          length = true;
          break;
        default:
          cerr << argv[i] << " INVALID FLAG" << '\n';
          return -1;
      }
    }
    else if(fileNum == 0){
      fileNum++;
      inFile.open(argv[i]);

      if (inFile.is_open() == false) {
        cerr << argv[i] << " CANNOT OPEN" << '\n';
        return -1;
      }
    }
    else{
      cerr << "TOO MANY FILES" << '\n';
      return -1;
    }
  }
  // check for conflicting flags after command arguments have been parsed
  if(censor == true || squish == true){
    cout << "CONFLICTING FLAGS" << '\n';
    return 0;
  }

  if (quiet) {
    /* code */
  }

  if (stats) {
    /* code */
  }

  if (length) {
    /* code */
  }

  inFile.close();
  return 0;
}
