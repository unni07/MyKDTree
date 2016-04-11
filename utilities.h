/******************************************************************************/
/*!
\file   utilities.h
\author Unni Nair
\par    email: u.nair@digipen.edu
\par    KDTree
\date   03/04/2016

*/
/******************************************************************************/


/******************************************************************************/
/*!
\class utilities
\brief
Utilities provides different funcitons like distance calculation,data conversion from string to desired data type etc.

Operations include:

- using Eucledian function it calculates distance between 2 point.
- convert given data into a string which can be later used to write onto a file
- converts a string value to desired data to perform manipulations

*/
/******************************************************************************/

#pragma once
#include <vector>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <limits>

template<typename T>
class utilities
{
public:
	static T distance(std::vector<T>, std::vector<T>);
	static const std::string dataTostring(const std::vector<T>& data);
	static const std::string dataTostring(const T& data);
	static const std::vector<T> stringToData(const std::string& data);
private:
	utilities();
	~utilities();

};

/******************************************************************************/
/*!

This fucntion calculates the distance between 2 point.

*/
/******************************************************************************/

template <typename T>
T utilities<T>::distance(std::vector<T> p1, std::vector<T> p2)
{
	T distance = 0;
	for (unsigned int i = 0; i < p1.size(); ++i)
	{
		distance = distance + (p1[i] - p2[i]) * (p1[i] - p2[i]);
	}
	T result = static_cast<T>(sqrt(distance));
	return result;
}


/******************************************************************************/
/*!

Converts a data from a specified format to std::string type. This function returns a string version of the data.

*/
/******************************************************************************/

template <typename T>
const std::string utilities<T>::dataTostring(const std::vector<T>& data)
{
	std::string output;
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		std::ostringstream ss;
		ss << std::setprecision(10) << data[i];
		output.append(ss.str() + ',');
	}
	output = output.substr(0, output.length() - 1);
	return output;
}

/******************************************************************************/
/*!

Specialized template to handle the container with double type of data.

*/
/******************************************************************************/
template <>
const std::string utilities<double>::dataTostring(const std::vector<double>& data)
{
	std::string output;
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		std::ostringstream ss;
		ss << std::setprecision(std::numeric_limits<long double>::digits10) << data[i];
		output.append(ss.str() + ',');
	}
	output = output.substr(0, output.length() - 1);
	return output;
}

/******************************************************************************/
/*!

Specialized template to handle the container with long double type of data.

*/
/******************************************************************************/

template <>
const std::string utilities<long double>::dataTostring(const std::vector<long double>& data)
{
	std::string output;
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		std::ostringstream ss;
		ss << std::setprecision(30) << data[i];
		output.append(ss.str() + ',');
	}
	size_t pos = output.find_last_of(',');
	output = output.substr(0, pos);
	return output;
}

/******************************************************************************/
/*!

This converts a data to a string.

*/
/******************************************************************************/

template <typename T>
const std::string utilities<T>::dataTostring(const T& data)
{
	std::string output;
	std::ostringstream ss;
	ss << std::setprecision(10) << data;
	output.append(ss.str() + ',');
	size_t pos = output.find_last_of(',');
	output = output.substr(0, pos);
	return output;
}

/******************************************************************************/
/*!

converts a string to required data.
data can be
-unsigned int
-int
-long long int
-float
-double
-long double
*/
/******************************************************************************/
template <typename T>
const std::vector<T> utilities<T>::stringToData(const std::string& line)
{
	std::string value;
	value.append(line);
	size_t pos = 0;
	std::vector<T> result;
	while (pos != std::string::npos)
	{
		if (pos == 0)
			value = value.substr(pos, std::string::npos);
		else
			value = value.substr(pos + 1, std::string::npos);
		pos = value.find(",");
		std::stringstream stream(value);
		T temp;
		stream >> temp;
		result.push_back(temp);
	}
	return result;
}

template <typename T>
utilities<T>::utilities()
{
}

template <typename T>
utilities<T>::~utilities()
{
}
