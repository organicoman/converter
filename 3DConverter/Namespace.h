#pragma once
#ifndef NAMESPACE_H
#define NAMESPACE_H

//---------STL---------
#include <array>
#include <cstdint>
#include <iostream>
#include <type_traits>

//---------------------

#include "../packages/nlohmann.json.3.9.1/build/native/include/nlohmann/json.hpp"

using json = nlohmann::json;



namespace conv
{
	enum shader_t {VERTEX_SHD, GEOMETRY_SHD, FRAGMENT_SD};

	constexpr auto vertex = u8"vertex";
	constexpr auto normal = u8"vertexNormal";
	constexpr auto face = u8"face";
	constexpr auto textCoord = u8"textureCoord";
	constexpr auto vertColor = u8"vertexColor";

	
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

	// ---- some template wizardery ------

	// checks if this type hase a member typedef called '::value_type'
	template<typename T, class = void>
	struct has_value_type_member : std::false_type {};
	template<typename T>
	struct has_value_type_member <T, std::void_t<typename T::value_type>> : std::true_type {};

	// selects one of the two types depending on a boolean value
	template<bool, typename T, typename V>
	struct select_if { typedef V value_type; };

	template<typename T, typename V>
	struct select_if <true, T, V> { typedef T value_type;  };

	template<bool b, typename T, typename V>
	using select_if_t = typename select_if<b, T, V>::value_type;

	// ----- end of Template wizardery --------

	// an algorithm to return next(/previous) word in a container given a separator
	// a word: is a sequence of contained value_type bounded by two separator expl: {sep, value_type, value_type,..., sep}
	
	namespace ImplDetail
	{
		template<typename Container>
		Container prev_word(const Container& C, typename Container::iterator& beg, const typename Container::value_type& sep, ...)
		{
			static_assert(has_value_type_member<Container>::value);

			if (beg == std::begin(C))
				return Container{};
			while (*beg == sep)
			{
				beg = std::prev(beg);
				if (beg == std::begin(C))
					return Container{};
			}
			auto end = std::next(beg);
			while (beg != std::begin(C) and *beg != sep)
				beg = std::prev(beg);
			return Container{ beg, end };
		}

		template<typename Container>
		Container prev_word(const Container& C, typename Container::iterator& beg, const typename Container::value_type& sep, std::input_iterator_tag)
			{
				static_assert(has_value_type_member<Container>::value);
				return Container{};
			}
	};

	template<typename Container>
	class splitter
	{
		using v_t = typename Container::value_type;
		using C_iter = typename Container::iterator;

		const Container& m_c;
		const v_t m_sep;
		C_iter end_pos;

	public:
		splitter() = delete;
		explicit splitter(const Container& C, v_t&& sep, const C_iter& beg) :
			m_c(C), m_sep(sep), end_pos(beg)
		{

		}

		splitter(const splitter& other) = default;
		splitter(splitter&& other) = default;

		Container next_word()
		{
			if (end_pos == std::end(m_c))
				return Container{};
			while (*end_pos == m_sep)
			{
				end_pos = std::next(end_pos);
				if (end_pos == std::end(m_c))
					return Container{};
			}
			auto beg = end_pos;
			while (*end_pos != m_sep and end_pos != std::end(m_c))
				end_pos = std::next(end_pos);
			return Container{ beg, end_pos };
		}
		template<typename Iterator, bool = false>
		Container prev_word()
		{
			return Container{};
		}

		Container prev_word()
		{
			return ImplDetail::prev_word(m_c, end_pos, m_sep, typename std::iterator_traits<C_iter>::iterator_category());
		}
	};


}
#endif //NAMESPACE_H