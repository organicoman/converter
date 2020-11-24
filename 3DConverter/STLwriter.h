#pragma once
#include "include/Mesh3D.h"
#include "include/Vertex.h"
#include "include/Namespace.h"

// vertex writer
void BinaryWriter(const conv::Mesh3D<>& mesh, const std::string& pat, std::ostream& out)
{
	// make sure the ostream is open in binary mode
	// first the 80 byte header
	std::string header{80,'#'};
	uint32_t nTris = mesh.Fsize();
	out.write(header.c_str(), header.size());
	out.write((const char*) &nTris, 4);
	for (auto fIt = mesh.fBegin(); fIt != mesh.fEnd(); ++fIt)
	{
		auto n = fIt->second.normal(mesh);
		auto vIDset = fIt->second.getVertIDs();
		for (auto Vid: vIDset)
		{
			out.write((const char*)&n[0], 4);
			out.write((const char*)&n[1], 4);
			out.write((const char*)&n[2], 4);
			auto v = mesh.getVertex(Vid);
			double x = v.getPos_X();
			double y = v.getPos_Y();
			double z = v.getPos_Z();
			out.write((const char*)&x , 4);
			out.write((const char*)&y , 4);
			out.write((const char*)&z , 4);
		}
	}
	uint32_t end = 0;
	out.write((const char*)end, 2);

	return;
}