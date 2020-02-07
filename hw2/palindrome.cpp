/* A program which takes an input file of dictionary of words separated by newline characters
 * and concurrently selects the words in the file that are palindromes, or
 * words where their reverse appear in the wordlist somewhere. Input dictionary should be located in
 * ./dictionary/words and the palindromic words will be printed to ./dictionary/palindromes.
 * 
 * Compile: g++ palindromes.cpp -fopenmp -o -palindromes
 * Usage: ./palindromes #NUM_THREADS
 * View palindromic words: cat dictionary/palindromes
 * Written by Jacob Hedén Malm (@jacobhm98) 07/02/2020
 */

//todo: number of cores should be an argument. print execution time of parallel part.

#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <stdlib.h>
#define DEBUG 0
#define DEFAULT_THREADS 1

using namespace std;

string reverseWord(string);
bool isPalindrome(string);

int NUM_THREADS;

int main (int argc, char * argv[]){
	NUM_THREADS = (argc > 1) ? atoi(argv[1]) : DEFAULT_THREADS;
	
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

	set<string> palindromes;
	omp_set_num_threads(NUM_THREADS);
	cout << "Maximum number of threads allowed: " << NUM_THREADS << endl;
	double startTime = omp_get_wtime();
#pragma omp parallel	//block of code that we want to execute using multiple threads
	#pragma omp single	//we only want one thread to iterate through the foor loop and spawn tasks for the other threads
	{
		#pragma omp task untied 	/* iterating through the for loop is the main task, so 
						 * burden should be shared if execution is suspended
						 */
		{
			for (set<string>::iterator i = wordList.begin(); i != wordList.end(); ++i){
			#pragma omp task	//spawn the tasks of evaluating whether each word should be inserted into palindrome list
				{
					if (isPalindrome(*i)){	//if the word is by itself a palindrome, insert
						palindromes.insert(*i);
					}
					/* if the reverse of the current word is in the wordlist and it hasn't already been inserted,
					 * insert them both into set of palindromes
					 */	
					else if (wordList.find(reverseWord(*i)) != wordList.end()){
						if(palindromes.find(*i) == palindromes.end()){
							palindromes.insert(*i);
							palindromes.insert(reverseWord(*i));
						}
					}
				}
			}
		}
	}
	double endTime = omp_get_wtime() - startTime;
	//iterate over list of palindromes and print them to outputstream (./dictionary/palindromes)
	for (set<string>::iterator i = palindromes.begin(); i != palindromes.end(); i++){
		out << *i << endl;
	}
	cout << "Execution time of the parallel region is: " << endTime << endl;
	cout << "palindromes printed to ./dictionary/palindromes" << endl;
	return 0;
}
/* A function which checks whether
 * given string is a palindrome. Uses pointer
 * comparisons. Could have been done through
 * checking word == reverseWord but
 * pointer comparison is faster than
 * creating new string.
 */
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


/* Generate the reverse of the given word as a new string
 * by going through the given word backwards and appending.
 */
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
