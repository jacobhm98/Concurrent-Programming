
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
using namespace std;

int main (){
	ifstream in;
	ofstream out;
	in.open("dictionary/words");
	out.open("dictionary/palindromes");
	set<string> wordList;
	if (!in) {
		cerr << "there is no dictionary file at ./dictionary/words" << endl;
		return 1;
	}
	string word;
	while (in >> word){
		wordList.insert(word);	
	}
	cout << "The size of the wordList is: " << wordList.size() << endl;
	cout << "palindromes printed to ./dictionary/palindromes" << endl;
	return 0;
}

bool isPalindrome(string word){
	bool isPalindrome = false;
	if (word.length() == 1){
		isPalindrome = true;
		return isPalindrome;
	}
	return isPalindrome;
}
