#pragma once
#ifndef NAMESPACE_H
#define NAMESPACE_H

//---------STL---------
#include <array>
#include <cstdint>

//---------------------

//#include "../thirdParty/json.hpp"

//using json = nlohmann::json;

class json {}; // FIX ME: to be replaced by 'nlohmann::json'

namespace conv
{
	enum shader_t {VERTEX_SHD, GEOMETRY_SHD, FRAGMENT_SD};
	
	template<typename T = double> using Matrix = std::array<std::array<T, 3>, 3>;
	using Matrixf = Matrix<float>;

	template<typename T = double> using pos3D = std::array<T, 3>;
	using pos3Df = pos3D<float>;

	template<typename T = double> using pos2D = std::array<T, 2>;
	using pos2Df = pos2D<float>;

	using color = std::array<uint8_t, 4>;

	template<typename T = double> class Vertex;
	using Vertexf = Vertex<float>;

	class Face;

	template<typename T= double> class Mesh3D;
	using Mesh3Df = Mesh3D<float>;

	class Converter;

	struct Factory;

	class Serializer;
	class Deserializer;
}
#endif //NAMESPACE_H