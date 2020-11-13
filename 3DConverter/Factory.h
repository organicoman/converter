#pragma once
#ifndef FACTORY_H
#define FACTORY_H
//----------STL---------

//----------------------
#include "Namespace.h"

struct conv::Factory
{
	virtual Serializer makeSerializer(const json& fileFormat) = 0 ;
	virtual Deserializer makeDeserializer(const json& fileFormat) = 0 ;

	virtual ~Factory();
};

#endif //FACTORY_H
