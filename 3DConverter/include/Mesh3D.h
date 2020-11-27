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
#include <iostream>
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
	std::pair<T, T>        m_minXmax;
	std::pair<T, T>        m_minYmax;
	std::pair<T, T>        m_minZmax;

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

    Vertex<T> getVertex(uint64_t vID) const;
	Vertex<T>& getVertex(uint64_t vID);

	inline void addFace(const Face& f);
	inline void addFace(Face&& f);
	Face getFace(uint64_t fID) const;


	inline void addShader(shader_e type, const std::string& source);
	inline std::string getShader(shader_e type) const;

	template<typename U>
	inline void addFeature(const std::string& key, U&& value);

	template<typename U> 
	inline U& getFeature(const std::string& key) const;
	std::string listExtraFeatures() const;
	
	inline double area() const;
	inline double volume() const;

	/*
	 * Assumptions: 
	 *  1- All normals are pointing in the correct direction such when viewing
	 * the mesh in a 3D visualizer we don't see artifacts
	 *  2- the mesh is water tight
	 */
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
	auto x = ver.getPos_X();
	auto y = ver.getPos_Y();
	auto z = ver.getPos_Z();
	if (x > m_minXmax.second) m_minXmax.second = x;
	if (x <= m_minXmax.first) m_minXmax.first = x;
	if (y > m_minYmax.second) m_minYmax.second = y;
	if (y <= m_minYmax.first) m_minYmax.first = y;
	if (z > m_minZmax.second) m_minZmax.second = z;
	if (z <= m_minZmax.first) m_minZmax.first = z;

	m_vertArr.insert({ ver.getID(), ver });
}

template<typename T>
inline void conv::Mesh3D<T>::addVertex(Vertex<T>&& ver)
{
	auto x = ver.getPos_X();
	auto y = ver.getPos_Y();
	auto z = ver.getPos_Z();
	if (x > m_minXmax.second) m_minXmax.second = x;
	if (x <= m_minXmax.first) m_minXmax.first = x;
	if (y > m_minYmax.second) m_minYmax.second = y;
	if (y <= m_minYmax.first) m_minYmax.first = y;
	if (z > m_minZmax.second) m_minZmax.second = z;
	if (z <= m_minZmax.first) m_minZmax.first = z;

	m_vertArr.insert({ ver.getID(), std::move(ver) });
}

template<typename T>
conv::Vertex<T> conv::Mesh3D<T>::getVertex(uint64_t vID) const
{
	auto vIt = m_vertArr.find(vID);
	if (vIt == m_vertArr.end())
		return {std::numeric_limits<uint64_t>::max()};
	return vIt->second;
}

template<typename T>
inline conv::Vertex<T>& conv::Mesh3D<T>::getVertex(uint64_t vID)
{
	// a `get` which adds a vertex if it doesn't exist!!
	auto vIt = m_vertArr.find(vID);
	if (vIt == m_vertArr.end())
	{
		Vertex<T> v{ vID };
		addVertex(v);
		return m_vertArr.at(vID);
	}
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
inline std::string conv::Mesh3D<T>::getShader(shader_e type) const
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
	// volume using tetrahedron at origin
	double volume = 0;
	std::vector<pos3D<T>> verts;
	const size_t X = 0;
	const size_t Y = 1;
	const size_t Z = 2;
	
	for (auto fpair : m_faceArr)
	{
		auto vertIDset = fpair.second.getVertIDs();
		for (auto& vID : vertIDset)
		{
			verts.push_back(m_vertArr.at(vID).getPos());
		}
		volume += (1.0 / 6.0) * (-verts[2].at(X) * verts[1].at(Y) * verts[0].at(Z) +
			verts[1].at(X) * verts[2].at(Y) * verts[0].at(Z) +
			verts[2].at(X) * verts[0].at(Y) * verts[1].at(Z)
			- verts[0].at(X) * verts[2].at(Y) * verts[1].at(Z)
			- verts[1].at(X) * verts[0].at(Y) * verts[2].at(Z) +
			verts[0].at(X) * verts[1].at(Y) * verts[2].at(Z));
		verts.clear();
	}
	return volume;
}

