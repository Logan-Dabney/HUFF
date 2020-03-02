#include "Huffman.h"
#include <iostream>
#include <fstream>

Huffman::Huffman()
{
	//root = nullptr;
	for (int i = 0; i < 256; i++) 
	{
		asciiArray[i] = new node();
		asciiArray[i]->character = i;
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
		if (asciiArray[i] != nullptr && asciiArray[i]->numberOfInstances < lowestNode->numberOfInstances)	// if the asciiArray of all instances is not nullptr and 
		{																									// that node's, at num i, weight is less than the lowest nodes weight
			lowestNode->numberOfInstances = asciiArray[i]->numberOfInstances;								// set that node's weight to lowestNode's weight
			lowestNode->character = asciiArray[i]->character;												// and set the node's character to lowestNode's character
			/*lowestIns->numberOfInstances = asciiArray[i];*/
		}
	}

	lowestNode->RCH = asciiArray[lowestNode->character]->RCH;	// set lowestNode's right child to asciiArray's right child at lowestNodes' character's ascii value  
	lowestNode->LCH = asciiArray[lowestNode->character]->LCH;	// same but for the left child						

	asciiArray[(unsigned char)lowestNode->character] = nullptr;					// set asciiArray at lowestNodes' character's ascii value to null
	return lowestNode;
}

// Takes the inputfile given by the user and reads it in char by char to fill the ascii array by number of instances
void Huffman::fillArray(string inputFile)
{
	ifstream inFile;
	inFile.open("C:\\Users\\logan\\Testing\\" + inputFile); // Obtain's file from specified location based on the input entered by user

	if (!inFile)	// If the file isn't found print out the file was not found and exit program
	{
		cout << "The file was not found.";
		exit(1);
	}

	while (inFile.peek() != -1)				// Enter the loop and continue to iterate through as long as the file still contains a char as the next char to be read in
	{
		//char nextChar = inFile.get();		// Obtains the next char	

		asciiArray[(unsigned char)inFile.get()]->numberOfInstances++;		// increment the instance of character taken from file
	//	if (asciiArray[(int)nextChar] == nullptr)			// if the character of the char doesn't exist in the asciiArray create the node and add the character
	//	{
	//		asciiArray[(int)nextChar] = new node();
	//		asciiArray[(int)nextChar]->character = nextChar;
	//	}

	//	asciiArray[(int)nextChar]->numberOfInstances++;		// otherwise increment the instance of that character
	}
	inFile.close();		// close the file
}