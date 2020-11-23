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

	// stamp header
	if (m_jsonFile.contains(conv::HEADER))
	{
		auto t = m_jsonFile.at(conv::HEADER).get<std::string>();
		if (!t.empty())
			dest << t << '\n';
	}
		 

	// if `section` exists in json file layout then stamp the output according to it
	if (m_jsonFile.contains(conv::SECTION))
	{
		json jSection = m_jsonFile.at(conv::SECTION);
		auto pattern = jSection.at(conv::PATTERNS).get<std::string>();
		json jTags = jSection.at(conv::TAGS);
		
		splitter<std::string> findTag(pattern, '%', std::begin(pattern));
		std::string prefix = findTag.next_word();
		std::string suffix = (findTag + 2).curr_word();
		for (auto tag : jTags)
		{
			// find the json object in the template file corresponding to this section tag
			auto objIt = m_jsonFile.find(tag.get<std::string>());
			if (objIt == m_jsonFile.end())
				continue;
			json sectionObj = *objIt;
			
			// in this section find the tag , expl: "tag":"v"
			// then the callback which print the mesh features
			auto secTagIt = sectionObj.find(conv::TAG);
			if (secTagIt == sectionObj.end())
				continue;
			auto sectionTag = secTagIt->get<std::string>();
			
			auto it = m_stampers.find(sectionTag);
			if (it == m_stampers.end())
				continue;

			stamper_t meshPrint = nullptr;
			meshPrint = it->second;
			if (meshPrint == nullptr)
				continue;

			if (prefix.empty() && !pattern.empty())
				dest << pattern << '\n';
			else
				dest << prefix << tag.get<std::string>() << suffix << '\n';
			// find the pattern
			auto tmpPat = m_allTagPattern.at(sectionTag);
			meshPrint(mesh, tmpPat, dest);
		}
	}
	else // there are no sectin defined in the json tamplate file
		// then for each tag found call the stamper associated
	{
		for (auto& tagPat : m_allTagPattern)
		{
			auto meshPrint = m_stampers.at(tagPat.first);
			if (meshPrint == nullptr)
				continue;
			meshPrint(mesh, tagPat.second, dest);
		}
	}
	// stamp footer
	if (m_jsonFile.contains(conv::FOOTER))
	{
		auto t = m_jsonFile.at(conv::FOOTER).get<std::string>();
		if (!t.empty())
			dest << t << '\n';
	}

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

	for (auto obj : m_jsonFile)
	{
		if (obj.contains(conv::TAG))
		{
			auto tag = obj.at(conv::TAG).get<std::string>();
			auto pattern = obj.at(conv::PATTERN).get<std::string>();

			m_allTagPattern.insert({ tag, pattern });
			m_stampers.insert({ tag, nullptr });
		}
	}
}

namespace conv
{
	std::string dumpTheJsonFile(Serializer& self)
	{
		if (self.m_workerThread.joinable())
			self.m_workerThread.join();

		return self.m_jsonFile.dump();
	}

	std::unordered_set<std::string> dumpAllTags(Serializer& self)
	{
		if (self.m_workerThread.joinable())
			self.m_workerThread.join();

		std::unordered_set<std::string> tags;
		for (auto& pairs : self.m_allTagPattern)
			tags.insert(pairs.first);
		return tags;
	}
	std::unordered_set<std::string> dumpAllPatterns(Serializer& self)
	{
		if (self.m_workerThread.joinable())
			self.m_workerThread.join();

		std::unordered_set<std::string> patterns;
		for (auto& pairs : self.m_allTagPattern)
			patterns.insert(pairs.second);
		return patterns;
	}
}