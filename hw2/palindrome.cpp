#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#define DEBUG 0
using namespace std;

string reverseWord(string);
bool isPalindrome(string);

int main (){
	
	//input/output filestreams with files
	ifstream in;
	ofstream out;
	in.open("dictionary/words");
	out.open("dictionary/palindromes");

	// Make sure the dictionary is in the right place
	if (!in) {
		cerr << "there is no dictionary file at ./dictionary/words" << endl;
		return 1;
	}
	//Read input word by word and put in a set (red black tree)
	set<string> wordList;
	string word;
	reverseWord(word);
	while (in >> word){
		wordList.insert(word);
	}
	cout << "The size of the wordList is: " << wordList.size() << endl;

	// concurrently iterate over the words in the set and store the palindromes in a new set
	set<string> palindromes;
#pragma omp parallel
#pragma omp single
	{
#pragma omp task untied
		{
			for (set<string>::iterator i = wordList.begin(); i != wordList.end(); ++i){
#pragma omp task
				{
					if (isPalindrome(*i)){
						palindromes.insert(*i);
					}
					//if the reverse of the current word is in the wordlist, insert them both into palindrome	
					else if (wordList.find(reverseWord(*i)) != wordList.end()){
						palindromes.insert(*i);
						palindromes.insert(reverseWord(*i));
					}
				}
			}
		}
	}


	cout << "palindromes printed to ./dictionary/palindromes" << endl;
	return 0;
}

bool isPalindrome(string word){
	bool isPalindrome = true;
	int p1 = 0;
	int p2 = word.length() - 1;
	while (p1 < p2){
		if (word[p1] ==  word[p2]){
			p1++;
			p2--;
		}
		else {
			isPalindrome = false;
			break;
		}
	}
#if DEBUG == 1
	if (isPalindrome){
	cout << word << " is a palindrome" << endl;
	}
	else {
	cout << word << " is not a palindrome" << endl;
	}
#endif
	return isPalindrome;
}
string reverseWord(string word){
	string reversed;
	for (int i = word.length() - 1; i >= 0; i--){
		reversed += word[i];
	}
#if DEBUG == 1
	cout << reversed.length() << " = " << word.length() << " and " << reversed << " is " << word << " reversed " << endl;
#endif
	return reversed;
}
