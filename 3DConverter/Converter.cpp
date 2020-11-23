#include "include/Converter.h"
#include "include/Mesh3D.h"

conv::Mesh3D<> conv::Converter::mesh{};

conv::Converter::Converter():
	m_des(nullptr), m_ser(nullptr)
{
}

conv::Converter::Converter(const std::shared_ptr<Deserializer>& meshReader):
	m_des(meshReader), m_ser(nullptr)
{
}

conv::Converter::Converter(const std::shared_ptr<Deserializer>& meshReader, const std::shared_ptr<Serializer>& meshWriter):
	m_des(meshReader), m_ser(meshWriter)
{
}

void conv::Converter::setDeserializer(const std::shared_ptr<Deserializer>& meshReader)
{
	m_des = meshReader;
}

void conv::Converter::setSerializer(const std::shared_ptr<Serializer>& meshWriter)
{
	m_ser = meshWriter;
}

std::shared_ptr<conv::Serializer> conv::Converter::getSerializer() const
{
	return m_ser;;
}

std::shared_ptr<conv::Deserializer> conv::Converter::getDeserializer() const
{
	return m_des;
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
