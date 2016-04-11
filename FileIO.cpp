/******************************************************************************/
/*!
\file   FileIO.h
\author Unni Nair
\par    email: u.nair@digipen.edu
\par    KDTree
\date   03/04/2016

*/
/******************************************************************************/

#include "FileIO.h"
#include <iostream>

/******************************************************************************/
/*!

Reads the file named "filename". Returns a data in string format using the vector container. It gives an empty container if failed to open the file.

*/
/******************************************************************************/
const std::vector<std::string> FileIO::readFile(const std::string& filename)
{
	std::vector<std::string> lines;
	port.open(filename.c_str(), std::ios::in | std::ios::binary);
	if (port.fail())
	{
		std::cout << "Failed to open file" << " " << filename << std::endl;
		return lines;
	}
	std::string eachline;
	while (std::getline(port, eachline))
	{
		lines.push_back(eachline);
	}
	port.close();
	return lines;
}


/******************************************************************************/
/*!

Opens a file with the given filename and extension. It copies the data to the opened file. Retrun true if succeeds and false if failed to open the file

*/
/******************************************************************************/

bool FileIO::openFiletoWrite(const std::string& fileName, const std::string& ext, const std::vector<std::string>& data)
{
	
	port.open((fileName + ext).c_str(), std::ios::out | std::ios::binary);
	if (port.fail())
	{
		std::cout << "Failed to create file" << " " << fileName + ext << std::endl;
		return false;
	}
	std::vector<std::string>::const_iterator iter = data.begin();
	while (iter != data.end())
	{
		port << *iter;
		port << std::endl;
		++iter;
	}
	port.close();
	return true;
}

/******************************************************************************/
/*!

this return a reference to a static object of class FileIO, thus ensures that only one object is instanced per life cycle.

*/
/******************************************************************************/

FileIO& FileIO::getInstance()
{
	static FileIO obj;
	return obj;
}

FileIO::FileIO()
{
}


FileIO::~FileIO()
{
	if (port.is_open())
	{
		port.close();
	}
}
