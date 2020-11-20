#include <algorithm>
#include <execution>

#include "Mesh3D.h"
#include "Face.h"
#include "Vertex.h"


template<typename T>
inline conv::Mesh3D<T>::Mesh3D(uint64_t id):
	m_id(id), m_vertArr(), m_faceArr(), m_vertShader(), m_fragShader(),
	m_geomShader(), m_extrafeatures()
{
}

template<typename T>
inline void conv::Mesh3D<T>::addVertex(const Vertex<T>& ver)
{
	m_vertArr.push_back(ver);
}

template<typename T>
inline void conv::Mesh3D<T>::addVertex(Vertex<T>&& ver)
{
	m_vertArr.push_back(ver);
}

template<typename T>
inline conv::Vertex<T> conv::Mesh3D<T>::getVertex(uint64_t vID) const
{
	auto v = m_vertArr[vID];
	if ( v.getID() == vID)
		return v;
	// assume that the vertices are in random order
	auto lambda = [](const Vertex<T>&) p) { return p.getID() == vID; };
	if (m_vertArr.size() > BIG_SIZE)
	{
		auto it = std::find(std::execution::par_unseq, 
			m_vertArr.begin(), m_vertArr.end(), lambda);
	}
	else
	{
		auto it = std::find(m_vertArr.begin(), m_vertArr.end(), lambda);
	}
	return *it;
}

template<typename T>
inline void conv::Mesh3D<T>::addFace(const Face& f)
{
	m_faceArr.push_back(f);
}

template<typename T>
inline void conv::Mesh3D<T>::addFace(Face&& f)
{
	m_faceArr.push_back(f);
}

template<typename T>
inline conv::Face conv::Mesh3D<T>::getFace(uint64_t fID) const
{
	auto f = m_faceArr[fID];
	if (f.getID() == fID)
		return f;
	// assume that the faces are in random order
	auto lambda = [](const Face& p) { return p.getID() == fID; };
	if (m_faceArr.size() > BIG_SIZE)
	{
		auto it = std::find(std::execution::par_unseq,
			m_faceArr.begin(), m_faceArr.end(), lambda);
	}
	else
	{
		auto it = std::find(m_faceArr.begin(), m_faceArr.end(), lambda);
	}
	return *it;
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
	case conv::FRAGMENT_SD:
		m_fragShader = source;
		break;
	default:
		break;
	}
}

template<typename T>
inline std::string conv::Mesh3D<T>::getShader(shader_e type) const
{
	switch (type)
	{
	case conv::VERTEX_SHD:
		return m_vertShader;

	case conv::GEOMETRY_SHD:
		return m_geomShader;

	case conv::FRAGMENT_SD:
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
inline U conv::Mesh3D<T>::getFeature(const std::string& key) const
{
	auto val = m_extrafeatures.at(key).get<U>();
	return val;
}

template<typename T>
std::string conv::Mesh3D<T>::listExtraFeatures() const
{
	return m_extrafeatures.dump();
}

template<typename T>
inline double conv::Mesh3D<T>::area() const
{
	auto BinOp =[&](const Face& f1, const Face& f2) 
	            { return std::abs(f1.area(*this)) + std::abs(f2.area(*this)); };
	
	return std::reduce(m_faceArr.begin(), m_faceArr.end(), 0, BinOp);
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
