//-------------------------------------------------------------------------
// File: HashMain.cpp
// Purpose: Main File For Programming Assignment 4
//          Hashing Tables and Collision Resolution Test
// Programming Language: C++
// Author: Jacob Hauenstein (adapted from Rick Coleman's work)
// Date:   November 2020
//-------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <string.h>
#define TABLESIZE     100
#define KEYSIZE       4
#define EMPTYKEY      "----"
#define DATAFILE      "\P4DATA.TXT"

using namespace std;

// The struct to use for the hash table
struct HashStruct
{
	char Key[5];
	int	 data;
};

// Function prototypes for utility functions
void InitTable(HashStruct hashT[], int TableSize);
int HashInsert(HashStruct T[], char *key, int data, int hNum, int dhNum);

// Function prototypes for hashing and rehashing (aka double hashing) functions
// (You'll need to write these)
int Hash_1(char *key);
int Hash_2(char *key);
int Hash_3(char *key);
int ProbeDec_1(char *key);
int ProbeDec_2(char *key);
int ProbeDec_3(char *key);

int main()
{
	int          i, hashNum, dHashNum, count;
	ifstream     *inFile;
	HashStruct   T[100];  // Hash table array of 100 data structures
	char         line[64];// Array to hold lines read from file
	char         key[5];  // Array to hold 4-character keys
	int          data;    // Integer data

	// For each of three hash functions
	for (hashNum = 0; hashNum < 3; hashNum++)
	{
		//		 cout << "Starting hashNum " << hashNum << "\n";
				 // For each of three double hash functions
		for (dHashNum = 0; dHashNum < 3; dHashNum++)
		{

			//			cout << "About to init table\n";
			cout.flush();
			InitTable(T, TABLESIZE);               // Call function to initialize table to empty
//			cout << "Table initialized\n";
			cout.flush();
			inFile = new ifstream();
			inFile->open(DATAFILE, ifstream::in);   // Open data file for this test
			if (!inFile->is_open())
			{
				cout << "Unable to open data file. Program terminating\n";
				return 1;
			}

			//			cout << "File opened\n";
			cout.flush();
			count = 0;     // Initialize collision counter

			for (i = 0; i < 50; i++)
			{
				inFile->getline(line, 64, '\n');
				sscanf(line, "%s %d", key, &data);
				//				 cout << "Hashing " << key << "\n";
				//				 cout.flush();
				count += HashInsert(T, key, data, hashNum, dHashNum);
				// Note: The HashInsert() function uses the indices of hashNum and
				// dHashNum to know which hash function and double hash function to call
				// in this test.
			}
			//	fclose(fp);
			inFile->close();		// Close the text file 
			if (inFile->is_open()) cout << "Error: file is still open\n";
			else delete inFile;

			// Output the results
			cout << "Testing hash function " << hashNum << " using double hash " << dHashNum << ".\n";
			cout << "Total collisions = " << count << ".\n";
			// Show the form of the array
			for (i = 0; i < 100; i++)
			{
				if (strcmp(T[i].Key, EMPTYKEY))
					cout << "|";     // Indicate an entry at this location
				else
					cout << "-";     // Indicate no entry at this location
			}
			cout << "\n";
			cout << "Finished run hashNum=" << hashNum << "  dHashNum=" << dHashNum << "\n\n";
			cout.flush();
		}
		//		cout << "Finished run " << hashNum << "\n";
	}

	return 0;
}

//---------------------------------
// InitTable()
// Initialize hash table to all
// empty slots.
//---------------------------------
void InitTable(HashStruct hashT[], int TableSize)
{
	int i;

	for (i = 0; i < TableSize; i++)
	{
		strcpy(hashT[i].Key, EMPTYKEY);
		hashT[i].data = 0;
	}
}


