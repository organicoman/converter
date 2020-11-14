#include "Deserializer.h"
#include <fstream>
#include <string>


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
		return {};
	}

	std::string Line;
	while (std::getline(source, Line))
	{

	}
	return {};
}

void conv::Deserializer::tagPattern()
{
	for (auto obj : m_jsonFile)
	{
		if (obj.contains("tag"))
			m_allTagPattern.insert(obj.at("tag"), obj.at("pattern"));
	}
}
