#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <string.h>
using namespace std;

stringstream currLexeme;
string strLexeme;
TType tokenType;
char charIn;

Token getNextToken(istream *in, int *linenumber){
  //Start of new lexeme/token
  while(in->get(charIn)){

    if(isspace(charIn)){
      if(!isblank(charIn)) *linenumber = *linenumber +1; //check for newline char
    }

    else if(isalpha(charIn)){ //check for IDENT or keywords
      currLexeme << charIn;
      //check for end of file and read next char
      //keep reading until not an isalnum()
      while(in->get(charIn) && isalnum(charIn)){
        currLexeme << charIn;
      }
      if (isspace(charIn) && !isblank(charIn)){
        *linenumber = *linenumber +1;
      }else{
        in->putback(charIn); //put back last char read since not part of lexeme
      }
      strLexeme = currLexeme.str();
      currLexeme.str(""); //clear lexeme
      //seperate keywords and IDENT
      if("var" == strLexeme) return Token(VAR, strLexeme, *linenumber);
      else if("print" == strLexeme) return Token(PRINT, strLexeme, *linenumber);
      else if("set" == strLexeme) return Token(SET, strLexeme, *linenumber);
      else if("repeat" == strLexeme)return Token(REPEAT, strLexeme, *linenumber);
      else return Token(IDENT, strLexeme, *linenumber);
    }

    else{ //check for ICONST SCONST comments and operators
      if(isdigit(charIn)){
        currLexeme <<  charIn;
        while (in->get(charIn) && isdigit(charIn)){
          currLexeme << charIn;
        }
        if(isalpha(charIn)){
          currLexeme << charIn;
          strLexeme = currLexeme.str();
          return Token(ERR, strLexeme, *linenumber);
        }
        if (isspace(charIn) && !isblank(charIn)){
          *linenumber = *linenumber +1;
        }else{
          in->putback(charIn); //put back last char read since not part of lexeme
        }
        strLexeme = currLexeme.str();
        currLexeme.str("");
        return Token(ICONST, strLexeme, *linenumber);
      }
      switch (charIn) {
        case '"':
          while(in->get(charIn) && !(isspace(charIn) && !isblank(charIn)) && (charIn != '"')){
            currLexeme << charIn;
          }
          if(charIn == '"'){
            strLexeme = currLexeme.str();
            currLexeme.str("");
            return Token(SCONST, strLexeme, *linenumber);
          }else if(isspace(charIn) && !isblank(charIn)){
            *linenumber = *linenumber +1;
            currLexeme << charIn;
            strLexeme = '"' + currLexeme.str();
            return Token(ERR, strLexeme, *linenumber);
          }
          break;
        case '-':
          currLexeme << charIn;
          while (in->get(charIn) && isdigit(charIn)){
            currLexeme << charIn;
          }
          if (isspace(charIn) && !isblank(charIn)){
            *linenumber = *linenumber +1;
          }else{
            in->putback(charIn); //put back last char read since not part of lexeme
          }
          strLexeme = currLexeme.str();
          currLexeme.str("");
          if(strLexeme == "-"){
            return Token(MINUS, strLexeme, *linenumber);
          }else{
            return Token(ICONST, strLexeme, *linenumber);
          }
          break;
        case '#':
          while (in->get(charIn) && !(isspace(charIn) && !isblank(charIn))) {
            //do nothing just ignore characters
          }
          if (isspace(charIn) && !isblank(charIn)) *linenumber = *linenumber +1;
          break;
        case '+':
          return Token(PLUS, "+", *linenumber);
          break;
        case '*':
          return Token(STAR, "*", *linenumber);
          break;
        case ':':
          return Token(COLON, ":", *linenumber);
          break;
        case '[':
          return Token(LSQ, "[", *linenumber);
          break;
        case ']':
          return Token(RSQ, "]", *linenumber);
          break;
        case '(':
          return Token(LPAREN, "(", *linenumber);
          break;
        case ')':
          return Token(RPAREN, ")", *linenumber);
          break;
        case ';':
          return Token(SC, ";", *linenumber);
          break;
        default:
          currLexeme << charIn;
          strLexeme = currLexeme.str();
          return Token(ERR, strLexeme, *linenumber);
      }
    }
  }
  currLexeme << charIn;
  strLexeme = currLexeme.str();
  return Token(DONE, strLexeme, *linenumber);
}

//overloading << operator for Token obj
ostream& operator<<(ostream& out, const Token& tok){
  if(tok == IDENT){
    out << "IDENT" << "(" << tok.GetLexeme() << ")"<< '\n';
    return out;
  }else if(tok == ICONST){
    out << "ICONST" << "(" << tok.GetLexeme() << ")"<< '\n';
    return out;
  }else if(tok == SCONST){
    out << "SCONST" << "(" << tok.GetLexeme() << ")"<< '\n';
    return out;
  }else if(tok == VAR){
    out << "VAR" << '\n';
    return out;
  }else if(tok == PRINT){
    out << "PRINT" << '\n';
    return out;
  }else if(tok == SET){
    out << "SET" << '\n';
    return out;
  }else if(tok == REPEAT){
    out << "REPEAT" << '\n';
    return out;
  }else if(tok == PLUS){
    out << "PLUS" << '\n';
    return out;
  }else if(tok == MINUS){
    out << "MINUS" << '\n';
    return out;
  }else if(tok == STAR){
    out << "STAR" << '\n';
    return out;
  }else if(tok == COLON){
    out << "COLON" << '\n';
    return out;
  }else if(tok == LSQ){
    out << "LSQ" << '\n';
    return out;
  }else if(tok == RSQ){
    out << "RSQ" << '\n';
    return out;
  }else if(tok == LPAREN){
    out << "LPAREN" << '\n';
    return out;
  }else if(tok == RPAREN){
    out << "RPAREN" << '\n';
    return out;
  }else{
    out << "SC" << '\n';
    return out;
  }
}
