#include "Huffman.h"
#include <iostream>
#include <fstream>
#include <time.h>

Huffman::Huffman()
{
	//int asciiArray[256] = {};
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

// This method is used to create an huffan tree and create a file for it. It is passed an input
// file and an optional output file. THe input file is used to fill the nodeArray and build the 
// the huffman tree. Which is returned and put into the output file
void Huffman::MakeTreeBuilder(string inputFile, string outputFile)
{
	clock_t start = clock();

	string treeOrder = fillArray(inputFile);	// input file is sent to fillArray and a string of node combinations used to make the huffman tree is returned
	ofstream treeFile;	// creating output stream

	if (outputFile == "") treeFile.open(inputFile.substr(0, inputFile.find_last_of('.')) + ".htree", ios::out | ios::binary);	// if the file is nothing take the input file name and tack on .htree
	else if (outputFile.find(".htree") == string::npos && outputFile.find(".510") == string::npos)								// if the output file doesn't contain .510 or .htree exit code
	{
		cout << "Output file name enterted does not contain htree or 510";
		exit(1);
	}
	else treeFile.open(outputFile, ios::out | ios::binary);																		// otherwise open output file as is

	bytesIn = nodeArray[0]->numberOfInstances;
	bytesOut = 510;

	treeFile << treeOrder;	// add the treeOrder to the treeFile
	treeFile.close();		// close the file
	methodTime(start, clock());
}

// This function is used to compress/encode an input file with a separate tree as the one that would be created
// from the input file. It is printed to the output file that would be entered by the user. If nothing is entered
// it will take the inputfile name and tack on .huf at the end of it for the output file. It also handles a variety 
// of errors that could happen like, unable to find inputfile, incomplete tree file, etc.
void Huffman::EncoderFileWithTree(string inputfile, string treeFile, string outputFile)
{
	clock_t start = clock();

	writeToFileE(inputfile, outputFile, createTreeFromFileOrPairString(treeFile));	// used to write to the output file for both the encoding with tree and regular encoding
																					// this function calls createTreeFromFile which creates the huffman tree and returns the treeOrder string
	methodTime(start, clock());
}

// This function is used to compress/encode an input file
void Huffman::EncoderFile(string inputFile, string outputFile)
{
	clock_t start = clock();

	writeToFileE(inputFile, outputFile);

	methodTime(start, clock());
}

void Huffman::DecoderFile(string inputFile, string outputFile)
{
	clock_t start = clock();

	writeToFileD(inputFile, outputFile);

	methodTime(start, clock());
	// TO-DO if the file is found read in the first 510 bytes this is the treeOrder check to see if it is correct file
	//
}

// This function is used by EncoderFile and EncoderFileWithTree to encode and write to an output file. It's
// parameters are needed: inputfile string, optional: outputfile string and a treeorder string. 
void Huffman::writeToFileE(string inputFile, string outputFile, string treeOrder)
{
	string buffer = "";
	ifstream inFile;
	ofstream encodedFile;

	if (treeOrder == "") treeOrder = fillArray(inputFile);

	buildEncodingMap(nodeArray[0]);

	if (outputFile == "") encodedFile.open(inputFile.substr(0, inputFile.find_last_of('.')) + ".enc", ios::out | ios::binary);
	else if (outputFile.find(".enc") == string::npos && outputFile.find(".huf") == string::npos)
	{
		cout << "Output file name enterted does not contain .enc";
		exit(1);
	}
	else encodedFile.open(outputFile, ios::out | ios::binary);

	encodedFile << treeOrder;
	bytesOut += 510;

	inFile.open(inputFile, ios::binary);

	// if the file doesn't open

	while (inFile.peek() != -1)
	{
		unsigned char character = inFile.get();
		buffer += encodingMap[character];
		bytesIn++;

		while (buffer.length() >= 8)
		{
			char b = 0;

			if (buffer[0] == '1') b |= (1 << 7);
			if (buffer[1] == '1') b |= (1 << 6);
			if (buffer[2] == '1') b |= (1 << 5);
			if (buffer[3] == '1') b |= (1 << 4);
			if (buffer[4] == '1') b |= (1 << 3);
			if (buffer[5] == '1') b |= (1 << 2);
			if (buffer[6] == '1') b |= (1 << 1);
			if (buffer[7] == '1') b |= (1 << 0);
			buffer.erase(0, 8);

			encodedFile << b;
			bytesOut++;
		}
	}


	if (buffer.length() < 8 && buffer.length() > 0)
	{
		char b = 0;
		buffer += longestPath.substr(0, 8 - buffer.length());

		if (buffer[0] == '1') b |= (1 << 7);
		if (buffer[1] == '1') b |= (1 << 6);
		if (buffer[2] == '1') b |= (1 << 5);
		if (buffer[3] == '1') b |= (1 << 4);
		if (buffer[4] == '1') b |= (1 << 3);
		if (buffer[5] == '1') b |= (1 << 2);
		if (buffer[6] == '1') b |= (1 << 1);
		if (buffer[7] == '1') b |= (1 << 0);
		buffer.erase(0, 8);

		encodedFile << b;
		bytesOut++;
	}

	encodedFile.close();
}

void Huffman::writeToFileD(string inputFile, string outputFile)
{
	ofstream decodedFile;
	ifstream decodeInFile;
	char character;
	string treeOrder;
	node* p;

	decodeInFile.open(inputFile, ios::in | ios::binary);

	if (!decodeInFile)
	{
		cout << "The decode file could not be found";
		exit(1);
	}

	if (outputFile == "") decodedFile.open(inputFile.substr(0, inputFile.find_last_of('.')) + ".txt", ios::out | ios::binary);
	else if (outputFile.find(".txt") == string::npos)
	{
		cout << "Output file name enterted does not contain .txt";
		exit(1);
	}
	else decodedFile.open(outputFile, ios::out | ios::binary);

	for (int i = 0; i < 510; i++)
	{
		decodeInFile.get(character);
		treeOrder += (unsigned char)character;

		if (decodeInFile.peek() == -1)
		{
			cout << "Incomplete encoded file. Does not contain full huffman pair string.";
			exit(1);
		}
	}

	createTreeFromFileOrPairString("", treeOrder);
	buildEncodingMap(nodeArray[0]);

	p = nodeArray[0];

	while (decodeInFile.get(character))
	{
		string buffer = "00000000";
		unsigned int byte = (unsigned char)character;
		bytesIn++;

		if ((byte >> 7 & 1) == 1) buffer[0] = '1';
		if ((byte >> 6 & 1) == 1) buffer[1] = '1';
		if ((byte >> 5 & 1) == 1) buffer[2] = '1';
		if ((byte >> 4 & 1) == 1) buffer[3] = '1';
		if ((byte >> 3 & 1) == 1) buffer[4] = '1';
		if ((byte >> 2 & 1) == 1) buffer[5] = '1';
		if ((byte >> 1 & 1) == 1) buffer[6] = '1';
		if ((byte >> 0 & 1) == 1) buffer[7] = '1';

		for (int i = 0; i < 8; i++) 
		{
			if (buffer[i] == '1') p = p->RCH;
			if (buffer[i] == '0') p = p->LCH;
			if (p->LCH == nullptr && p->RCH == nullptr)
			{
				decodedFile << (char)p->character;
				p = nodeArray[0];

				bytesOut++;
			}
		}
	}

	decodedFile.close();
}

// This method builds the encodingmap(class variable) used to encode a file. This function is to be used after
// the combineNodes() function. It is a post order transversal that creates the binary equavalent from the
// completed huffman tree
void Huffman::buildEncodingMap(node* p)
{
	if (p->LCH != nullptr)			// if the pointer has a left child attach 0 to binaryString to signify a left branch
	{								// then call the buildEncodingMap on left child
		binaryString += "0";
		buildEncodingMap(p->LCH);
	}
	if (p->RCH != nullptr)			// if the pointer has a right child attach 1 to binaryString to signify a right branch
	{								// then call the buildEncodingMap on right child
		binaryString += "1";
		buildEncodingMap(p->RCH);
	}
	if (p->LCH == nullptr && p->RCH == nullptr)												// if both children are nullpointer as to exclude the parents as they do not contain
	{																						// characters set the longest path as long as the given binaryString's length
		if (binaryString.length() > longestPath.length()) longestPath = binaryString;		// is longer than longestPath's length
		encodingMap[p->character] = binaryString;											// then set the encodingMap's index and pointers character equal to binary string

	}
	if (binaryString != "") binaryString.pop_back();	// pops the last character off the binary string (going back up the tree)
}

// This function is used to create a huffman tree from a .htree file. It reads in the .htree file into a treeOrder string
// using tree order it combines from the intialized nodeArray in the order as specified by the string. This works because
// instances are not needed for anything beisdes finding the order of combining nodes. Since the ordwer is obtained from the 
// file entered by the user this is all that is needed. 
// This function also handles errors like: no .htree file found and incomplete htree file
string Huffman::createTreeFromFileOrPairString(string treeFile, string treeOrder)
{
	char character;			// creating a char value named character
	//string treeOrder;		// creating the string treeOrder to read the file into
	ifstream treeInFile;	// creating the ifstream used to access the file

	if (treeFile != "")
	{
		treeInFile.open(treeFile, ios::in | ios::binary); //opens the .htree file in binary mode

		if (!treeInFile)	// If the file isn't found print out the file was not found and exit program
		{
			cout << "The tree file was not found.";
			exit(1);
		}
		else if (treeFile.find(".htree") == string::npos && treeFile.find(".510") == string::npos)	// if the output file doesn't contain .510 or .htree exit code
		{
			cout << "Output file name enterted does not contain .htree or .510";
			exit(1);
		}

		while (treeInFile.get(character))			// while another character can be returned the file and put into character continue loop
		{
			treeOrder += (unsigned char)character;	// add the character to treeOrder string as an unsigned char
		}
	}

	if (treeOrder.length() != 510)	// if the string is no the length of 510 it is either incomplete or to large. Exit with code 1
	{
		cout << "File used for tree was an incomplete file.";
		exit(1);
	}

	for (int i = 0; i < treeOrder.length(); i += 2)			// while i is less than the string treeOrder's length then continue to loop after each interation add 2 to i
	{
		node* parent = new node();							// create/initialize parent as node pointer 
		node* left = new node();							// create/initialize left as node pointer 
		node* right = new node();							// create/initialize right as node pointer 
		int leftNum = (unsigned char)treeOrder[i];			// create/initialize leftNum as the int value of treeOder and index of i (this works because string are arrays of chars)
		int rightNum = (unsigned char)treeOrder[i + 1];		// create/initialize rightNum as the int value of treeOder and index of i + 1

		left->character = nodeArray[leftNum]->character;	// set the left's attributes equal to the nodeArray's index at leftNum
		left->LCH = nodeArray[leftNum]->LCH;
		left->RCH = nodeArray[leftNum]->RCH;

		right->character = nodeArray[rightNum]->character; // set the right's attributes equal to the nodeArray's index at rightNum
		right->LCH = nodeArray[rightNum]->LCH;
		right->RCH = nodeArray[rightNum]->RCH;

		parent->LCH = left;		// parents left child becomes left
		parent->RCH = right;	// parents right child become right

		nodeArray[leftNum] = parent;	// set the index of nodeArray at leftNum = to the parent node
		delete(nodeArray[rightNum]);	// delete the index of nodeArray at rightNum
	}

	bytesIn += 510;

	return treeOrder;	// return the string treeOrder
}

string Huffman::outputFileCheck(string inputFile, string outputFile, string type1, string type2)
{

	if (outputFile == "") outputFile = inputFile.substr(0, inputFile.find_last_of('.')) + type1;
	else if (outputFile.find(type1) == string::npos && outputFile.find(type2) == string::npos)
	{
		if (type2 == "")cout << "Output file name enterted does not contain " + type1;
		else cout << "Output file name enterted does not contain " + type1 + "or" + type2;
		exit(1);
	}

	return outputFile;
}


int Huffman::findLowest(int firstLowestIndex)
{
	unsigned int lowestIndex = 256; // set the number of intances to the max character 
	unsigned int lowIndWeight = numeric_limits<int>::max();
	//node* lowestNode = new node();
	//lowestNode->numberOfInstances = numeric_limits<unsigned int>::max(); 

	for (int i = 0; i < 256; i++)	// iterates through the entire array
	{
		if (i != firstLowestIndex && nodeArray[i] != nullptr && nodeArray[i]->numberOfInstances < lowIndWeight)	// if the nodeArray at i is not nullptr and 
		{																										// that node's, at num i, weight is less than the lowest nodes weight
			lowIndWeight = nodeArray[i]->numberOfInstances;														// set that node's weight to lowIndWeight
			lowestIndex = i;																					// and set the node's idex to lowestIndex
		}
	}

	return lowestIndex;
}


// Takes the inputfile given by the user and reads it in char by char to fill the ascii array by number of instances
string Huffman::fillArray(string inputFile)
{
	char character = NULL;
	ifstream inFile;

	inFile.open(inputFile, ios::binary); // Obtain's file from specified location based on the input entered by user

	if (!inFile)	// If the file isn't found print out the file was not found and exit program
	{
		cout << "The file was not found.";
		exit(1);
	}

	while (inFile.get(character))				// Enter the loop and continue to iterate through as long as the file still contains a char as the next char to be read in
	{
		freqTable[(unsigned char)character] += 1;	// increment the instance of character taken from file
	}
	inFile.close();	// close the file

	if (character == NULL) // if the character is null then the file had nothing inside print out file empty and exit code
	{
		cout << "File Empty";
		exit(1);
	}

	for (int i = 0; i < 256; i++) // fill node array with instances
	{
		nodeArray[i]->numberOfInstances = freqTable[i];
	}

	return combineNodes();
}

// called after fillArray(). Loops though finding the two lowest weighted index's. Once found
// their weights are combined and put into a node. The first lowest and second lowest become
// the left and right child respectively into the node with their combine weights.
string Huffman::combineNodes()
{
	string treeOrder = "";						// keeps track of the pairs made

	int firstLNum = findLowest();				// find's the first lowest
	int secondLNum = findLowest(firstLNum);		// find's the second lowest

	while (secondLNum != 256)	// while secondLnum isn't equal to 256, set term in lowest if no other lowest is found continue through the loop.
	{
		node* parent = new node();		// intializing new nodes 
		node* firstLNode = new node();
		node* secondLNode = new node();

		if (secondLNum < firstLNum)  // if secondLNum is lower than first number switch them
		{
			int swap = firstLNum;
			firstLNum = secondLNum;
			secondLNum = swap;
		}

		treeOrder = treeOrder + (char)firstLNum + (char)secondLNum; // enter the pair order in treeOrder string

		firstLNode->character = nodeArray[firstLNum]->character;	// set the firstLNode's attributes equal to the node array's index at firstLNum
		firstLNode->numberOfInstances = nodeArray[firstLNum]->numberOfInstances;
		firstLNode->LCH = nodeArray[firstLNum]->LCH;
		firstLNode->RCH = nodeArray[firstLNum]->RCH;

		secondLNode->character = nodeArray[secondLNum]->character; // set the secondLNode's attributes equal to the node array's index at secondLNum
		secondLNode->numberOfInstances = nodeArray[secondLNum]->numberOfInstances;
		secondLNode->LCH = nodeArray[secondLNum]->LCH;
		secondLNode->RCH = nodeArray[secondLNum]->RCH;

		parent->numberOfInstances = firstLNode->numberOfInstances + secondLNode->numberOfInstances; //set parent's weight to firstLNode weight + secondLnodes weight
		parent->LCH = firstLNode;	// Parents left child becomes firstLNode
		parent->RCH = secondLNode;	// Parents right child becomes secondLNode

		nodeArray[firstLNum] = parent;		// firstLNum index in nodeArray becomes the parent node
		nodeArray[secondLNum] = nullptr;	// while the secondLNum index becomes null

		firstLNum = findLowest();				// refind first lowest
		secondLNum = findLowest(firstLNum);		// refind second lowest
	}

	return treeOrder;	// return the treeOrder string
}

void Huffman::methodTime(clock_t start, clock_t end)
{
	double timeElapsed = ((double)end - (double)start) / double(CLOCKS_PER_SEC);

	cout << "Time: " << timeElapsed << " seconds. " << bytesIn << " bytes in / " << bytesOut << " bytes out";
}