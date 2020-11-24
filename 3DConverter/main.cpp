// 3DConverter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

//
#include "include/Converter.h"
#include "include/Factory.h"

// callbacks for reading OBJ, writing STL binary
#include "OBJ_reader.h"
#include "STLwriter.h"

int main()
{
	std::string STL_jsonTemplate = "C:\\Users\\Nadir\\source\\Shapr3d\\3DConverter\\STL_jsonTemplate.json";
	std::string OBJ_jsonTemplate = "C:\\Users\\Nadir\\source\\Shapr3d\\3DConverter\\OBJ_jsonTemplate.json";
	conv::Factory fac;
	conv::Converter cnv;
	std::shared_ptr<conv::Serializer> ser = fac.makeSerializer(STL_jsonTemplate);
	std::shared_ptr<conv::Deserializer> des = fac.makeDeserializer(OBJ_jsonTemplate);
	cnv.setSerializer(ser);
	cnv.setDeserializer(des);
	conv::stamper_t stlBinaryWriter{ BinaryWriter };
	ser->addStamper("facet", stlBinaryWriter);
	conv::parser_t objVertexParser{ OBJvertex };
	conv::parser_t objFaceParser{ OBJface };
	conv::parser_t objNormalParser{ OBJnormal };
	conv::parser_t objTexCoordParser{ OBJtextCoord };
	des->addParser("v", objVertexParser);
	des->addParser("vn", objNormalParser);
	des->addParser("vt", objTexCoordParser);
	des->addParser("f", objFaceParser);

	cnv.Read("C:\\Users\\Nadir\\source\\Shapr3d\\3DConverter\\pumpkin_example.obj");
	cnv.Write("C:\\Users\\Nadir\\source\\Shapr3d\\3DConverter\\example.stl");

	return 0;

}

