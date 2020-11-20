#include "Serializer.h"
#include <fstream>

conv::Serializer::Serializer(const std::string& jsonFile)
{
	std::ifstream source(jsonFile);
	if (source.is_open())
	{
		source >> m_jsonFile;
		// start a worker thread only when m_jsonFile structure is populated
		if (!m_jsonFile.empty())
			m_workerThread = std::thread(&Serializer::tagPattern, this);
	}
}

conv::Serializer::Serializer(const json& jsonFile) :
	m_jsonFile(jsonFile), m_workerThread(&Serializer::tagPattern, this)
{
}

conv::Serializer::Serializer(json&& jsonFile) :
	m_jsonFile(std::move(jsonFile)), m_workerThread(&Serializer::tagPattern, this)
{
}

void conv::Serializer::streamWriter(const Mesh3D<>& mesh, std::ostream& dest)
{
	// make sure the worker thread finished populating the hash Table
	if(m_workerThread.joinable())
		m_workerThread.join();

}

void conv::Serializer::streamWriter(const Mesh3Df& mesh, std::ostream& dest)
{
	// becarful it's a C cast!	
	return streamWriter((Mesh3D<>&)mesh, dest);
}

conv::RET_CODE conv::Serializer::addStamper(const std::string& tag, stamper_t parser)
{
	// make sure the worker thread finished populating the hash Table
	if (m_workerThread.joinable())
		m_workerThread.join();

	auto prev = m_stampers.find(tag);
	conv::RET_CODE ret = conv::ADDED;
	if (prev == m_stampers.end() || prev->second == nullptr)
		ret = conv::ADDED;
	else
		ret = conv::REPLACED;

	m_stampers[tag] = parser;
	return ret;	
}

void conv::Serializer::tagPattern()
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
			m_parsers.insert({ tag, nullptr });
		}
	}
}
