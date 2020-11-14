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
			m_workerThread = std::move(std::thread(&Deserializer::parseSeqTokens, this));
	}
}

conv::Deserializer::Deserializer(const json & jsonFile):
	m_jsonFile(jsonFile), m_workerThread(&Deserializer::parseSeqTokens, this)
{
	if (jsonFile.contains(conv::vertex))
		m_vertexPattern.first = jsonFile.at(conv::vertex)["tag"];
	if (jsonFile.contains(conv::face))
		m_facePattern.first = jsonFile.at(conv::face)["tag"];
	if (jsonFile.contains(conv::normal))
		m_normalPattern.first = jsonFile.at(conv::normal)["tag"];
	if (jsonFile.contains(conv::textCoord))
		m_textCoordPattern.first = jsonFile.at(conv::textCoord)["tag"];
	if (jsonFile.contains(conv::vertColor))
		m_colorPattern.first = jsonFile.at(conv::vertColor)["tag"];
}

std::string conv::Deserializer::streamReader(const std::string& filename, Mesh3D<>& dest)
{
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
