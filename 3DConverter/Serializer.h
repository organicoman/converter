#pragma once
#ifndef SERIALIZER_H
#define SERIALIZER_H

//---------------STL------------
#include <ostream>
//------------------------------
#include "Namespace.h"

class conv::Serializer
{
	json m_fileFormat;
public:
	constexpr Serializer(const json& fileFormat);

	virtual void streamWriter(const Mesh3D<>& source, std::ostream& dest);
	virtual void streamWriter(const Mesh3Df& source, std::ostream& dest);

	virtual ~Serializer() = default;
};
#endif//SERIALIZER_H


