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
			m_workerThread = std::thread(&Deserializer::tagPattern, this);
	}
}

conv::Deserializer::Deserializer(const json & jsonFile):
	m_jsonFile(jsonFile), m_workerThread(&Deserializer::tagPattern, this)
{
}

conv::Deserializer::Deserializer(json && jsonFile):
	m_jsonFile(std::move(jsonFile)), m_workerThread(&Deserializer::tagPattern, this)
{
}

std::string conv::Deserializer::streamReader(const std::string& filename, Mesh3D<>& dest)
{
	// make sure the worker thread finished populating the hash Table
	if(m_workerThread.joinable())
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
	return std::string{}; // empty string for success.
} 

std::string conv::Deserializer::streamReader(const std::string & filename, Mesh3Df & dest)
{
	// becarful it's a C cast!	
	return streamReader(filename, (Mesh3D<>&) dest);
}

conv::RET_CODE conv::Deserializer::addParser(const std::string & tag, parser_t parser)
{
	// make sure the worker thread finished populating the hash Table
	if (m_workerThread.joinable())
		m_workerThread.join();

	auto prev = m_parsers.find(tag);
	conv::RET_CODE ret = conv::ADDED;
	if (prev == m_parsers.end() || prev->second == nullptr)
		ret = conv::ADDED;
	else
		ret = conv::REPLACED;

	m_parsers[tag] = parser;
	return ret;
}

json conv::Deserializer::getJson() const
{
	return m_jsonFile;
}

bool conv::Deserializer::parsePattern(const std::string& inputLine, Mesh3D<>& dest) const 
{
	conv::splitter<std::string> splitLine(inputLine, ' ', std::begin(inputLine));
	auto keyVal = m_allTagPattern.begin();
	for (; keyVal!= m_allTagPattern.end(); ++keyVal)
	{
		std::string pattern = keyVal->second;
		conv::splitter<std::string> splitPatern(pattern, ' ', std::end(pattern));
		auto tagPos = splitPatern.prev_word(); //get last word 
		splitLine += std::stoul(tagPos);
		// seek a match between text at position tagPos and the value of available tags
		if (keyVal->first != splitLine.next_word())
		{
			// reset the position to the beginning.
			splitLine.rewind();
			continue;
		}
		break; // else matched!
	}
	if (keyVal == m_allTagPattern.end())
		return false;
	
	return dispatcher(keyVal->first, inputLine, dest);
}

bool conv::Deserializer::parsePattern(const std::string& inputLine, Mesh3Df & dest) const 
{
	// becarful it's a C cast!
	return parsePattern(inputLine, (Mesh3D<>&) dest);
}

bool conv::Deserializer::dispatcher(const std::string& tag, const std::string& inputLine, Mesh3D<>& dest) const
{
	std::string patCopy = m_allTagPattern.at(tag);
	auto parserIt = m_parsers.find(tag);
	if (parserIt == m_parsers.end() || parserIt->second == nullptr)
		return false;

	//pop off the added text in the back of the pattern
	while (patCopy.back() != ' ')
		patCopy.pop_back();
	return parserIt->second(patCopy, inputLine, dest);
}
void conv::Deserializer::tagPattern()
{
	// FIX ME: handle the object files wich uses 'Section' to define the 3D Mesh Features

	for (auto obj : m_jsonFile)
	{
		if (obj.contains(conv::TAG))
		{
			auto tag = obj.at(conv::TAG).get<std::string>();
			auto pattern = obj.at(conv::PATTERN).get<std::string>();

			// find the string '%tag%' in pattern and,
			// append its position index to the end of 'pattern'
			auto it = std::search(pattern.begin(), pattern.end(),
				std::begin(conv::_TAG_), std::end(conv::_TAG_));
			size_t pos = it - pattern.begin();
			pattern.push_back(' '); // seperate this addition with a space
			pattern += std::to_string(pos);

			m_allTagPattern.insert({ tag, pattern });
			m_parsers.insert({ tag, nullptr });
		}
	}
}
namespace conv
{
	std::string dumpTheJsonFile(Deserializer& self)
	{
		if (self.m_workerThread.joinable())
			self.m_workerThread.join();

		return self.m_jsonFile.dump();
	}

	std::unordered_set<std::string> dumpAllTags(Deserializer& self)
	{
		if (self.m_workerThread.joinable())
			self.m_workerThread.join();

		std::unordered_set<std::string> tags;
		for (auto& pairs : self.m_allTagPattern)
			tags.insert(pairs.first);
		return tags;
	}
	std::unordered_set<std::string> dumpAllPatterns(Deserializer& self)
	{
		if (self.m_workerThread.joinable())
			self.m_workerThread.join();

		std::unordered_set<std::string> patterns;
		for (auto& pairs : self.m_allTagPattern)
			patterns.insert(pairs.second);
		return patterns;
	}
}