//---------------------------------
// HashInsert()
// Insert an item into the hash
// table given the key, data,
// hash table index and double
// hash index.
//---------------------------------
int HashInsert(HashStruct T[], char *key, int data, int hNum, int dhNum)
{
	int  testNum = (hNum * 3) + dhNum;
	int  colCount = 0;
	int  hashIndex, probeDec;

	//	cout << "Insert with " << hNum << ", " << dhNum << ", test=" << testNum << "\n";

	switch (testNum)
	{
	case 0:  // Hash function 1 -- Double hash 1 (linear probing) 
		hashIndex = Hash_1(key);
		probeDec = ProbeDec_1(key); // Function just returns 1 
		break;
	case 1:  // Hash function 1 -- Double hash 2  
		hashIndex = Hash_1(key);
		probeDec = ProbeDec_2(key);
		break;
	case 2:  // Hash function 1 -- Double hash 3  
		hashIndex = Hash_1(key);
		probeDec = ProbeDec_3(key);
		break;
	case 3:  // Hash function 2 -- Double hash 1 (linear probing)  
		hashIndex = Hash_2(key);
		probeDec = ProbeDec_1(key); // Function just returns 1
		break;
	case 4:  // Hash function 2 -- Double hash 2  
		hashIndex = Hash_2(key);
		probeDec = ProbeDec_2(key);
		break;
	case 5:  // Hash function 2 -- Double hash 3  
		hashIndex = Hash_2(key);
		probeDec = ProbeDec_3(key);
		break;
	case 6:  // Hash function 3 -- Double hash 1 (linear probing)  
		hashIndex = Hash_3(key);
		probeDec = ProbeDec_1(key); // Function just returns 1
		break;
	case 7:  // Hash function 3 -- Double hash 2  
		hashIndex = Hash_3(key);
		probeDec = ProbeDec_2(key);
		break;
	case 8:  // Hash function 3 -- Double hash 3  
		hashIndex = Hash_3(key);
		probeDec = ProbeDec_3(key);
		break;
	}


	// Find a place to insert into the table
	while (strcmp(T[hashIndex].Key, EMPTYKEY) != 0)
	{
		colCount++;
		hashIndex += probeDec;  // Decrementing was chosen you could also choose
		hashIndex %= TABLESIZE;
		//		  cout << "Index " << hashIndex << "\n";
	}

	/* Insert entry into table */
	strcpy(T[hashIndex].Key, key);
	T[hashIndex].data = data;

	/* Return count of collisions */
	return(colCount);
}

//--------------------------------------------------------
// INSERT YOUR CODE INTO THE INDICATED PLACES BELOW.  
// YOU MAY ASSUME THE FOLLOWING DATA IS GLOBALLY KNOWN
// Size of Hash Table = 100
// Number of items to Hash = 50
// Range of keys = AAAA to ZZZZ
//--------------------------------------------------------

//-----------------------------------
// Hash_1()
// Args: key -- integer key to hash
// Return: Index into hash table
//-----------------------------------
int Hash_1(char *key)
{
	//Write your first hash function here. It should use the key to compute an int.
	return 1;
}

//-----------------------------------
// Hash_2()
// Args: key -- integer key to hash
// Return: Index into hash table
//-----------------------------------
int Hash_2(char *key)
{
	//Write your second hash function here. It should use the key to compute an int.
	return 1;
}

//-----------------------------------
// Hash_3()
// Args: key -- integer key to hash
// Return: Index into hash table
//-----------------------------------
int Hash_3(char *key)
{
	//Write your third hash function here. It should use the key to compute an int.
	return 1;
}

//------------------------------------------
// ProbeDec_1()
// Args: key -- integer key to double hash
// Return: Index into hash table
//------------------------------------------
int ProbeDec_1(char *key)
{
	//Write your first double hash function here.
	//Remember that one of these should just return a value of 1 (i.e., linear probing).
	//Others should always return non-zero values and guarantee that all elements in the table
	//can be accessed.
	return 1;
}

//------------------------------------------
// ProbeDec_2()
// Args: key -- integer key to double hash
// Return: Index into hash table
//------------------------------------------
int ProbeDec_2(char *key)
{
	//Write your second double hash function here.
	//Remember that one of these should just return a value of 1 (i.e., linear probing).
	//Others should always return non-zero values and guarantee that all elements in the table
	//can be accessed.
	return 1;
}

//------------------------------------------
// ProbeDec_3()
// Args: key -- integer key to double hash
// Return: Index into hash table
//------------------------------------------
int ProbeDec_3(char *key)
{
	//Write your second double hash function here.
	//Remember that one of these should just return a value of 1 (i.e., linear probing).
	//Others should always return non-zero values and guarantee that all elements in the table
	//can be accessed.
	return 1;
}