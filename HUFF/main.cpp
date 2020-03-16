// HUFF.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//==============================================================================================
// main.cpp - Huffman tree main
//​
// It instantiates an huffman tree (implemented in a separate class) contains visible methods
// MakeTreeBuilder, EncoderFile, EcoderFileWithTree, DecoderFile and DisplayHelp.
//​
// Author:    Logan Dabney, University of Toledo​
// Date:      Mar 1, 2020​
// Copyright: Copyright 2020 by Logan Dabney. All rights reserved.
//==============================================================================================

#include <iostream>
#include "Huffman.h"
//#include <ifstream>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc > 1 && (string)argv[1] == "HUFF")	// checks to see if the second value in argv is HUFF because the first will always be HUFF.exe
	{
		Huffman* tree = new Huffman();			// if it is create an instance of the huffman class

		if ((string)argv[2] == "-t")			// if the third value of argv is -t create the tree for given file and output
		{
			if (argc == 4) tree->MakeTreeBuilder((string)argv[3]);							// if there is only an input file
			else if (argc == 5) tree->MakeTreeBuilder((string)argv[3], (string)argv[4]);	// if there is both and input file and output file
			else cout << "Did not properly enter -d command! Use -help, -h, -? for help.";	// other wise print out error
		} 
		else if ((string)argv[2] == "-e")		// if the third value of argv is -e encode a file
		{
			if (argc == 4) tree->EncoderFile((string)argv[3]);								// if the count is 4 only an input file has been enter
			else if (argc == 5) tree->EncoderFile((string)argv[3], (string)argv[4]);		// if the count is 5 an input and a output file has been entered
			else cout << "Did not properly enter -d command! Use -help, -h, -? for help.";	// other wise print out error
		}
		else if ((string)argv[2] == "-et")		// if the third value of argv is -e encode a file with a predetermined tree file
		{
			if (argc == 5) tree->EncoderFileWithTree((string)argv[3], (string)argv[4]);							// if the count is 5 only the input file and tree file have been entered
			else if (argc == 6) tree->EncoderFileWithTree((string)argv[3], (string)argv[4], (string)argv[5]);	// if the count is 6 the input, tree and output file have been entered
			else cout << "Did not properly enter -et command! Use -help, -h, -? for help."; // other wise print out error
		}
		else if ((string)argv[2] == "-d")		// if the third value of argv is -d decode a file 
		{
			if (argc == 5) tree->DecoderFile((string)argv[3], (string)argv[4]);	// if the count is 5 the input and outputfile has been entered
			else cout << "Did not properly enter -d command! Use -help, -h, -? for help."; // other wise print out error
		}
		else if ((string)argv[2] == "-help" || (string)argv[2] == "-?" || (string)argv[2] == "-h") 
		{
			tree->DisplayHelp(); // if the third value of argv is -help, -? or -h display help screen
		}
	}
}