#include "Face.h"
#include "Mesh3D.h"
#include "Vertex.h"

conv::Face::Face(const uint64_t id, uint64_t v1, uint64_t v2, uint64_t v3):
	m_id(id), m_vertices({v1, v2, v3})
{
}

uint64_t conv::Face::getID() const
{
	return m_id;
}

std::unordered_set<uint64_t> conv::Face::getVertIDs() const
{
	return m_vertices;
}

double conv::Face::area(const Mesh3D<>& mesh) const
{
	if (m_vertices.size() != 3)
		return 0.0;
	auto it = m_vertices.begin();
	auto v1 = mesh.getVertex(*it);
	auto v2 = mesh.getVertex(*++it);
	auto v3 = mesh.getVertex(*++it);

	v1 = v1 - v3;
	v2 = v2 - v3;

	double cx = v1.getPos_Y() * v2.getPos_Z() - v1.getPos_Z() * v2.getPos_Y();
	double cy = v1.getPos_Z() * v2.getPos_X() - v1.getPos_X() * v2.getPos_Z();
	double cz = v1.getPos_X() * v2.getPos_Y() - v1.getPos_Y() * v2.getPos_X();

	return std::sqrt((cx * cx) + (cy * cy) + (cz * cz));
}

conv::pos3D<> conv::Face::normal(const Mesh3D<>& mesh) const
{
	if (m_vertices.size() != 3)
		return { 0.0, 0.0 , 0.0 };
	auto it = m_vertices.begin();
	auto v1 = mesh.getVertex(*it);
	auto v2 = mesh.getVertex(*++it);
	auto v3 = mesh.getVertex(*++it);

	v1 = v1 - v3;
	v2 = v2 - v3;

	double cx = v1.getPos_Y() * v2.getPos_Z() - v1.getPos_Z() * v2.getPos_Y();
	double cy = v1.getPos_Z() * v2.getPos_X() - v1.getPos_X() * v2.getPos_Z();
	double cz = v1.getPos_X() * v2.getPos_Y() - v1.getPos_Y() * v2.getPos_X();

	double mag = (cx*cx) + (cy*cy) + (cz*cz);
	return {cx/mag, cy/mag, cz/mag};
}

conv::pos3D<> conv::Face::center(const Mesh3D<>& mesh) const
{
	if (m_vertices.size() != 3)
		return { 0.0, 0.0 , 0.0 };
	auto it = m_vertices.begin();
	auto v1 = mesh.getVertex(*it);
	auto v2 = mesh.getVertex(*++it);
	auto v3 = mesh.getVertex(*++it);

	v1 = v1 - v3;
	v2 = v2 - v3;

	double cx = v1.getPos_X() + v2.getPos_X();
	double cy = v1.getPos_Y() + v2.getPos_Y();
	double cz = v1.getPos_Z() + v2.getPos_Z();

	return { cx, cy, cz };
}
