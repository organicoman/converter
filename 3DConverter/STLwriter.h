#pragma once
#include "include/Mesh3D.h"
#include "include/Vertex.h"
#include "include/Namespace.h"

#include <sstream>


// vertex writer
void BinaryWriter(const conv::Mesh3D<>& mesh, const std::string& pat, std::ostream& out)
{
	// make sure the ostream is open in binary mode
	// first the 80 byte header; this header is not in the json template file
	// because Serializer::streamWrite use a formatted output function to
	// write to the output stream.
	std::string buffer{"binary stl file by nadir "};
	auto nTris = mesh.Fsize();
	buffer.append( 54, ' ');
	buffer.push_back('\0');
	// 32 bit uint number of triangular faces.
	buffer.append((char*)&nTris, 4);

	for (auto fIt = mesh.fBegin(); fIt != mesh.fEnd(); ++fIt)
	{
		auto n = fIt->second.normal(mesh);
		auto vIDset = fIt->second.getVertIDs();
		// 32 bit IEEE floating point format for STL
		float i = n[0];
		float j = n[1];
		float k = n[2];
		buffer.append((char*)&i, 4);
		buffer.append((char*)&j, 4);
		buffer.append((char*)&k, 4);
		
		for (auto Vid: vIDset)
		{
			// 32 bit IEEE floating point
			auto v = mesh.getVertex(Vid);
			float x = v.getPos_X();
			float y = v.getPos_Y();
			float z = v.getPos_Z();
			buffer.append((char*)&x , 4);
			buffer.append((char*)&y , 4);
			buffer.append((char*)&z , 4);	
		}
		buffer.append(2, '\0');
	}
	
	out.write(buffer.c_str(), buffer.size());

	return;
}