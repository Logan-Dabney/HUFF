#pragma once
#include <string>
#include <time.h>

using namespace std;

class Huffman {

public:

	Huffman();
	~Huffman();
	string DisplayHelp();											// Display's a help screen with information on how to use HUFF.exe 
	void MakeTreeBuilder(string inputFile, string outputFile = "");		// Builds a tree based on inputfile
	void EncoderFile(string inputFile, string outputFile = "");			// Ecodes/compresses a file 
	void DecoderFile(string inputFile, string outputFile = "");			// Decodes a compressed file 
	void EncoderFileWithTree(string inputfile, string treeFile, string outputFile = "");	// Encodes a file based on a preiously made tree


private:
	struct node 
	{
		unsigned int numberOfInstances = 0;
		unsigned char character;
		node* LCH = nullptr;
		node* RCH = nullptr;
	};
	//node* root = nullptr;

	unsigned int freqTable[256] = {};
	node* nodeArray[256] = {};
	string encodingMap[256] = {};
	string binaryString = "";
	string longestPath = "";
	long long bytesIn = 0;
	long long bytesOut = 0;


	string fillArray(string inputFile);
	string createTreeFromFileOrPairString(string treeFile = "", string treeOrder = "");
	string outputFileCheck(string inputFile, string outputFile, string type1, string type2 = "");
	void writeToFileE(string inputFile, string outputFile, string treeOrder = "");
	void writeToFileD(string inputFile, string outputFile);
	void buildEncodingMap(node* p);
	int findLowest(int firstLowestIndex = -1);
	string combineNodes();
	void methodTime(clock_t start, clock_t end);
};