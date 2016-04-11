/******************************************************************************/
/*!
\file   FileIO.h
\author Unni Nair
\par    email: u.nair@digipen.edu
\par    KDTree
\date   03/04/2016

*/
/******************************************************************************/


/******************************************************************************/
/*!
\class FileIO
\brief
FileIO is a singleton class which ensures that only one instance will be there through out the program's life cycle.
This class is responsible for all file read/write funtions.
Following the coding style of Resource Acquisition Is Initialization (RAII) destructor ensures the file is closed in all possible scenario.

Operations include:

- Read a file given the file name
- write data on to a file given the file name and extension of choice

*/
/******************************************************************************/

#pragma once
#include <string>  //for std::string operations
#include <vector>  //a very useful data structures which grows dynamically
#include <fstream> //for all file related operations

class FileIO
{
public:
	const std::vector<std::string> readFile(const std::string& filename);
	bool openFiletoWrite(const std::string& fileName, const std::string& ext, const std::vector<std::string>& data);
	static FileIO& getInstance();
private:
	FileIO();
	~FileIO();
	//! port will take care of all the file operations, both read and write.
	std::fstream port;
};

