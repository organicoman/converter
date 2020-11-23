#pragma once
#ifndef FACTORY_H
#define FACTORY_H
//----------STL---------
#include <memory>
#include <string>

//----------------------
#include "Namespace.h"
#include "Serializer.h"
#include "Deserializer.h"


struct conv::Factory
{
	std::shared_ptr<Serializer> makeSerializer(const json& fileFormat);
	std::shared_ptr<Serializer> makeSerializer(const std::string& jsonFileName);

	std::shared_ptr<Deserializer> makeDeserializer(const json& fileFormat);
	std::shared_ptr<Deserializer> makeDeserializer(const std::string& jsonFileName);
};

#endif //FACTORY_H