template<typename T>
inline bool conv::Mesh3D<T>::isInside(const pos3D<T>& point) const
{
	const T& pointX = point[0];
	const T& pointY = point[1];
	const T& pointZ = point[2];
	// check bounding box
	if (m_minXmax.second < pointX || pointX < m_minXmax.first) return false;
	if (m_minYmax.second < pointY || pointY < m_minYmax.first) return false;
	if (m_minYmax.second < pointZ || pointZ < m_minZmax.first) return false;
	// the three plans algorithm by me
	uint64_t posXface = 0;
	uint64_t negXface = 0;
	uint64_t posYface = 0;
	uint64_t negYface = 0;
	uint64_t posZface = 0;
	uint64_t negZface = 0;
	std::vector<Vertex<T>> vertices;
	const size_t X = 0;
	const size_t Y = 1;
	const size_t Z = 2;
	for (auto& f : m_faceArr)
	{
		auto fCenter = f.second.center(*this);
		auto vertIDset = f.second.getVertIDs();
		auto it = vertIDset.begin();
		vertices.push_back(m_vertArr.at(*it));
		vertices.push_back(m_vertArr.at(*(++it)));
		vertices.push_back(m_vertArr.at(*(++it)));
		int YZplan = 0;
		int XYplan = 0;
		int ZXplan = 0;
		for (auto& v : vertices)
		{
			if (v.getPos_X() < pointX) --YZplan;
			else ++YZplan;

			if (v.getPos_Y() < pointY) --ZXplan;
			else ++ZXplan;

			if (v.getPos_Z() < pointZ) --XYplan;
			else ++XYplan;
		}
		vertices.clear();
		// is this face intersects with the YZ plan at position pointX
		if (std::abs(YZplan) == 1)
		{
			if (fCenter.at(Y) > pointY) // in front of `point` 
			{
				auto it = m_faceArr.find(posYface);
				if (it == m_faceArr.end())
					posYface = f.first;
				else
					if (fCenter.at(Y) < it->second.center(*this).at(Y))
						posYface = f.first;
			}
			else // at back of `point`
			{
				auto it = m_faceArr.find(negYface);
				if (it == m_faceArr.end())
					negYface = f.first;
				else
					if (fCenter.at(Y) > it->second.center(*this).at(Y))
						negYface = f.first;
			}

			if (fCenter.at(Z) > pointZ) // on top of `point`
			{
				auto it = m_faceArr.find(posZface);
				if (it == m_faceArr.end())
					posZface = f.first;
				else
					if (fCenter.at(Z) < it->second.center(*this).at(Z))
						posZface = f.first;
			}
			else // at bottom of `point`
			{
				auto it = m_faceArr.find(negZface);
				if (it == m_faceArr.end())
					negZface = f.first;
				else
					if (fCenter.at(Z) > it->second.center(*this).at(Z))
						negZface = f.first;
			}
		}
		// is this face intersects with the ZX plan at position pointY
		if (std::abs(ZXplan) == 1)
		{
			if (fCenter.at(X) > pointX) // to the right of `point` 
			{
				auto it = m_faceArr.find(posXface);
				if (it == m_faceArr.end())
					posXface = f.first;
				else
					if (fCenter.at(X) < it->second.center(*this).at(X))
						posXface = f.first;
			}
			else // to the left of `point`
			{
				auto it = m_faceArr.find(negXface);
				if (it == m_faceArr.end())
					negXface = f.first;
				else
					if (fCenter.at(X) > it->second.center(*this).at(X))
						negXface = f.first;
			}

			if (fCenter.at(Z) > pointZ) // on top of `point`
			{
				auto it = m_faceArr.find(posZface);
				if (it == m_faceArr.end())
					posZface = f.first;
				else
					if (fCenter.at(Z) < it->second.center(*this).at(Z))
						posZface = f.first;
			}
			else // in bottom of `point`
			{
				auto it = m_faceArr.find(negZface);
				if (it == m_faceArr.end())
					negZface = f.first;
				else
					if (fCenter.at(Z) > it->second.center(*this).at(Z))
						negZface = f.first;
			}
		}
		// is this face intersects with the XY plan at position pointZ
		if (std::abs(XYplan) == 1)
		{
			if (fCenter.at(Y) > pointY) // in front of `point` 
			{
				auto it = m_faceArr.find(posYface);
				if (it == m_faceArr.end())
					posYface = f.first;
				else
					if (fCenter.at(Y) < it->second.center(*this).at(Y))
						posYface = f.first;
			}
			else // back of `point`
			{
				auto it = m_faceArr.find(negYface);
				if (it == m_faceArr.end())
					negYface = f.first;
				else
					if (fCenter.at(Y) > it->second.center(*this).at(Y))
						negYface = f.first;
			}

			if (fCenter.at(X) > pointX) // on top of `point`
			{
				auto it = m_faceArr.find(posXface);
				if (it == m_faceArr.end())
					posXface = f.first;
				else
					if (fCenter.at(X) < it->second.center(*this).at(X))
						posXface = f.first;
			}
			else // in bottom of `point`
			{
				auto it = m_faceArr.find(negXface);
				if (it == m_faceArr.end())
					negXface = f.first;
				else
					if (fCenter.at(X) > it->second.center(*this).at(X))
						negXface = f.first;
			}
		}
	}
	/*
	 *after gathering the six closest faces to the `point` of interest
	 * we check if the normal of this faces points toward the point to convey "outsideness"
	 * or pointing away of it to convey "insideness"
	 * using the vector dot product: if negative result => pointing toward the point, else away of the point
	 */
	// early test: if one of the six faces is missing then Point is outside the Mesh
	if(negXface * posXface * negYface * posYface * negZface * posZface == 0)
		return false;
	std::vector<Face> sixFaces;
	sixFaces.push_back(m_faceArr.at(negXface));
	sixFaces.push_back(m_faceArr.at(posXface));
	sixFaces.push_back(m_faceArr.at(negYface));
	sixFaces.push_back(m_faceArr.at(posYface));
	sixFaces.push_back(m_faceArr.at(negZface));
	sixFaces.push_back(m_faceArr.at(posZface));
	for (auto& face : sixFaces)
	{
		pos3D<T> toPointVec{ face.center(*this).at(X) - point.at(X), face.center(*this).at(Y) - point.at(Y), 
			face.center(*this).at(Z) - point.at(Z) };
		auto dotProd = toPointVec.at(X) * face.normal(*this).at(X) + toPointVec.at(Y) * face.normal(*this).at(Y) +
			toPointVec.at(Z) * face.normal(*this).at(Z);
		if (dotProd < 0)
			return false;
	}
	return true;
}

template<typename T>
void conv::Mesh3D<T>::transformMesh(const Matrix<>& mat)
{
	// FIX ME:
	std::cout << "a simple vector to matrix multiplication\n";
}
#endif // MESH3D_H