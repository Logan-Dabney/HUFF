#pragma once
#include <string>
#include <time.h>

using namespace std;

class Huffman {

public:

	Huffman();
	~Huffman();
	void DisplayHelp();																		// Display's a help screen with information on how to use HUFF.exe 
	void MakeTreeBuilder(string inputFile, string outputFile = "");							// Builds a tree based on inputfile
	void EncoderFile(string inputFile, string outputFile = "");								// Ecodes/compresses a file 
	void DecoderFile(string inputFile, string outputFile);									// Decodes a compressed file 
	void EncoderFileWithTree(string inputfile, string treeFile, string outputFile = "");	// Encodes a file based on a preiously made tree


private:
	struct node									// structure node for nodes in the huffman tree
	{
		unsigned int numberOfInstances = 0;		// this is the amount of times a character is read in (weight)
		unsigned char character;				// the character attached to the node
		node* LCH = nullptr;					// left child of the node
		node* RCH = nullptr;					// right child of the node
	};

	unsigned int freqTable[256] = {};			// counts the frequaency for each of the 256 ascii values
	node* nodeArray[256] = {};					// holds the nodes for each of the 256 ascii values
	string encodingMap[256] = {};				// holding the encoding map for each of the 256 ascii values 
	string binaryString = "";					// for the building the ecoding map
	string longestPath = "";					// holds the longest path found when building the encoding map 
	long long bytesIn = 0;						// holds the amount of bytes read in
	long long bytesOut = 0;						// holds the amount of bytes that were written to an output file


	string buildTree(string inputFile);															// builds the huffman tree with along with the function combineNodes()
	string createTreeFromFileOrPairString(string treeFile = "", string treeOrder = "");			// creates the huffman tree from a provided tree file or a tree builder string (treeOrder string)
	void writeToFileE(string inputFile, string outputFile, string treeOrder = "");				// writes to an output file for encoding
	void writeToFileD(string inputFile, string outputFile);										// writes to an output file for decoding
	void buildEncodingMap(node* p);																// builds the encoding map
	int findLowest(int firstLowestIndex = -1);													// finds the lowest weighted index for combineNodes()
	string combineNodes();																		// combines nodes on a loop till the huffman tree is made
	void methodTime(clock_t start, clock_t end);												// prints out the total time taken along with the byte's in and out
	void fileOpeningTest(bool openTest, string file);											// tests to see if a file was able to open
	void outPathEqualsInPathTest(string inputPath, string outputPath);							// tests to see if the output and inputfile refer to the same file
	void fileTypeTest(string inputFile, string type1, string type2);							// tests to see if the file entered is the correct type
	string outputFileCheck(string inputFile, string outputFile, string type1, string type2);	// tests to see if the output file is the correct type
	node* postTraversal(node* p);
};