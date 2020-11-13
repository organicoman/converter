#pragma once
#ifndef MESH3D_H
#define MESH3D_H
//--------STL-----------
#include <vector>
#include <string>

//----------------------
#include "Namespace.h"

template<typename T>
class conv::Mesh3D
{
	uint64_t               m_id;
	std::vector<Vertex<T>> m_vertArr;
	std::vector<Face>      m_faceArr;
	std::string            m_vertShader;
	std::string            m_fragShader;
	std::string            m_geomShader;
	json                   m_features;

public:
	Mesh3D();
	explicit Mesh3D(uint64_t id);

	inline void addVertex(const Vertex<T>& ver);
	inline void addVertex(Vertex<T>&& ver);

	inline Vertex<T> getVertex(uint64_t vID) const;

	inline void addFace(const Face& f);
	inline void addFace(Face&& f);
	inline Face getFace(uint64_t fID) const;


	inline void addShader(shader_t type, const std::string& source);
	inline std::string getShader(const shader_t& type) const;

	template<typename T>
	inline void addFeature(const std::string& key, T&& value);

	template<typename T> 
	inline T getFeature(const std::string& key) const;
	std::vector<std::string> listAllFeatures() const;
	
	inline double area() const;
	inline double volume() const;
	inline bool isInside(const pos3D<T>& point) const;

	void transformMesh(const Matrix<>& mat);
	// void applyVertexShader();
};

#endif // MESH3D_H