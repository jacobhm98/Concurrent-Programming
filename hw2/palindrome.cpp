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


#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <omp.h>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <stdlib.h>
#include <vector>
#include <algorithm>
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
	//Read input word by word and put in a vector
	vector<string> wordList;
	string word;
	while (in >> word){
		wordList.push_back(word);
	}
	cout << "The size of the wordList is: " << wordList.size() << endl;
	//for some reason we need to sort the array, or we don't find all the palindromes. Even though
	//it's supposed to be sorted already.	
	sort(wordList.begin(), wordList.end());	
	
	//vector where we are going to store found palindromes, take user input to set threads
	//and start the timer.	
	vector<string> palindromes;
	omp_set_num_threads(NUM_THREADS);
	cout << "Maximum number of threads allowed: " << NUM_THREADS << endl;
	double startTime = omp_get_wtime();

//parallelizable for loop
#pragma omp parallel for	
	for (int i = 0; i < wordList.size(); i ++){
		if (isPalindrome(wordList[i])){
		#pragma omp critical
			{
			palindromes.push_back(wordList[i]);
			}
		}
		else if (binary_search(wordList.begin(), wordList.end(), reverseWord(wordList[i]))){
		#pragma omp critical	
			{
			palindromes.push_back(wordList[i]);
			}
		}
		
	}	

	double endTime = omp_get_wtime() - startTime;
	//iterate over list of palindromes and print them to outputstream (./dictionary/palindromes)
	cout << "number of palindromes found: " << palindromes.size() << endl;
	for (int i  = 0; i < palindromes.size(); i++){
		out << palindromes[i] << endl;
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
