#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
using namespace std;

int main(int argc, char const *argv[]) {
  bool quiet = false; bool squish = false; bool censor = false; bool stats = false; bool length = false;
  int fileNum = 0;
  char charIn;
  string itrString;
  ifstream inFile;
  ostringstream outStream;
  ostringstream fileStream;
  stringstream wordStream;

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
      string fileName = argv[i];
      inFile.open(fileName);
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
  // check for conflicting flags after all flags have been set
  if(quiet == true){
    if (censor == true || squish == true ) {
      cout << "CONFLICTING FLAGS" << '\n';
      return -1;
    }
  }

  if (quiet) {
    //no output
    return 0;
  }else{
    //read in the file one char at a time and preform neccesary actions
    if(squish){
      while (inFile.get(charIn)) {
        fileStream << charIn;
      }
      itrString = fileStream.str();
      for(size_t i = 0; i < itrString.length(); i++) {
        if(isspace(itrString[i]) && !isspace(itrString[i+1])){
          outStream << itrString[i];
        }else if(!isspace(itrString[i])){
          outStream << itrString[i];
        }
      }
    }
    if(censor){
      if(squish){
        itrString = outStream.str();
        outStream.str("");
      }else{
        while (inFile.get(charIn)) {
          fileStream << charIn;
        }
        itrString = fileStream.str();
      }

      for(size_t i = 0; i < itrString.length(); i++) {
        if(isalpha(itrString[i])) {
            wordStream << itrString[i];
        }else if(isspace(itrString[i])){
          outStream << wordStream.str();
          outStream << itrString[i];
          wordStream.str("");
        }else{
          while(!isspace(itrString[i]) ){
            i++;
          }
          while (!isalpha(itrString[i+1])) {
            i++;
          }
          wordStream.str("");
        }
      }
    }
    if(squish || censor){
      string outString = outStream.str();
      cout << outString;
    }else {
      while (inFile.get(charIn)) {
        cout << charIn;
      }
    }
  }

  if(stats || length);
  inFile.close();
  return 0;
}
