//
// parse command line parameters
//
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	for (int i = 0; i < argc; i++){
		cout << argv[i] << " ";
	}
	cout << endl;
	cout << argc << " arguments" << endl;
	return 0;
}
