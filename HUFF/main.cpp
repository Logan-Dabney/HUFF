// HUFF.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Huffman.h"
//#include <ifstream>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc > 1 && (string)argv[1] == "HUFF")	// checks to see if the second value in argv is HUFF because the first will always be HUFF.exe
	{
		Huffman* tree = new Huffman();			// if it is create an instance of the huffman class

		if ((string)argv[2] == "-t")			// if the third value of argsv is -t create the tree for given file and output
		{
			if (argc < 5) tree->MakeTreeBuilder((string)argv[3]);		// if there is only an input file
			tree->MakeTreeBuilder((string)argv[3], (string)argv[4]);	// if there is both and input file and output file
		} 
		else if ((string)argv[2] == "-e")
		{
			if (argc < 5) tree->EncoderFile((string)argv[3]);
			tree->EncoderFile((string)argv[3], (string)argv[4]);
		}
		else if ((string)argv[2] == "-et")
		{
			if (argc < 6) tree->EncoderFileWithTree((string)argv[3], (string)argv[4]);
			tree->EncoderFileWithTree((string)argv[3], (string)argv[4], (string)argv[5]);
		}
		else if ((string)argv[2] == "-d") 
		{
			if (argc < 5) tree->DecoderFile((string)argv[3]);
			tree->DecoderFile((string)argv[3], (string)argv[4]);
		}
	}
	//Huffman* tree = new Huffman();
	//string temp;
	//cin >> temp;

	//tree->MakeTreeBuilder(temp, "");



	 //open input file stream 
	 //use get method to read one char at a time


	/*do
	{
	for (int i = 0; i < argc; i++)
	{
		if ((string)argv[i] == "HUFF")
		{
			cout << "Shiitty";
		}
		else if (argv[i] == "")
		{

		}
		else if (argv[i] == "")
		{

		}
		else if (argv[i] == "")
		{

		}
	}
	} while (cin.get() != '\n');
	{
		return 0;
	}*/
}