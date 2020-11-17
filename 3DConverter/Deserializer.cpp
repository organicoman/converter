#include "Deserializer.h"
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>


conv::Deserializer::Deserializer(const std::string & jsonFile)	
{
	std::ifstream source(jsonFile);
	if (source.is_open())
	{
		source >> m_jsonFile;
		// start a worker thread only when m_jsonFile structure is populated
		if(!m_jsonFile.empty())
			m_workerThread = std::move(std::thread(&Deserializer::tagPattern, this));
	}
}

constexpr conv::Deserializer::Deserializer(const json & jsonFile):
	m_jsonFile(jsonFile), m_workerThread(&Deserializer::tagPattern, this)
{
}

constexpr conv::Deserializer::Deserializer(json && jsonFile):
	m_jsonFile(std::move(jsonFile)), m_workerThread(&Deserializer::tagPattern, this)
{
}

std::string conv::Deserializer::streamReader(const std::string& filename, Mesh3D<>& dest)
{
	// make sure the worker thread finished populating the hash Table
	m_workerThread.join();

	std::ifstream source(filename);
	if (!source.is_open())
	{
		std::cerr << "Fatal Error: Unable to open file " << filename.c_str() << '\n';
		return {"ERROR"};
	}

	std::string Line;
	while (std::getline(source, Line))
	{
		std::istringstream lineStream(Line);
		std::string word;

		// extract the tag and stops at the first one which is not handled by this method
		// the failing line will be returned for further parse by an eventual overloaded
		// derived method.
		lineStream >> word;
		if (m_allTagPattern.find(word) == m_allTagPattern.end())
			return Line;

		parsePattern(lineStream, m_allTagPattern[word], dest);
	}
	return {}; // empty string for success.
} 

std::string conv::Deserializer::streamReader(const std::string & filename, Mesh3Df & dest)
{
	// becarful it's a C cast!
	streamReader(filename, (Mesh3D<>&) dest);
	return std::string();
}

void conv::Deserializer::tagPattern()
{
	// FIX ME: handle the object files wich uses 'Section' to define the 3D Mesh Features

	using namespace std::string_literals;
	std::string tag_str = "%tag%"s;

	for (auto obj : m_jsonFile)
	{
		if (obj.contains("tag"s))
		{
			auto tag = obj.at("tag"s).get<std::string>();
			auto pattern = obj.at("pattern"s).get<std::string>();

			// find the string '%tag%' in pattern and,
			// append its position index to the end of 'pattern'
			auto it = std::search(pattern.begin(), pattern.end(), 
								  tag_str.begin(), tag_str.end());
			size_t pos = pattern.begin() - it;
			pattern.push_back(' '); // seperate this addition with a space
			pattern += std::to_string(pos);

			m_allTagPattern.insert({ tag, pattern });
		}
	}
}

bool conv::Deserializer::parsePattern(std::istringstream & inputLine, const std::string & pattern, Mesh3D<>& dest) const
{
	/*
	 * pattern parsing:
	 * %tag% -> is the tag label, to be ignored
	 * %id%  -> is the id of the feature expl: vID, fID ..etc
	 * %i%   -> any integer value
	 * %f%   -> any float value
	 * %d%   -> any double value
	 * %*%   -> repeat the last pattern indifinitly separated with one or more spaces
	            this wild card SHOULD BE USED AT THE END OF THE PATTERN, or it causes errors
	 * %?%   -> all what is left of the string line is lumped in one variable.
	 * char  -> any character which is not enclosed in %% is copied as it is.
	 */

	// --Algorithm Start--
	// -- replace %tag% value in pattern --


	return false;
}

bool conv::Deserializer::parsePattern(std::istringstream & inputLine, const std::string & pattern, Mesh3Df & dest) const
{
	// becarful it's a C cast!
	return parsePattern(inputLine, pattern, (Mesh3D<>&) dest);
}
