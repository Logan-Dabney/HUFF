//==============================================================================================
// File: Huffman.cpp - Huffman tree implementation
// c.f.: Huffman.h
//​
// This code impelements the huffman tree and the uses of a huffman tree. The tree is built 
// from the node that is declared in Huffman.h; it contains numberOfIntances, character, 
// LCH, and RCH (For further explination of the node look at Huffman.h. The huffman, in this
// code, is used to encode/compress a file using or not using a tree file, decode/decompress a
// file and make a tree file. 
//​
// Author:    Logan Dabney, University of Toledo​
// Date:      Mar 1, 2020​
// Copyright: Copyright 2020 by Logan Dabney. All rights reserved.
//==============================================================================================


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
	delete[] &freqTable;
	delete[] &encodingMap;
}

// Displays the functionality of the huffman program in easily readable text
void Huffman::DisplayHelp()
{
	cout << "HUFF -operation [depends on operation]" << endl;
	cout << "=====================================================================================" << endl;
	cout << "Operations: -e, -d, -t, -et, -help, -?, -h" << endl;
	cout << "=====================================================================================" << endl;
	cout << "Operation Explanation			[] - optional input" << endl;
	cout << "-e file1 [file2]			  : Encodes file1 and places the out into file2. If no " << endl;
	cout << "								file2 is provided it will take the name of file1 and" << endl;
	cout << "								append .enc to it" << endl;
	cout << "								at the end of file name if used" << endl;
	cout << "-d file1 file2				  : Decodes file1 and places it into file2" << endl;
	cout << "-t file1 [file2]			  : Creates a tree builder file from the input file1 and" << endl;
	cout << "								and places it into file2. If no file2 is inputed it" << endl;
	cout << "								take file1's name and append .htree" << endl;
	cout << "-et file1 file2 [file3]	  : Encodes file1 using the tree builder file2 and places" << endl;
	cout << "								the output in file3. If no file3 is inputed it takes" << endl;
	cout << "								the name of file1 and appends .enc" << endl;
	cout << "-help or -? or -h			  : Displays this screen again.";
}

// This method is used to create an huffan tree and create a file for it. It is passed an input
// file and an optional output file. THe input file is used to fill the nodeArray and build the 
// the huffman tree. Which is returned and put into the output file
void Huffman::MakeTreeBuilder(string inputFile, string outputFile)
{
	clock_t start = clock();	// records the starting time
	ofstream treeFile;			// creating output stream
	string treeOrder;			// creates the treeOrder string

	outPathEqualsInPathTest(inputFile, outputFile);														// tests the equality of the two strings

	treeFile.open(outputFileCheck(inputFile, outputFile, ".htree", ".510"), ios::out | ios::binary);	// opens outputFile in binary after checking it with outputFileCheck

	fileOpeningTest(!treeFile, outputFile);			// tests if the file opened

	treeOrder = buildTree(inputFile);				// input file is sent to fillArray and a string of node combinations used to make the huffman tree is returned

	bytesIn = nodeArray[0]->numberOfInstances;		// adds the total weight of the huffman tree to the bytes in because each intances is a byte that was read in
	bytesOut = 510;

	treeFile << treeOrder;			// add the treeOrder to the treeFile
	treeFile.close();				// close the treeFile
	methodTime(start, clock());		// prints out the total time taken, bytes in and bytes out
}

// This function is used to compress/encode an input file with a separate tree as the one that would be created
// from the input file. It is printed to the output file that would be entered by the user. If nothing is entered
// it will take the inputfile name and tack on .huf at the end of it for the output file. It also handles a variety 
// of errors that could happen like, unable to find inputfile, incomplete tree file, etc.
// Along with the output file this method returns the time taken, bytes in and bytes out.
void Huffman::EncoderFileWithTree(string inputFile, string treeFile, string outputFile)
{
	clock_t start = clock();		// records the starting time

	outPathEqualsInPathTest(inputFile, outputFile);		// tests the equality of the two strings

	writeToFileE(inputFile, outputFile, createTreeFromFileOrPairString(treeFile));	// used to write to the output file for both the encoding with tree and regular encoding
																					// this function calls createTreeFromFile which creates the huffman tree and returns the treeOrder string
	methodTime(start, clock());		// prints out the total time taken, bytes in and bytes out
}

