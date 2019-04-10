#include "Lex.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <map>
using namespace std;

Token tok;
istream *in;
ifstream inFile;
bool verbose = false; bool mci = false; bool sum = false;
int totalTokens = 0; int totalStrings = 0;
int fileNum = 0;
int lineNumber;
int lexNum = 0;
map <string, int> mciMap;
map <int, string> longestWordLen;


int main(int argc, char const *argv[]) {
  //iterate through command line arguments
  for (int i = 1; i < argc; i++) {
    if(argv[i][0] == '-' ){
      switch (argv[i][1]) {
        case 'v':
          verbose = true;
          break;
        case 'm':
          mci = true;
          break;
        case 's':
          sum = true;
          break;
        default:
          cerr << "INVALID FLAG " << argv[i] << '\n';
          return -1;
      }
    }
    else if(fileNum == 0){
      fileNum++;
      string fileName = argv[i];
      inFile.open(fileName);
      if (inFile.is_open() == false) {
        cerr << "UNABLE TO OPEN " << argv[i] << '\n';
        return -1;
      }
    }
    else{
      cerr << "TOO MANY FILE NAMES" << '\n';
      return -1;
    }
  }

  if(fileNum > 0){
    in = &inFile;
  }else{
    in = &cin;
  }

  while( (tok = getNextToken(in, &lineNumber)) != DONE && tok != ERR ){
    if(verbose){
      cout << tok;
    }
    if(mci && tok == IDENT){
      string lex = tok.GetLexeme();
      mciMap[lex]++;
      if(mciMap[lex] > lexNum) lexNum = mciMap[lex];
    }
    if(sum){
      totalTokens++;
      if(tok == SCONST){
        totalStrings++;
        longestWordLen[(tok.GetLexeme()).length()] = tok.GetLexeme();
      }
    }
  }
  //handle errors
  if(tok == ERR){
    cout << "Error on line " << tok.GetLinenum() << " (" << tok.GetLexeme() << ')' << '\n';
    return -1;
  }
  //print statistics
  if(mci && (mciMap.empty()) != true){
    cout << "Most Common Identifier: ";
    bool first = true;
    for(auto i = mciMap.begin(); i != mciMap.end();i++) {
      if((i->second == lexNum) && first){
        first = false;
        cout << i->first;
      }else if(i->second == lexNum){
        cout << ',' << i->first;
      }
    }
    cout << '\n';
  }
  if(sum){
    cout << "Total lines: " << lineNumber << '\n';
    cout << "Total tokens: " << totalTokens << '\n';
    if(totalTokens != 0){
      cout << "Total strings: " << totalStrings << '\n';
      if(totalStrings != 0) cout << "Length of longest string: " << (--longestWordLen.end())->first <<'\n';
    }
  }

  return 0;
}
