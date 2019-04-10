#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

int main(int argc, char const *argv[]) {
  bool quiet = false; bool squish = false; bool censor = false; bool stats = false; bool length = false;
  int inChar = 0; int outChar= 0; int inLines= 0; int outLines= 0; int inWords= 0; int outWords= 0; int inGood= 0; int inReal= 0; int inCap= 0; int inAcc= 0;
  int numWords = 0; int numGood = 0; int numReal = 0;
  unsigned longestWord = 0; unsigned longestGood = 0; unsigned longestReal = 0;
  vector<string> longestWords; vector<string> longestGoods; vector<string> longestReals;
  vector<string>::iterator found;
  int fileNum = 0;
  char charIn;
  string word;
  ifstream inFile;
  istream *in;
  istream *fileStream;
  stringstream outStream;
  stringstream inputStream;
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
  if(quiet && (censor || squish)){
      cout << "CONFLICTING FLAGS" << '\n';
      return -1;
  }

  //Determine if reading from Standard Input or File
  if(fileNum > 0){
    in = &inFile;
  }else{
    in = &cin;
  }

  if(squish || censor){
    wordStream.str("");
    while(in->get(charIn)){
      inputStream << charIn;
      if(isspace(charIn)){
        if(censor && squish){
          //wordStream to outStream only if last whitespace
          //clear wordStream
          outStream << wordStream.str();
          wordStream.str("");
          if(isspace(charIn) && !isspace(in->peek())){
            outStream << charIn;
          }
        }else if(squish){
          //only print charIn if last whitespace
          if(isspace(charIn) && !isspace(in->peek())){
            outStream << charIn;
          }
        }else{
          //wordStream to outStream and charIn to outStream
          //clear wordStream
          outStream << wordStream.str();
          wordStream.str("");
          outStream << charIn;
        }
      }else if(isalpha(charIn)){
        if(censor || (censor && squish)){
          //write char to wordStream
          wordStream << charIn;
        }else{
          //charIn to outStream
          outStream << charIn;
        }
      }else{
        if(censor){
          //clear wordStream leading letters and spaces
          while(!isspace(charIn)){
            in->get(charIn);
          }
          while (!isalpha(in->peek())) {
            in->get(charIn);
          }
          wordStream.str("");
        }else{
          //charIn to outStream
          outStream << charIn;
        }
      }
    }
  }

  if(squish || censor){
    fileStream = &inputStream;
  }else{
    fileStream = in;
  }

  if(stats || length){
    //read char at a time from input file
    while(fileStream->get(charIn)){
      inChar++;
      if(isspace(charIn) && !isblank(charIn)) inLines++;
      if(!isspace(charIn)){
        //write charIn into wordStream
        wordStream << charIn;
      }else if(isspace(charIn) && !isspace(fileStream->peek())){
        //convert wordStream into string
        word = wordStream.str();
        //clear wordStream
        wordStream.str("");
        inWords++;
        if(word.length() > longestWord){
          longestWord = word.length();
          longestWords.clear();
          longestWords.push_back(word);
        }else if(word.length() == longestWord){
          found = find(longestWords.begin(), longestWords.end(), word);
          if(found == longestWords.end()){
            longestWords.push_back(word);
          }
        }
        if(isupper(word[0])){
          inCap++;
          inGood++;
          if(word.length() > longestGood){
            longestGood = word.length();
            longestGoods.clear();
            longestGoods.push_back(word);
          }else if(word.length() == longestGood){
            found = find(longestGoods.begin(), longestGoods.end(), word);
            if(found == longestGoods.end()){
              longestGoods.push_back(word);
            }
          }
          bool isAcc;
          bool isReal;
          for (size_t i = 0; i < word.length(); i++) {
            if (!isupper(word[i])) {
              isAcc = false;
            }else{
              isAcc = true;
            }
          }
          if (isAcc == true) {
            inAcc++;
          }
          for (size_t i = 0; i < word.length(); i++) {
            if (!isalpha(word[i])) {
              isReal = false;
            }else{
              isReal = true;
            }
          }
          if (isReal == true) {
            inReal++;
            if(word.length() > longestReal){
              longestReal = word.length();
              longestReals.clear();
              longestReals.push_back(word);
            }else if(word.length() == longestReal){
              found = find(longestReals.begin(), longestReals.end(), word);
              if(found == longestReals.end()){
                longestReals.push_back(word);
              }
            }
          }
        }else if(isalnum(word[0])){
          inGood++;
          if(word.length() > longestGood){
            longestGood = word.length();
            longestGoods.clear();
            longestGoods.push_back(word);
          }else if(word.length() == longestGood){
            found = find(longestGoods.begin(), longestGoods.end(), word);
            if(found == longestGoods.end()){
              longestGoods.push_back(word);
            }
          }
          bool isReal;
          for (size_t i = 0; i < word.length(); i++) {
            if (!isalpha(word[i])) {
              isReal = false;
            }else{
              isReal = true;
            }
          }
          if (isReal == true) {
            inReal++;
            if(word.length() > longestReal){
              longestReal = word.length();
              longestReals.clear();
              longestReals.push_back(word);
            }else if(word.length() == longestReal){
              found = find(longestReals.begin(), longestReals.end(), word);
              if(found == longestReals.end()){
                longestReals.push_back(word);
              }
            }
          }
        }
      }
    }
    if(censor || squish){
      wordStream.str("");
      while (outStream.get(charIn)) {
        outChar++;
        //count newLine characters
        if(isspace(charIn) && !isblank(charIn)) outLines++;
        if(isspace(charIn) && !isspace(fileStream->peek())){
          outWords++;
        }
      }
    }else{
      outChar = inChar;
      outLines = inLines;
      outWords = inWords;
    }
  }

  if(quiet){
    outChar = 0;
    outLines = 0;
    outWords = 0;
  }else if(squish || censor){
    string outString = outStream.str();
    cout << outString;
  }else{
    while (inFile.get(charIn)) {
      cout << charIn;
    }
  }
  if(stats){
    cout << "Characters " << inChar << " in " << outChar << " out\n";
    cout << "Lines " << inLines << " in " << outLines << " out\n";
    cout << "Words " << inWords << " in " << outWords << " out\n";
    cout << "Goodwords " << inGood << " in\n";
    cout << "Realwords " << inReal << " in\n";
    cout << "Capwords " << inCap << " in\n";
    cout << "Acronyms " << inAcc << " in\n";
  }
  if(length){
    cout << "Word (length " << longestWord << "):";
    if(longestWords.size() == 1){
      cout << longestWords[0];
    }else{
      for (unsigned i = 0; i < longestWords.size() -1; i++) {
        cout << longestWords[i] << ",";
        if(i == (longestWords.size() - 1)){
          cout << longestWords[i] << '\n';
        }
      }
    }
    cout << "Goodword (length " << longestGood << "):";
    if(longestGoods.size() == 1){
      cout << longestGoods[0];
    }else{
      for (unsigned i = 0; i < longestGoods.size() -1; i++) {
        cout << longestGoods[i] << ",";
        if(i == (longestGoods.size() - 1)){
          cout << longestGoods[i] << '\n';
        }
      }
    }
    cout << "Realword (length " << longestReal << "):";
    if(longestReals.size() == 1){
      cout << longestReals[0];
    }else{
      for (unsigned i = 0; i < longestReals.size() -1; i++) {
        cout << longestReals[i] << ",";
        if(i == (longestReals.size() - 1)){
          cout << longestReals[i] << '\n';
        }
      }
    }
  }

  numWords++;numGood++;numReal++;
  inFile.close();
  return 0;
}
