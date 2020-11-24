#include "include/Converter.h"
#include "include/Mesh3D.h"
#include <fstream>

conv::Converter::Converter():
	m_des(nullptr), m_ser(nullptr), 
	m_SerializerJsonTemplate{}, m_DeserializerJsonTemplate{}
{
}

conv::Converter::Converter(const std::shared_ptr<Deserializer>& meshReader):
	m_des(meshReader), m_ser(nullptr),
	m_SerializerJsonTemplate{}
{
	m_DeserializerJsonTemplate = m_des->getJson();
}

conv::Converter::Converter(const std::shared_ptr<Deserializer>& meshReader, const std::shared_ptr<Serializer>& meshWriter):
	m_des(meshReader), m_ser(meshWriter)
{
	m_DeserializerJsonTemplate = m_des->getJson();
	m_SerializerJsonTemplate = m_ser->getJson();
}

void conv::Converter::setDeserializer(const std::shared_ptr<Deserializer>& meshReader)
{
	m_des = meshReader;
	m_DeserializerJsonTemplate = m_des->getJson();
}

void conv::Converter::setSerializer(const std::shared_ptr<Serializer>& meshWriter)
{
	m_ser = meshWriter;
	m_SerializerJsonTemplate = m_ser->getJson();
}

std::shared_ptr<conv::Serializer> conv::Converter::getSerializer() const
{
	return m_ser;;
}

std::shared_ptr<conv::Deserializer> conv::Converter::getDeserializer() const
{
	return m_des;
}

void conv::Converter::Read(const std::string& srcFile)
{
	m_des->streamReader(srcFile, mesh);
}

void conv::Converter::Write(const std::string& destFile) const
{
	auto Ext = m_SerializerJsonTemplate.find(".ext");
	auto Type = m_SerializerJsonTemplate.find("type");
	splitter<std::string> extSplit(destFile, '.', destFile.begin());
	// check if same extension
	// extension in `destFile` is stronger
	auto name = extSplit.next_word();
	if (extSplit.next_word().empty())
	{
		if (Ext != m_SerializerJsonTemplate.end())
			name += *Ext;
	}
	else
		name = destFile;

	std::ios_base::openmode mode = std::ios_base::out;
	if (Type != m_SerializerJsonTemplate.end())
		if(*Type == "binary")
			mode |= std::ios_base::binary;
	std::ofstream out{ name, mode };

	m_ser->streamWriter(mesh, out);
}

void conv::Converter::transformMesh(const Matrix<>& mat)
{
	mesh.transformMesh(mat);
}

bool conv::Converter::isInside(const pos3D<>& point) const
{
	return mesh.isInside(point);
}

double conv::Converter::volume() const
{
	return mesh.volume();
}

double conv::Converter::area() const
{
	return mesh.area();
}
