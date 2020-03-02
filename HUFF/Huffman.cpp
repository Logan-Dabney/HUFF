#include "Huffman.h"
#include <iostream>
#include <fstream>

Huffman::Huffman()
{
	int asciiArray[256] = {};
	//root = nullptr;
	for (int i = 0; i < 256; i++) 
	{
		freqTable[i] = 0;
		nodeArray[i] = new node();
		nodeArray[i]->character = i;
	}
}

Huffman::~Huffman()
{

}

void Huffman::MakeTreeBuilder(string inputFile, string outputFile)
{
	fillArray(inputFile);

	node* firstLowest = findLowest();
	node* secondLowest = findLowest();

	while (firstLowest != secondLowest)
	{
		node* parent = new node();
		parent->numberOfInstances = firstLowest->numberOfInstances + secondLowest->numberOfInstances;

	}
}

Huffman::node* Huffman::findLowest()
{
	node* lowestNode = new node();
	lowestNode->numberOfInstances = numeric_limits<int>::max(); // set the number of intances to the max character 

	for (int i = 0; i < 256; i++)								// iterates through the entire array
	{
		if (nodeArray[i] != nullptr && nodeArray[i]->numberOfInstances < lowestNode->numberOfInstances)	// if the asciiArray of all instances is not nullptr and 
		{																									// that node's, at num i, weight is less than the lowest nodes weight
			lowestNode->numberOfInstances = nodeArray[i]->numberOfInstances;								// set that node's weight to lowestNode's weight
			lowestNode->character =  nodeArray[i]->character;												// and set the node's character to lowestNode's character
			/*lowestIns->numberOfInstances = asciiArray[i];*/
		}
	}

	lowestNode->RCH = nodeArray[lowestNode->character]->RCH;	// set lowestNode's right child to asciiArray's right child at lowestNodes' character's ascii value  
	lowestNode->LCH = nodeArray[lowestNode->character]->LCH;	// same but for the left child						

	nodeArray[(unsigned char)lowestNode->character] = nullptr;					// set asciiArray at lowestNodes' character's ascii value to null
	return lowestNode;
}

// Takes the inputfile given by the user and reads it in char by char to fill the ascii array by number of instances
void Huffman::fillArray(string inputFile)
{
	char character = 0;
	ifstream inFile;
	inFile.open("C:\\Users\\logan\\Testing\\" + inputFile, ios::binary); // Obtain's file from specified location based on the input entered by user

	if (!inFile)	// If the file isn't found print out the file was not found and exit program
	{
		cout << "The file was not found.";
		exit(1);
	}

	while (inFile.get(character))				// Enter the loop and continue to iterate through as long as the file still contains a char as the next char to be read in
	{
		unsigned char symbol = character;
		freqTable[symbol] += 1;

		//asciiArray[(unsigned char)inFile.get()]->numberOfInstances++;		// increment the instance of character taken from file
	}
	inFile.close();		// close the file

	for (int i = 0; i < 256; i++) 
	{
		nodeArray[i]->numberOfInstances = freqTable[i];
	}

}


	//	if (asciiArray[(int)nextChar] == nullptr)			// if the character of the char doesn't exist in the asciiArray create the node and add the character
	//	{
	//		asciiArray[(int)nextChar] = new node();
	//		asciiArray[(int)nextChar]->character = nextChar;
	//	}

	//	asciiArray[(int)nextChar]->numberOfInstances++;		// otherwise increment the instance of that character