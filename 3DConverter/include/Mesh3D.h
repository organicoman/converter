#pragma once
#ifndef MESH3D_H
#define MESH3D_H
//--------STL-----------
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <execution>
#include <limits>
//----------------------
#include "Namespace.h"
#include "Face.h"

template<typename T>
class conv::Mesh3D
{
public:
	using VArr_t = std::unordered_map<uint64_t, conv::Vertex<T>>;
	using FArr_t = std::unordered_map<uint64_t, conv::Face>;
private:
	uint64_t               m_id;
	VArr_t                 m_vertArr;
	FArr_t                 m_faceArr;
	std::string            m_vertShader;
	std::string            m_fragShader;
	std::string            m_geomShader;
	json                   m_extrafeatures;

public:
	Mesh3D() = default;
	explicit Mesh3D(uint64_t id);

	uint64_t getID() const;

	bool isVertexEmpty() const { return !m_vertArr.size(); }
	void clearVertex() { m_vertArr.clear(); }
	size_t Vsize() const { return m_vertArr.size(); }

	bool isFaceEmpty() const { return !m_faceArr.size(); }
	void clearFaces() { m_faceArr.clear(); }
	size_t Fsize() const { return m_faceArr.size(); }

	FArr_t::const_iterator fBegin() const;
	FArr_t::const_iterator fEnd() const;

	inline void addVertex(const Vertex<T>& ver);
	inline void addVertex(Vertex<T>&& ver);

    Vertex<T>& getVertex(uint64_t vID) const;

	inline void addFace(const Face& f);
	inline void addFace(Face&& f);
	Face getFace(uint64_t fID) const;


	inline void addShader(shader_e type, const std::string& source);
	inline std::string& getShader(shader_e type) const;

	template<typename U>
	inline void addFeature(const std::string& key, U&& value);

	template<typename U> 
	inline U& getFeature(const std::string& key) const;
	std::string listExtraFeatures() const;
	
	inline double area() const;
	inline double volume() const;
	inline bool isInside(const pos3D<T>& point) const;

	void transformMesh(const Matrix<>& mat);
	// void applyVertexShader();
};

template<typename T>
inline conv::Mesh3D<T>::Mesh3D(uint64_t id) :
	m_id(id), m_vertArr(), m_faceArr(), m_vertShader(), m_fragShader(),
	m_geomShader(), m_extrafeatures()
{
}

template<typename T>
inline uint64_t conv::Mesh3D<T>::getID() const
{
	return m_id;
}

template<typename T>
inline conv::Mesh3D<T>::FArr_t::const_iterator conv::Mesh3D<T>::fBegin() const
{
	return m_faceArr.cbegin();
}

template<typename T>
inline conv::Mesh3D<T>::FArr_t::const_iterator conv::Mesh3D<T>::fEnd() const
{
	return m_faceArr.cend();
}

template<typename T>
inline void conv::Mesh3D<T>::addVertex(const Vertex<T>& ver)
{
	m_vertArr.insert({ ver.getID(), ver });
}

template<typename T>
inline void conv::Mesh3D<T>::addVertex(Vertex<T>&& ver)
{
	m_vertArr.insert({ ver.getID(), std::move(ver) });
}

template<typename T>
conv::Vertex<T>& conv::Mesh3D<T>::getVertex(uint64_t vID) const
{
	auto vIt = m_vertArr.find(vID);
	if (vIt == m_vertArr.end())
		return {std::numeric_limits<uint64_t>::max()};
	return vIt->second;
}

template<typename T>
inline void conv::Mesh3D<T>::addFace(const Face& f)
{
	m_faceArr.insert({f.getID(), f});
}

template<typename T>
inline void conv::Mesh3D<T>::addFace(Face&& f)
{
	m_faceArr.insert({ f.getID(), std::move(f) });
}

template<typename T>
conv::Face conv::Mesh3D<T>::getFace(uint64_t fID) const
{
	auto fIt = m_faceArr.find(fID);
	if (fIt == m_faceArr.end())
		return {0,0,0,0};
	return fIt->second;
}

template<typename T>
inline void conv::Mesh3D<T>::addShader(shader_e type, const std::string& source)
{
	switch (type)
	{
	case conv::VERTEX_SHD:
		m_vertShader = source;
		break;
	case conv::GEOMETRY_SHD:
		m_geomShader = source;
		break;
	case conv::FRAGMENT_SHD:
		m_fragShader = source;
		break;
	default:
		break;
	}
}

template<typename T>
inline std::string& conv::Mesh3D<T>::getShader(shader_e type) const
{
	switch (type)
	{
	case conv::VERTEX_SHD:
		return m_vertShader;

	case conv::GEOMETRY_SHD:
		return m_geomShader;

	case conv::FRAGMENT_SHD:
		return m_fragShader;

	default:
		break;
	}
	return {};
}

template<typename T>
template<typename U>
inline void conv::Mesh3D<T>::addFeature(const std::string& key, U&& value)
{
	m_extrafeatures[key] = value;
	return;
}

template<typename T>
template<typename U>
inline U& conv::Mesh3D<T>::getFeature(const std::string& key) const
{
	auto val = m_extrafeatures.at(key).get<U>();
	return val;
}

template<typename T>
std::string conv::Mesh3D<T>::listExtraFeatures() const
{
	return std::string(m_extrafeatures.dump());
}

template<typename T>
inline double conv::Mesh3D<T>::area() const
{
	/*auto BinOp = [&](const Face& f1, const Face& f2)
	{ return std::abs(f1.area(*this)) + std::abs(f2.area(*this)); };*/
	double surface = 0.0;
	for (auto f : m_faceArr)
		surface += f.second.area(*this);
	/*return std::reduce(m_faceArr.begin(), m_faceArr.end(), 0, BinOp);*/
	return surface;
}

template<typename T>
inline double conv::Mesh3D<T>::volume() const
{
	// FIX ME:
	return 0.0;
}

template<typename T>
inline bool conv::Mesh3D<T>::isInside(const pos3D<T>& point) const
{
	// FIX ME:
	return false;
}

template<typename T>
void conv::Mesh3D<T>::transformMesh(const Matrix<>& mat)
{
	// FIX ME:
}
#endif // MESH3D_H