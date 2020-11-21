#pragma once
#ifndef CONVERTER_H
#define CONVERTER_H

//----------STL------------
#include <memory>

//-------------------------
#include "Namespace.h"
#include "Deserializer.h"
#include "Serializer.h"

class conv::Converter
{
	std::shared_ptr<Deserializer> m_des;
	std::shared_ptr<Serializer>   m_ser;

	template<typename T> static Mesh3D<T> object;

public:
	Converter();
	Converter(std::shared_ptr<Deserializer> meshReader);
	Converter(std::shared_ptr<Deserializer> meshReader, std::shared_ptr<Serializer> meshWriter);

	void setDeserializer(const Deserializer& meshReader);
	void setSerializer(const Serializer& meshWriter);

	std::shared_ptr<Serializer> getSerializer() const;
	std::shared_ptr<Deserializer> getDeserializer() const;

	void transformMesh(const Matrix<>& mat);
	bool isInside(const pos3D<>& point) const;
	double volume() const;
	double area() const;

};
#endif//CONVERTER_H