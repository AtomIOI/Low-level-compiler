#include <iostream>
using namespace std;

int main(int argc, char *argv[]){
	if(argc < 2){
		cout << "MISSING SIZE" << endl;
		return 0;
	}

	switch(argv[1]){
		case 'small':
			for (i=2; i < argc; i++){
				if(argv[i] >= 5){
					cout << argv[i] << endl;

				}
			}
		case 'medium':
			for (i=2; i < argc; i++){
				if(argv[i] >= 10){
					cout << argv[i] << endl;

				}
			}
		case 'large':
			for (i=2; i < argc; i++){
				if(argv[i] >= 20){
					cout << argv[i] << endl;

			    }
			}
	}

}
