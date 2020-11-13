#pragma once
#ifndef DESERIALIZER_H
#define DESERIALIZER_H
//---------STL---------------

//---------------------------
#include "Namespace.h"

class conv::Deserializer
{
	json m_fileFormat;

public:
	constexpr Deserializer(const json& fileFormat);

	virtual void streamReader(const std::istream& source, Mesh3D<>& dest);
	virtual void streamReader(const std::istream& source, Mesh3Df& dest);

	virtual ~Deserializer() = default;

};
#endif//DESERIALIZER_H