// This function is used to compress/encode an input file into a designated output file. It handles a variety
// of errors that could occur when using this method. Along with outputing a file as a result of the encoding proccess
// it records the time, bytes in and bytes out then outputs it for the user to see.
void Huffman::EncoderFile(string inputFile, string outputFile)
{
	clock_t start = clock();							// records the starting time

	outPathEqualsInPathTest(inputFile, outputFile);		// tests the equality of the two strings

	writeToFileE(inputFile, outputFile);				// does all computations and writes the encoded text to the outputFile

	methodTime(start, clock());							// prints out the total time taken, bytes in and bytes out
}

// This function is used to decompress/decode a preiously encoded file. The file must have been encoded previously by
// the huffman process if it is not this method will not work. An input and output file must be provided for it to work.
// This method hnadles a variety of errors that could occur during the process some being the aformentioned requirements.
// Along with the output file this method returns the time taken, bytes in and bytes out.
void Huffman::DecoderFile(string inputFile, string outputFile)
{
	clock_t start = clock();							// records the starting time

	outPathEqualsInPathTest(inputFile, outputFile);		// tests the equality of the two strings

	writeToFileD(inputFile, outputFile);				// does all computations and writes the decoded text to the outputFile

	methodTime(start, clock());							// prints out the total time taken, bytes in and bytes out
}

// This function is used by EncoderFile and EncoderFileWithTree to encode and write to an output file. It's
// parameters are needed: inputfile string, optional: outputfile string and a treeorder string. 
// Along with the output file this method returns the time taken, bytes in and bytes out.
void Huffman::writeToFileE(string inputFile, string outputFile, string treeOrder)
{
	string buffer = "";		// creates/intializes buffer string
	ifstream input;			// creates the ifstream for the inputFile
	ofstream output;		// creates the ofstream for the outputFile

	if (treeOrder == "") treeOrder = buildTree(inputFile); // if the treeOrder is not provided use the buildTree function to create it

	buildEncodingMap(nodeArray[0]); // once the tree is created build the encoding map with the buildEncodingMap funciton

	output.open(outputFileCheck(inputFile, outputFile, ".huf", ".enc"), ios::out | ios::binary);	//opens outputfile after checking it with outputFileCheck function
	input.open(inputFile, ios::binary);																// open the inputFile 

	fileOpeningTest(!input, inputFile);				// tests to see if the file opened
	fileOpeningTest(!output, outputFile);			// tests to see if the file opened

	if (input.peek() == -1)							// tests to see if inputFile is empty
	{
		cout << inputFile << " is empty!";
		exit(1);
	}

	output << treeOrder;	// since this is encoding a file the tree will be needed to decode it so send the treeOrder to the output
	bytesOut += 510;		// increment the bytesOut by 510

	while (input.peek() != -1)						// while the file still contains characters
	{
		unsigned char character = input.get();		// create an unsigned char charater and intialize it to the next char in the input file
		buffer += encodingMap[character];			// set the buffer string to the index of the ascii value of character
		bytesIn++;									// increment the bytesIn by 1

		while (buffer.length() >= 8)				// while the buffer length is greater than or eqaul to 8 (one byte) continue to loop
		{
			char b = 0;								// set the character b to 0 so that all 8 bits are 0

			if (buffer[0] == '1') b |= (1 << 7);	// sets b to the first 8 spots of buffer (1 or 0)
			if (buffer[1] == '1') b |= (1 << 6);	// done by checking to see if it's one. If so
			if (buffer[2] == '1') b |= (1 << 5);	// set that place in b to 1 otherwise leave it as 
			if (buffer[3] == '1') b |= (1 << 4);	// 0. This will create a unique character
			if (buffer[4] == '1') b |= (1 << 3);
			if (buffer[5] == '1') b |= (1 << 2);
			if (buffer[6] == '1') b |= (1 << 1);
			if (buffer[7] == '1') b |= (1 << 0);
			buffer.erase(0, 8);						// erase the first 8 bits in buffer

			output << b;							// output new unique chracter b to output
			bytesOut++;								// increment bytesOut by 1
		}
	}


	if (buffer.length() < 8 && buffer.length() > 0)					// if the buffer still has 1 or 0's in it need to find a padding bits
	{
		char b = 0;													// set b = 0 so all bits are 0
		buffer += longestPath.substr(0, 8 - buffer.length());		// find the padding bits from the longest path by subtracting buffer's length from 8 and taking the 
																	// those bits from the front of buffer 
		if (buffer[0] == '1') b |= (1 << 7);
		if (buffer[1] == '1') b |= (1 << 6);
		if (buffer[2] == '1') b |= (1 << 5);
		if (buffer[3] == '1') b |= (1 << 4);
		if (buffer[4] == '1') b |= (1 << 3);
		if (buffer[5] == '1') b |= (1 << 2);
		if (buffer[6] == '1') b |= (1 << 1);
		if (buffer[7] == '1') b |= (1 << 0);
		buffer.erase(0, 8);

		output << b;
		bytesOut++;
	}

	input.close();		// closes the input file
	output.close();		// closes the output file
}

