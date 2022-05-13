#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "string"

using namespace std;

int generateRandoms(int start, int end)
{
	srand(time(0));
	int num = rand() % (end + 1 - start) + start;
	return num;
}
int noOfLines(string name) // it will return the number of lines in the file
{
	int lines = 0;
	fstream inFile;
	inFile.open(name);
	string reader;
	while (getline(inFile, reader)) // read a line
	{
		lines++; // increment the count
	}
	inFile.close(); // close the file
	return lines;
}

void readData(string name, char** words, char*** synonyms, int** synonymSizes, int* noOfSynonym) // this function will read data of file in multiple arrays
{
	int lineNo = 0;
	fstream inFile;
	inFile.open(name);
	string reader;
	while (getline(inFile, reader))
	{
		int wordSize = 0;
		int i, j = 0;
		int noOfSynonyms = 0;
		for (i = 0; reader[i] != ' '; i++)
		{
			wordSize++;
		}
		i++; // to set index to next word which will be our synonym
		words[lineNo] = new char[wordSize + 1];
		for (j = 0; j < wordSize; j++)
		{
			words[lineNo][j] = reader[j];
		}
		words[lineNo][j] = '\0'; // adding a stoping character 
		noOfSynonyms = count(reader.begin(), reader.end(), ' '); //after storing the word we need to count synonym of that word
		synonyms[lineNo] = new char* [noOfSynonyms + 1];
		synonymSizes[lineNo] = new int [noOfSynonyms + 1];
		noOfSynonym[lineNo] = noOfSynonyms;

		int index = i; // using temporary variable index
		for (int synonymNo = 0; synonymNo < noOfSynonyms; synonymNo++)
		{
			int synonymSize = 0;
			int s = 0;
			if (synonymNo < noOfSynonyms - 1) //if synonym is not last
			{
				for (; reader[index] != ' '; index++) // stop by space
				{
					synonymSize++; // increasing size
				}
				index++;
			}
			else //if synonym is last
			{
				for (; reader[index] != '\0'; index++) // stop by end character
				{
					synonymSize++; // increasing size
				}
			}
			synonyms[lineNo][synonymNo] = new char[synonymSize + 1]; // creating space for new synonym size
			synonymSizes[lineNo][synonymNo] = synonymSize;
			for (s = 0; s < synonymSize; s++, i++) // coping synonym to our array
			{
				synonyms[lineNo][synonymNo][s] = reader[i];
			}
			synonyms[lineNo][synonymNo][s] = '\0'; // stoping character at the end
			i = index;
		}
		synonyms[lineNo][noOfSynonyms] = nullptr; // last pointer of synonym characters will be null to stop our loops later
		synonymSizes[lineNo][noOfSynonyms] = 0;
		lineNo++; // next line of the file
	}
	inFile.close();
}

void tokenize(string input, char** inputTokens, int noOfTokens) // this function will break the input into words
{
	int index = 0;
	int tokenIndex = 0;
	int i = 0;
	for (i; i < noOfTokens; i++)
	{	
		int sizeOfToken = 0;
		int t = 0;
		if (i < noOfTokens - 1) // if token is not last
		{
			for (; input[index] != ' '; index++)
			{
				sizeOfToken++;
			}
			index++; // skipping the space for next time when loop will run
		}
		else
		{
			for (; input[index] != '\0'; index++)
			{
				sizeOfToken++;
			}
		}
		inputTokens[i] = new char[sizeOfToken + 1];
		for (t = 0; t < sizeOfToken; t++)
		{
			inputTokens[i][t] = input[tokenIndex++];
		}
		tokenIndex++;
		inputTokens[i][t] = '\0'; // storing the null character at the end of word
	}
}

// this function will place random synonyms of matched words in the tokens array
void paraphrase(int lines, int noOfTokens, char** inputTokens, char** words, char*** synonyms, int** synonymSizes, int* noOfSynonym)
{
	int tokenNo = 0;
	for (tokenNo; tokenNo < noOfTokens; tokenNo++)
	{
		for (int wordNo = 0; wordNo < lines; wordNo++)
		{
			bool matched = true;
			for (int i = 0; words[wordNo][i] != '\0' || inputTokens[tokenNo][i] != '\0'; i++) // if any of the word is not finished
			{
				if (words[wordNo][i] != '\0' && inputTokens[tokenNo][i] != '\0')
				{
					if (words[wordNo][i] != inputTokens[tokenNo][i])
					{
						matched = false;
					}
				}
				else if (words[wordNo][i] == '\0' && inputTokens[tokenNo][i] != '\0') // if words are of different sizes
				{
					matched = false; 
				}
				else
				{
					matched = false;
				}
			}
			if (matched == true) // if word is matched
			{
				delete[] inputTokens[tokenNo];
				int randomPosition = generateRandoms(0, noOfSynonym[wordNo] - 1); // return random number between no of synonyms
				int newTokenSize = synonymSizes[wordNo][randomPosition];
				inputTokens[tokenNo] = new char[newTokenSize + 1];
				for (int o = 0; o < newTokenSize; o++)
				{
					inputTokens[tokenNo][o] = synonyms[wordNo][randomPosition][o];
				}
				inputTokens[tokenNo][newTokenSize] = '\0'; // placing end character at the last
			}
		}
	}
}
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // for checking memory leak
//reading words and synonyms
	int lines = noOfLines("data.txt");
	char** words = new char* [lines + 1]; // a pointer to a dynamic array to store word (1 dimensional)
	char*** synonyms = new char** [lines + 1]; // a pointer to a dynamic array of arrays (2 dimentional)
	int** synonymSizes = new int*[lines + 1];
	int* noOfsynonyms = new int [lines + 1];
	readData("data.txt", words, synonyms, synonymSizes, noOfsynonyms); // it will store words and theirs synonyms in respective arrays

//user input section
	string input = "";
	getline(cin, input);
	int noOfTokens = count(input.begin(), input.end(), ' ') + 1;
	char** inputTokens = new char* [noOfTokens + 1];
	tokenize(input, inputTokens, noOfTokens);
	paraphrase(lines, noOfTokens, inputTokens, words, synonyms, synonymSizes, noOfsynonyms);
	for (int i = 0; i < noOfTokens; i++) // it will print the paraphrased paragraph as our output
	{
		cout << inputTokens[i] << " ";
	}

//deallocating the memory of words
	for (int i = 0; i < lines; i++)
	{
		delete[] words[i];
	}
	delete[] words;

//deallocating the memory of synonyms
	for (int i = 0; i < lines; i++)
	{
		for (int j = 0; j < noOfsynonyms[i]; j++)
		{
			delete[] synonyms[i][j];
		}
		delete[] synonyms[i];
	}
	delete[] synonyms;

//deallocating the memory of tokens
	for (int i = 0; i < noOfTokens; i++)
	{
		delete[] inputTokens[i];
	}
	delete[] inputTokens;

//deallocating the memory of synonym sizes
	for (int i = 0; i < lines; i++)
	{
		delete[] synonymSizes[i];
	}
	delete[] synonymSizes;

//deallocating the memory of noOfsynnonyms
	delete[] noOfsynonyms;

	system("Pause");
	return 0;
}