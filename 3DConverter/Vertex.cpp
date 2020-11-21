#include "Vertex.h"

template<typename T>
conv::Vertex<T>::Vertex(const uint64_t idx):
	m_id(idx),m_position(), m_normal(), m_texCoord(), m_color()
{
}

template<typename T>
conv::Vertex<T>::Vertex(const uint64_t idx, const T x, const T y, const T z):
	m_id(idx), m_position({x,y,z}), m_normal(), m_texCoord(), m_color()
{
}

template<typename T>
uint64_t conv::Vertex<T>::getID() const
{
	return m_id;
}

template<typename T>
void conv::Vertex<T>::setPos(const pos3D<T>& position)
{
	m_position = position;
}

template<typename T>
void conv::Vertex<T>::setPos(const T x, const T y, const T z)
{
	m_position = { x, y, z };
}

template<typename T>
conv::pos3D<T> conv::Vertex<T>::getPos() const
{
	return m_position;
}

template<typename T>
void conv::Vertex<T>::setPos_X(const T x)
{
	m_position[0] = x;
}

template<typename T>
void conv::Vertex<T>::setPos_Y(const T y)
{
	m_position[1] = y;
}

template<typename T>
void conv::Vertex<T>::setPos_Z(const T z)
{
	m_position[2] = z;
}

template<typename T>
T conv::Vertex<T>::getPos_X() const
{
	return m_position[0];
}

template<typename T>
T conv::Vertex<T>::getPos_Y() const
{
	return m_position[1];
}

template<typename T>
T conv::Vertex<T>::getPos_Z() const
{
	return m_position[2];
}

template<typename T>
void conv::Vertex<T>::setNormal(const pos3D<T>& coord)
{
	m_normal = coord;
}

template<typename T>
void conv::Vertex<T>::setNormal(const T i, const T j, const T k)
{
	m_normal = {i, j, k};
}

template<typename T>
conv::pos3D<T> conv::Vertex<T>::getNormal() const
{
	return m_normal;
}

template<typename T>
T conv::Vertex<T>::getNormal_i() const
{
	return m_normal[0];
}

template<typename T>
T conv::Vertex<T>::getNormal_j() const
{
	return m_normal[1];
}

template<typename T>
T conv::Vertex<T>::getNormal_k() const
{
	return m_normal[2];
}

template<typename T>
void conv::Vertex<T>::setTexCoord(const pos2D<T>& coordinates)
{
	m_texCoord = coordinates;
}

template<typename T>
void conv::Vertex<T>::setTexCoord(const T u, const T v)
{
	m_texCoord = {u, v};
}

template<typename T>
conv::pos2D<T> conv::Vertex<T>::getTexCoord() const
{
	return m_texCoord;
}

template<typename T>
T conv::Vertex<T>::getTexCoord_u() const
{
	return m_texCoord[0];
}

template<typename T>
T conv::Vertex<T>::getTexCoord_v() const
{
	return m_texCoord[1];
}

template<typename T>
void conv::Vertex<T>::setColor(const color& rgba)
{
	m_color = rgba;
}

template<typename T>
void conv::Vertex<T>::setColor(const uint8_t r, const uint8_t g, uint8_t b, uint8_t a)
{
	m_color = {r, g, b, a};
}

template<typename T>
conv::color conv::Vertex<T>::getColor() const
{
	return m_color;
}

template<typename T>
uint8_t conv::Vertex<T>::getColor_R() const
{
	return m_color[0];
}

template<typename T>
uint8_t conv::Vertex<T>::getColor_G() const
{
	return m_color[1];
}

template<typename T>
uint8_t conv::Vertex<T>::getColor_B() const
{
	return m_color[2];
}

template<typename T>
uint8_t conv::Vertex<T>::getColor_A() const
{
	return m_color[3];
}

template<typename T>
inline conv::Vertex<T>& conv::Vertex<T>::operator-(const Vertex<T>& other)
{
	m_position[0] = m_position[0] - other.getPos_X();
	m_position[1] = m_position[1] - other.getPos_Y();
	m_position[2] = m_position[2] - other.getPos_Z();

	return *this;
}

template<typename T>
conv::Vertex<T>::operator bool() const
{
	return (getPos_X() + getPos_Y() + getPos_Z()) != 0;
}
