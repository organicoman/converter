#include "Face.h"
#include "Mesh3D.h"
#include "Vertex.h"
#include <initializer_list>

conv::Face::Face(const uint64_t id, uint64_t v1, uint64_t v2, uint64_t v3) :
	m_id(id), m_vertices(std::initializer_list<uint64_t>{ v1, v2, v3 })
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

	auto vec1 = v1 - v3;
	auto vec2 = v2 - v3;

	double cx = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	double cy = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	double cz = vec1[0] * vec2[1] - vec1[1] * vec2[0];

	return 0.5 * std::sqrt((cx * cx) + (cy * cy) + (cz * cz));
}

conv::pos3D<> conv::Face::normal(const Mesh3D<>& mesh) const
{
	if (m_vertices.size() != 3)
		return { 0.0, 0.0 , 0.0 };
	auto it = m_vertices.begin();
	auto v1 = mesh.getVertex(*it);
	auto v2 = mesh.getVertex(*++it);
	auto v3 = mesh.getVertex(*++it);

	auto vec1 = v1 - v3;
	auto vec2 = v2 - v3;

	double cx = vec1[1] * vec2[2] - vec1[2] * vec2[1];
	double cy = vec1[2] * vec2[0] - vec1[0] * vec2[2];
	double cz = vec1[0] * vec2[1] - vec1[1] * vec2[0];

	//double mag = (cx * cx) + (cy * cy) + (cz * cz);
	double mag = 1.0;
	return { cx / mag, cy / mag, cz / mag };
}

conv::pos3D<> conv::Face::center(const Mesh3D<>& mesh) const
{
	if (m_vertices.size() != 3)
		return { 0.0, 0.0 , 0.0 };
	auto it = m_vertices.begin();
	auto v1 = mesh.getVertex(*it);
	auto v2 = mesh.getVertex(*++it);
	auto v3 = mesh.getVertex(*++it);

	auto x = v1.getPos_X() + v2.getPos_X() + v3.getPos_X();
	auto y = v1.getPos_Y() + v2.getPos_Y() + v3.getPos_Y();
	auto z = v1.getPos_Z() + v2.getPos_Z() + v3.getPos_Z();

	return { x / 3, y / 3, z / 3 };
}

bool conv::Face::operator ==(const conv::Face& other) const
{
	bool t = getID() == other.getID();
	bool v = getVertIDs() == other.getVertIDs();

	return t & v;
}

double conv::Face::sumArea(const conv::Face& other, const conv::Mesh3D<>& mesh) const
{
	return area(mesh) + other.area(mesh);
}
