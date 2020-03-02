// HUFF.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Huffman.h"
//#include <ifstream>

using namespace std;

int main(int argc, char* argv[])
{

	Huffman* tree = new Huffman();
	string temp;
	cin >> temp;

	tree->MakeTreeBuilder(temp, "");



	// open input file stream 
	// use get method to read one char at a time
	//do
	//{
	//for (int i = 0; i < argc; i++)
	//{
	//	cout << argv[i] << endl;
	//	if (argv[i] == "HUFF")
	//	{
	//		cout << "Shiitty";
	//	}
	//	else if (argv[i] == "")
	//	{

	//	}
	//	else if (argv[i] == "")
	//	{

	//	}
	//	else if (argv[i] == "")
	//	{

	//	}
	//}
	//} while (cin.get() != '\n');
	//{
	//	return 0;
	//}
}