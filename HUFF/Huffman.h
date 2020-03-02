#pragma once
#include <string>

using namespace std;

class Huffman {

public:

	Huffman();
	~Huffman();
	string DisplayHelp();											// Display's a help screen with information on how to use HUFF.exe 
	void MakeTreeBuilder(string inputFile, string outputFile);		// Builds a tree based on inputfile
	void EncoderFile(string inputFile, string outputFile);			// Ecodes/compresses a file 
	void DecoderFile(string inputFile, string outputFile);			// Decodes a compressed file 
	void EncoderFileWithTree(string inputfile, string outputFile);	// Encodes a file based on a preiously made tree


private:
	struct node 
	{
		int numberOfInstances = 0;
		unsigned char character;
		node* LCH = nullptr;
		node* RCH = nullptr;
	};
	//node* root = nullptr;

	node* asciiArray[256] = {};

	void fillArray(string inputFile);
	node* findLowest();
};