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
	using namespace std::string_literals;
	// stamp header
	if (m_jsonFile.contains("header"))
		dest << m_jsonFile.at("header").get<std::string>() << '\n';

	// if `section` exists in json file layout then stamp the output according to it
	if (m_jsonFile.contains("section"))
	{
		json jSection = m_jsonFile.at("section");
		auto pattern = jSection.at("pattern").get<std::string>();
		json jTags = jSection.at("tags");
		
		splitter<std::string> findTag(pattern, '%', std::begin(pattern));
		std::string prefix = findTag.next_word();
		std::string suffix = (findTag + 2).curr_word();
		for (auto tag : jTags)
		{
			// the callback which print the mesh features
			stamper_t meshPrint = m_stampers.at(tag.get<std::string>());
			if (prefix.empty())
				dest << pattern << '\n';
			else
				dest << prefix << tag.get<std::string>() << suffix << '\n';
			auto tmpPat = m_allTagPattern.at(tag.get<std::string>());
			meshPrint(mesh, tmpPat, dest);
		}
	}
	// stamp footer
	if (m_jsonFile.contains("footer"))
		dest << m_jsonFile.at("footer").get<std::string>() <<'\n';

	return;
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
	using namespace std::string_literals;
	std::string tag_str = "%tag%"s;

	for (auto obj : m_jsonFile)
	{
		if (obj.contains("tag"s))
		{
			auto tag = obj.at("tag"s).get<std::string>();
			auto pattern = obj.at("pattern"s).get<std::string>();

			m_allTagPattern.insert({ tag, pattern });
			m_stampers.insert({ tag, nullptr });
		}
	}
}
