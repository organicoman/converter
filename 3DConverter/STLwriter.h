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
	out << nTris;
	for (auto fIt = mesh.fBegin(); fIt != mesh.fEnd(); ++fIt)
	{
		auto n = fIt->second.normal(mesh);
		auto vIDset = fIt->second.getVertIDs();
		for (auto Vid: vIDset)
		{
			out << n[0] << n[1] << n[2];
			auto v = mesh.getVertex(Vid);
			out << v.getPos_X() << v.getPos_Y() << v.getPos_Z();
		}
	}
	out << uint16_t(0x0000);

	return;
}