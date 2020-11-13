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
	Face(const uint64_t id, const std::unordered_set<uint64_t>& verticesID);
	Face(const uint64_t id, std::unordered_set<uint64_t>&& verticesID);
	
	/*
	 * calculates the surface area of the face
	 * @returns the surface area in double precision
	 */
	double area() const;

	/*
	 * calculates the face normal
	 * @returns the 3D normal vector in double precision
	 */
	pos3D<> normal() const;

	/*
	 * calculates the face center in 3D space
	 * @returns the 3D position of the face center in double precision
	 */
	pos3D<> center() const;

	/*
	 * flip the normal of this face by swaping the 1st and 3rd vertex
	 * @param: no param
	 * @returns: nothing
	 */

	inline void flipNormal();
};
#endif //FACE_H

