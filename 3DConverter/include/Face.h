#pragma once
#ifndef FACE_H
#define FACE_H
//---------STL-------
#include <unordered_set>

//--------------------
#include "Namespace.h"

class conv::Face
{
	uint64_t m_id;
	std::unordered_set<uint64_t> m_vertices;

public:
	/*
	 * represents the concept of a face 
	 * and the IDs of the 3 vertices which define it
	 * @param id: a unique id which identify this face in the containing 3D Mesh
	 * @param verticesID: a set of 3 distinct vertex IDs, if more than 3 IDs are given then
	 *                    only the 3 first ones are kept, the others are ignored.
	 */
	Face(const uint64_t id, uint64_t v1, uint64_t v2, uint64_t v3);

	/*
	 * @returns: this face ID, this face is abstract until you tie it to a Mesh
	 */
	uint64_t getID() const;

	/*
	 * returns a set of vertex IDs, if the construction of this face was
	 * correct then the size of this set is 3, otherwise one or more of the IDs
	 * was duplicated
	 * @returns: a set of vertex IDs
	 */
	std::unordered_set<uint64_t> getVertIDs() const;

	/*
	 * calculates the surface area of the face, after tying it to a Mesh
	 * @param mesh: a mesh where you want a face with the same vertices
	 * @returns: the surface area in double precision of the face if it exists on
	 *           this mesh, otherwise 0.0
	 */
	double area(const Mesh3D<>& mesh) const;

	/*
	 * calculates the face normal, not normalized into [-1,1] range
	 * @returns the 3D normal vector in double precision
	 */
	pos3D<> normal(const Mesh3D<>& mesh) const;

	/*
	 * calculates the face center in 3D space
	 * @returns the 3D position of the face center in double precision
	 */
	pos3D<> center(const Mesh3D<>& mesh) const;

	bool operator ==(const Face& other) const;
};
#endif //FACE_H