// This method is called by the decoder. It is used to write to the file while also using the encoderMap to decode
// the incoming file. 
// It handles a couple of errors: decodefile node opening and an incomplete tree builder. 
// Along with the output file this method returns the time taken, bytes in and bytes out.
void Huffman::writeToFileD(string inputFile, string outputFile)
{
	ofstream output;		// creating an ofstream to write output to
	ifstream input;			// creating an ifstring to read input from
	char character;			// creating a char named character
	string treeOrder;		// creating a string named treeOrder to hold the tree builder string from file
	node* p;				// creating a node pointer , p, to used to traverse tree. 

	fileTypeTest(inputFile, ".huf", ".enc");			// tests if the inputFile is the correct type of file

	output.open(outputFile, ios::out | ios::binary);	// opens the output file in binary mode
	input.open(inputFile, ios::in | ios::binary);		// opens the input file in binary mode

	fileOpeningTest(!input, inputFile);					// tests to see if the file opened
	fileOpeningTest(!output, outputFile);

	for (int i = 0; i < 510; i++)						// this for loop is used to read in the tree builder string
	{
		if (input.peek() != -1)							// if the inputfile still conatins a character continue to execute 
		{
			input.get(character);						// get the next character in the file and puts in character
			treeOrder += (unsigned char)character;		// adds the charater to the treeOrder string
		}
		else											// OTHERWISE exit loop
		{
			cout << "Incomplete encoded file. Does not contain full huffman pair string.";
			exit(1);
		}
	}

	createTreeFromFileOrPairString("", treeOrder);	// create the huffman tree from the created treeOrder string
	buildEncodingMap(nodeArray[0]);					// once the tree is created build the encodingMap 

	p = nodeArray[0];	// set the pointer equal the nodeArray[0] (Huffamn root) 

	while (input.get(character)) // while the inputfile still contains characters execute the following code
	{
		string buffer = "00000000";						// set the buffer to all zeros allows for easy changing of values
		unsigned int byte = (unsigned char)character;	// create and unsigned byte from character by casting character and an unsigned char to get the positive ascii value
		bytesIn++;										//increment the bytesIn because each char read in is a byte

		// used to turn the integer byte into a binary code of 8 bits by check to see if each bit is a one then changing buffer 
		// at that bit location to 1
		if ((byte >> 7 & 1) == 1) buffer[0] = '1';
		if ((byte >> 6 & 1) == 1) buffer[1] = '1';
		if ((byte >> 5 & 1) == 1) buffer[2] = '1';
		if ((byte >> 4 & 1) == 1) buffer[3] = '1';
		if ((byte >> 3 & 1) == 1) buffer[4] = '1';
		if ((byte >> 2 & 1) == 1) buffer[5] = '1';
		if ((byte >> 1 & 1) == 1) buffer[6] = '1';
		if ((byte >> 0 & 1) == 1) buffer[7] = '1';

		for (int i = 0; i < 8; i++)		// this loop is used to traverse through the huffman tree. It is always set to 8
		{								// because a byte put into buffer will never be larger that 8 bits
			if (buffer[i] == '1') p = p->RCH;				// if the index of buffer is a one set pointer to it's RCH
			if (buffer[i] == '0') p = p->LCH;				// if the index of buffer is a one set pointer to it's LCH
			if (p->LCH == nullptr && p->RCH == nullptr)		// if the pointer is a leaf
			{
				output << (char)p->character;			// write the character to the outputFile
				p = nodeArray[0];							// reset the pointer to the root to start again

				bytesOut++;									// increment bytesOut because each character out is byte
			}
		}
	}

	input.close();	// close the input file
	output.close();	// close the output file
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

// This function is used to create a huffman tree from a .htree file or an already made treeOrdered string. It reads in the .htree file into a treeOrder string
// using tree order it combines from the intialized nodeArray in the order as specified by the string. This works because
// instances are not needed for anything beisdes finding the order of combining nodes. Since the ordwer is obtained from the 
// file entered by the user or already made pair string from file.
// This function also handles errors like: no .htree file found and incomplete htree file
string Huffman::createTreeFromFileOrPairString(string treeFile, string treeOrder)
{
	char character;			// creating a char value named character
	//string treeOrder;		// creating the string treeOrder to read the file into
	ifstream treeInFile;	// creating the ifstream used to access the file

	if (treeFile != "") // if the treeFile's name is nothing then excute the following code
	{
		fileTypeTest(treeFile, ".htree", ".510");			// tests if the tree file has the correct file type

		treeInFile.open(treeFile, ios::in | ios::binary);	// opens the .htree file in binary mode
		fileOpeningTest(!treeInFile, treeFile);				// tests to see if the file can open

		while (treeInFile.get(character))			// while another character can be returned the file and put into character continue loop
		{
			treeOrder += (unsigned char)character;	// add the character to treeOrder string as an unsigned char
		}
	}

	if (treeOrder.length() != 510)	// if the string is no the length of 510 it is either incomplete or to large. Exit with code 1
	{
		cout << "Trees' order pairs was an incomplete or too larger";
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

		parent->LCH = left;				// parents left child becomes left
		parent->RCH = right;			// parents right child become right

		nodeArray[leftNum] = parent;	// set the index of nodeArray at leftNum = to the parent node
		delete(nodeArray[rightNum]);	// delete the index of nodeArray at rightNum
	}

	bytesIn += 510;		// add 510 to bytes in because 510 bytes were read in to create treeOrder

	return treeOrder;	// return the string treeOrder
}

// This funciton is called by combineNodes() to find the index of nodeArray with the lowest weight
// if no index is found it returns 256 because the node array has an index of 0 - 255
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
string Huffman::buildTree(string inputFile)
{
	char character;
	ifstream inFile;

	inFile.open(inputFile, ios::binary);			// Obtain's file from specified location based on the input entered by user

	fileOpeningTest(!inFile, inputFile);			// tests to see if the file opens

	if (inFile.peek() == -1)						// checks if the file is empty
	{
		cout << inputFile << " is empty!";
		exit(1);
	}

	while (inFile.get(character))					// Enter the loop and continue to iterate through as long as the file still contains a char as the next char to be read in
	{
		freqTable[(unsigned char)character] += 1;	// increment the instance of character taken from file
	}
	inFile.close();	// close the file

	//if (character == NULL) // if the character is null then the file had nothing inside print out file empty and exit code
	//{
	//	cout << "File Empty";
	//	exit(1);
	//}

	for (int i = 0; i < 256; i++) // fill node array with instances
	{
		nodeArray[i]->numberOfInstances = freqTable[i];
	}

	return combineNodes(); // combines the nodes of nodeArray and returns the tree ordered pair
}

// called in buildTree(). Loops though finding the two lowest weighted index's. Once found
// their weights are combined and put into a node. The first lowest and second lowest become
// the left and right child respectively into the node with their combine weights.
string Huffman::combineNodes()
{
	string treeOrder = "";						// keeps track of the pairs made

	int firstLNum = findLowest();				// find's the first lowest
	int secondLNum = findLowest(firstLNum);		// find's the second lowest

	while (secondLNum != 256)					// while secondLnum isn't equal to 256, set term in lowest if no other lowest is found continue through the loop.
	{
		node* parent = new node();				// intializing new nodes 
		node* firstLNode = new node();
		node* secondLNode = new node();

		if (secondLNum < firstLNum)				// if secondLNum is lower than first number switch them
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

		secondLNode->character = nodeArray[secondLNum]->character;	// set the secondLNode's attributes equal to the node array's index at secondLNum
		secondLNode->numberOfInstances = nodeArray[secondLNum]->numberOfInstances;
		secondLNode->LCH = nodeArray[secondLNum]->LCH;
		secondLNode->RCH = nodeArray[secondLNum]->RCH;

		parent->numberOfInstances = firstLNode->numberOfInstances + secondLNode->numberOfInstances; //set parent's weight to firstLNode weight + secondLnodes weight
		parent->LCH = firstLNode;	// Parents left child becomes firstLNode
		parent->RCH = secondLNode;	// Parents right child becomes secondLNode

		nodeArray[firstLNum] = parent;			// firstLNum index in nodeArray becomes the parent node
		nodeArray[secondLNum] = nullptr;		// while the secondLNum index becomes null

		firstLNum = findLowest();				// refind first lowest
		secondLNum = findLowest(firstLNum);		// refind second lowest
	}

	return treeOrder;	// return the treeOrder string
}


// This method is used by the 4 main operations of the huffprogram to output total time taken to finish the operation, the amount of bytes read in and out.
// It takes in a start time and a end time. Doing simple calculations it find the time elapsed and outputs it along with the bytes read in and outputed.
// which are calucalted earlier in the code.
void Huffman::methodTime(clock_t start, clock_t end)
{
	double timeElapsed = ((double)end - (double)start) / double(CLOCKS_PER_SEC);								// find the timeElapsed by subracting end from start and dividing it by the clcoks per second

	cout << "Time: " << timeElapsed << " seconds. " << bytesIn << " bytes in / " << bytesOut << " bytes out";	// outputed string
}

// This function is called by a variety of methods to see if the file was able to open. 
// If the file was unable to ope it will print out the path that was tring to be accessed
// and exit the code
void Huffman::fileOpeningTest(bool openTest, string file)
{
	if(openTest)									// if the openTest is true then the file was unable to be opened
	{
		cout << file << " could not be opened!";	// print out the file path
		exit(1);									// and exit the code
	}
}

// This function is also called by a variety of functions to test if the input path and the outpath 
// are the same. If they are print the error and exit the code.
void Huffman::outPathEqualsInPathTest(string inputPath, string outputPath)
{
	if (outputPath == inputPath)										// path string equality test
	{
		cout << "Output can not refer to the same path as the input";	// error to be printed
		exit(1);														// exit the code
	}
}

// This function is used in createTreeFromFileOrPairString and writeToFileD to test if the inputed file
// is the right type. If note produce error and exit code
void Huffman::fileTypeTest(string inputFile, string type1, string type2)
{
	string fileType = inputFile.substr(inputFile.find_last_of('.'), inputFile.length() - 1); // gets the type of file from inputFile

	if (fileType != type1)													// if the first type doesn't match
	{
		if (type2 == "")													// and the type2 is nothing
		{
			cout << inputFile << " is not the correct type of file!";		// then produce error and exit
			exit(1);
		}
		else if (fileType != type2)											// or the type2 is not null but the types still do not match
		{
			cout << inputFile << " is not the correct type of file!";		// produce error and exit
			exit(1);
		}
	}
}

// This function is used to check the output file if it's in the correct format or if one was entered.
// If nothing was entered the outputfile is created from the path for the inputfile but with
// type1 instead of the input files type. If one is entered with an incorrect type it will
// produce an error and exit
string Huffman::outputFileCheck(string inputFile, string outputFile, string type1, string type2)
{
	string inputFileName;		// creates inputFileName as a string 
	string fileType = "";		// creates fileType as a string and intializes it to "" because if no file type is found then its nothing ie ""
	string outputFileName;		// creates outputFileName as a string

	if (outputFile == "")		// if the outputFile was not entered
	{
		if (inputFile.find('\\') != string::npos) inputFileName = inputFile.substr(inputFile.find_last_of('\\') + 1, inputFile.length() - 1);	// sets the inputFileName to right after the last \ if \ exist
		else inputFileName = inputFile;																											// otherwise it's just inputFile

		if (inputFileName.find('.') == string::npos) return inputFile + type1;		// if there is no . in inputFileName then return the inputFile with type1 on the end

		return inputFile.substr(0, inputFile.find_last_of('.')) + type1;			// otherwise remove the type on inputFile and return the inputFile with type1 on the end 
	}

	if (outputFile.find('\\') != string::npos) outputFileName = outputFile.substr(outputFile.find_last_of('\\') + 1, outputFile.length() - 1);	// sets the outputFileName to right after the last \ if \ exist
	else outputFileName = outputFile;																											// otherwise it's just outputFile

	if (outputFileName.find('.') != string::npos) fileType = outputFile.substr(outputFile.find_last_of('.'), outputFile.length() - 1);			// if outputFilename contain . then put the type in fileType otherwise it stays ""

	if (fileType != type1)																		// if the fileType is not equal to type1
	{
		if (fileType != type2)																	// and it's not equal to type2
		{
			cout << "Output file name enterted does not contain " + type1 + " or " + type2;		// then print out error and exit
			exit(1);
		}
	}

	return outputFile; // otherwise return outputFile because it is correct 
}

//if (outputFile == "") treeFile.open(inputFile.substr(0, inputFile.find_last_of('.')) + ".htree", ios::out | ios::binary);	// if the file is nothing take the input file name and tack on .htree
//else if (outputFile.find(".htree") == string::npos && outputFile.find(".510") == string::npos)								// if the output file doesn't contain .510 or .htree exit code
//{
//	cout << "Output file name enterted does not contain htree or 510";
//	exit(1);
//}
//else