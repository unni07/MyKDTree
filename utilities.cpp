#include "utilities.h"
#include <sstream>
#include <iomanip>


float utilities::distance(std::vector<double> p1, std::vector<double> p2)
{
	float distance = 0.0f;
	for (int i = 0; i < p1.size();++i)
	{
		distance = distance + (p1[i] - p2[i]) * (p1[i] - p2[i]);
	}

	return sqrt(distance);
}

void utilities::getValue(const std::string line, std::vector<double>& result)
{
	std::string value(line);
	size_t pos = 0;
	while (pos != std::string::npos)
	{
		if (pos == 0)
			value = value.substr(pos, std::string::npos);
		else
			value = value.substr(pos + 1, std::string::npos);
		pos = value.find(",");
		std::stringstream stream(value);
		double temp = 0.0;
		stream >> temp;
		result.push_back(temp);
	}
}

const std::string utilities::dataTostring(const std::vector<double> data)
{
	std::string output;
	for (int i = 0; i < data.size(); ++i)
	{
		std::ostringstream ss;
		ss << std::setprecision(30) << data[i];
		output.append(ss.str() + ',');
	}
	size_t pos = output.find_last_of(',');
	output = output.substr(0, pos);
	return output;
}

const std::string utilities::dataTostring(const double data)
{
	std::string output;
		std::ostringstream ss;
		ss << std::setprecision(30) << data;
		output.append(ss.str() + ',');
	size_t pos = output.find_last_of(',');
	output = output.substr(0, pos);
	return output;
}

const std::vector<double> utilities::stringToData(const std::string line)
{
	std::string value;
	value.append(line);
	size_t pos = 0;
	std::vector<double> result;
	while (pos != std::string::npos)
	{
		if (pos == 0)
			value = value.substr(pos, std::string::npos);
		else
			value = value.substr(pos + 1, std::string::npos);
		pos = value.find(",");
		std::stringstream stream(value);
		double temp = 0.0;
		stream >> temp;
		result.push_back(temp);
	}
	return result;
}

utilities::utilities()
{
}


utilities::~utilities()
{
}
