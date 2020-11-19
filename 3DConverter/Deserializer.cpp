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
		// for each line from the source file, scan it
		// for patterns in the provided JSON template file, otherwise
		// stop and return it to the caller for futher processing
		if (!parsePattern(Line, dest))
			return Line;
	}
	return {}; // empty string for success.
} 

std::string conv::Deserializer::streamReader(const std::string & filename, Mesh3Df & dest)
{
	// becarful it's a C cast!
	streamReader(filename, (Mesh3D<>&) dest);
	return std::string();
}

bool conv::Deserializer::parsePattern(const std::string& inputLine, Mesh3D<>& dest) const 
{
	conv::splitter<const std::string&> splitLine(inputLine, ' ', std::cbegin(inputLine));
	auto keyVal = m_allTagPattern.begin();
	for (; keyVal!= m_allTagPattern.end(); ++keyVal)
	{
		std::string pattern = keyVal->second;
		conv::splitter<std::string> splitPatern(pattern, ' ', std::end(pattern));
		auto tagPos = splitPatern.prev_word();
		splitLine += std::stoul(tagPos);
		// seek a match between text at position tagPos and the value of available tags
		if (keyVal->first != splitLine.curr_word())
			continue;
		break; // match!
	}
	if (keyVal == m_allTagPattern.end())
		return false;
	
	return dispatcher(keyVal->first, inputLine);
}

bool conv::Deserializer::parsePattern(const std::string& inputLine, Mesh3Df & dest) const 
{
	// becarful it's a C cast!
	return parsePattern(inputLine, (Mesh3D<>&) dest);
}

bool conv::Deserializer::dispatcher(const std::string& tag, const std::string& inputLine) const
{
	std::string patCopy = m_allTagPattern[tag];
	//pop off the added text in the back of the pattern
	while (patCopy.back() != ' ')
		patCopy.pop_back();
	auto parser = m_parsers[tag];

	return parser(patCopy, inputLine);
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