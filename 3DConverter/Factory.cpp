#include "Factory.h"

std::shared_ptr<conv::Serializer> conv::Factory::makeSerializer(const json& fileFormat)
{
	return std::make_shared<conv::Serializer>(fileFormat);
}

std::shared_ptr<conv::Serializer> conv::Factory::makeSerializer(const std::string& jsonFileName)
{
	return std::make_shared<conv::Serializer>(jsonFileName);
}

std::shared_ptr<conv::Deserializer> conv::Factory::makeDeserializer(const json& fileFormat)
{
	return std::make_shared<conv::Deserializer>(fileFormat);
}

std::shared_ptr<conv::Deserializer> conv::Factory::makeDeserializer(const std::string& jsonFileName)
{
	return std::make_shared<Deserializer>(jsonFileName);
}
