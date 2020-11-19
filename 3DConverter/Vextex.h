#pragma once
#ifndef VERTEX_H
#define VERTEX_H
//--------STL------
#include <vector>

//-----------------
#include "Namespace.h"
template<typename T = double>
class conv::Vertex
{
	uint64_t m_id;
	pos3D<T> m_position;
	pos3D<T> m_normal;
	pos2D<T> m_texCoord;
	color    m_color;

public:
	Vertex(const uint64_t idx);
	Vertex(const uint64_t idx, const T x, const T y, const T z);
	//Vertex(const uint64_t idx, const pos3D pos);
	void setPos(const pos3D<T>& position);
	void setPos(const T x, const T y, const T z);
	pos3D<T> getPos() const;
	T getPos_X() const;
	T getPos_Y() const;
	T getPos_Z() const;

	void setNormal(const pos3D<T>& coord);
	void setNormal(const T i, const T j, const T k);
	pos3D<T> getNormal() const;
	T getNormal_i() const;
	T getNormal_j() const;
	T getNormal_k() const;

	void setTexCoord(const pos2D<T>& coordinates);
	void setTexCoord(const T u, const T v);
	pos2D<T> getTexCoord() const;
	T getTexCoord_u() const;
	T getTexCoord_v() const;

	void setColor(const color& rgba);
	void setColor(const uint8_t r, const uint8_t g, uint8_t b, uint8_t a);
	color getColor() const;
	uint8_t getColor_R() const;
	uint8_t getColor_G() const;
	uint8_t getColor_B() const;
	uint8_t getColor_A() const;
};

#endif // VERTEX_H

