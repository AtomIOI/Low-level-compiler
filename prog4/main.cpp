/*
 * main.cpp
 */

#include "parse.h"
#include "parse.cpp"
#include "lex.cpp"
#include <fstream>
#include <iostream>
using namespace std;


int main(int argc, char const *argv[]) {
	istream *in = &cin;
	ifstream file;
	int linenum = 1;

	if(argc > 2){
		cerr << "TOO MANY FILENAMES" << endl;
		return 0;
	}else if(argc == 2){
		file.open(argv[1]);
		if( file.is_open() == false ) {
			cerr << "COULD NOT OPEN " << argv[1] << endl;
			return 0;
		}
		in = &file;
	}

	ParseTree *prog = Prog(in, &linenum);

	if( prog == 0 ){
		return 0;
	}
	/*
	if( prog->GetType() == ERRTYPE){
		cout << "prog has ERRTYPE" << endl;
		return 0;
	}
	*/
	/*
	int idc = prog->IdentCount();
	cout << "LEAF COUNT: " << prog->LeafCount() << endl;
	cout << "IDENT COUNT: " << idc << endl;
	if(idc) {
		map<string,bool> ids;
		prog->GetVars(ids);
		cout << "UNIQUE IDENT COUNT: " << ids.size() << endl;
		bool printed = false;
		for( auto s : ids ) {
			if( printed )
				cout << ", ";
			cout << s.first;
			printed = true;
		}
		cout << endl;
	}
	*/
	prog->Eval();
	return 0;
}
