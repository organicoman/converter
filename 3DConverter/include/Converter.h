#pragma once
#ifndef CONVERTER_H
#define CONVERTER_H

//----------STL------------
#include <memory>

//-------------------------
#include "Namespace.h"
#include "Vertex.h"
#include "Face.h"
#include "Mesh3D.h"
#include "Deserializer.h"
#include "Serializer.h"

class conv::Converter
{
	std::shared_ptr<Deserializer> m_des;
	std::shared_ptr<Serializer>   m_ser;

	json m_SerializerJsonTemplate;
	json m_DeserializerJsonTemplate;

	Mesh3D<> mesh{};

public:
	Converter();
	Converter(const std::shared_ptr<Deserializer>& meshReader);
	Converter(const std::shared_ptr<Deserializer>& meshReader, const std::shared_ptr<Serializer>& meshWriter);

	void setDeserializer(const std::shared_ptr<Deserializer>& meshReader);
	void setSerializer(const std::shared_ptr<Serializer>& meshWriter);

	std::shared_ptr<Serializer> getSerializer() const;
	std::shared_ptr<Deserializer> getDeserializer() const;

	void Read(const std::string& srcFile);
	void Write(const std::string& destFile) const;

	void transformMesh(const Matrix<>& mat);
	bool isInside(const pos3D<>& point) const;
	double volume() const;
	double area() const;

};
#endif//CONVERTER_H
