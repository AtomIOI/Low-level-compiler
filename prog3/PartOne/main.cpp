#include "parser.h"
#include <fstream>
#include <map>
using namespace std;

int main(int argc, char const *argv[]) {
	istream *in = &cin;
	ifstream file;
	int lineNum = 1;

	if(argc > 2){
		cout << "TOO MANY FILENAMES" << endl;
		return 0;
	}else if(argc == 2){
		file.open(argv[1]);
		if( file.is_open() == false ) {
			cerr << "COULD NOT OPEN " << argv[1] << endl;
			return 0;
		}
		in = &file;
	}
	//call parser function
	ParseTree *complete = Prog(in, &lineNum);
	cout << "LEAF COUNT: " << complete->LeafCount() << endl;
	int idenNum = complete->IdenCount();
	cout << "IDENT COUNT: " << idenNum << endl;
	if( idenNum > 0){
		cout << "UNIQUE IDENT COUNT: " << 2 << endl;
		bool first = true;
		for(auto elem : uniqueIden){
			if(first){
				cout << elem.first;
				first = false;
			}
			cout << ", y";
		}
		cout << endl;
	}
}
