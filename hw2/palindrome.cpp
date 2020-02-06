
#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#define DEBUG 1
using namespace std;

string reverseWord(string);

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
	in >> word;
	reverseWord(word);
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
string reverseWord(string word){
	string reversed;
	for (int i = word.length() - 1; i >= 0; i--){
		reversed += word[i];
	}
#ifdef DEBUG
	cout << reversed.length() << " = " << word.length() << " and " << reversed << " is " << word << " reversed " << endl;
#endif
	return reversed;
}
