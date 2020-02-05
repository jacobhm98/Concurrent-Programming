
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main (){
	ifstream in;
	ofstream out;
	in.open("dictionary/words");
	out.open("dictionary/palindromes");
	if (!in) {
		cerr << "there is no dictionary file at ./dictionary/words" << endl;
		return 1;
	}
	string word;
	while (in >> word){
		out << word << "\n";
	}
	cout << "palindromes printed to ./dictionary/palindromes" << endl;


}
