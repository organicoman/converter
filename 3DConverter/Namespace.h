#pragma once
#ifndef NAMESPACE_H
#define NAMESPACE_H

//---------STL---------
#include <array>
#include <vector>
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <iterator>

//---------------------
#include "../packages/nlohmann.json.3.9.1/build/native/include/nlohmann/json.hpp"

using json = nlohmann::json;

namespace conv
{
	enum shader_e {VERTEX_SHD, GEOMETRY_SHD, FRAGMENT_SD};

	enum feature_e {VERTEX, VNORMAL, VTEXTCOORD, VCOLOR, FACE};
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
	
	template<typename Container> class splitter; //forward declaraction

	namespace ImplDetail
	{
		template<typename Container>
		Container prev_word(splitter<Container>& self, ...)
		{
			//static_assert(has_value_type_member<Container>::value);

			if (self.end_pos == std::begin(self.m_c))
				return Container{};
			while (*self.end_pos == self.m_sep)
			{
				self.end_pos = std::prev(self.end_pos);
				if (self.end_pos == std::begin(self.m_c))
					return Container{};
			}
			auto beg = self.end_pos;
			self.end_pos = std::next(self.end_pos); // goback to the sep position
			while (beg != std::begin(self.m_c) and *beg != self.m_sep)
				beg = std::prev(beg);
			return Container{ beg, self.end_pos};
		}

		template<typename Container>
		Container prev_word(splitter<Container>& self, std::input_iterator_tag)
		{
			//static_assert(has_value_type_member<Container>::value);
			return Container{};
		}
	};

	template<typename Container>
	class splitter
	{
		using v_t = typename Container::value_type;
		using C_iter = typename Container::iterator;
		using C_diff_t = typename Container::difference_type;

		const Container& m_c;
		const v_t m_sep;
		C_iter end_pos;
		Container m_currWord;

		template<typename C>
		friend C ImplDetail::prev_word(splitter<C>& self, ...);

		template<typename C>
		friend C ImplDetail::prev_word(splitter<C>& self, std::input_iterator_tag);
		
	public:
		splitter() = delete;
		explicit splitter(const Container& C, v_t&& sep, const C_iter& beg):
			m_c(C), m_sep(sep), end_pos(beg), m_currWord()
		{
		}

		splitter(const splitter& other) = default;
		splitter(splitter&& other) = default;

		splitter& operator =(const splitter& other) = delete; // becuase we are using  `m_c` as a reference to a Container
		splitter& operator =(splitter&& other) = delete; // and `m_sep` is a constant value cannot be re-assigned

		Container curr_word() const
		{
			return m_currWord;
		}

		Container next_word()
		{
			if (end_pos == std::end(m_c))
			{
				m_currWord = Container{};
				return m_currWord;
			}
			while (*end_pos == m_sep)
			{
				end_pos = std::next(end_pos);
				if (end_pos == std::end(m_c))
				{
					m_currWord = Container{};
					return m_currWord;
				}
			}
			auto beg = end_pos;
			while (*end_pos != m_sep and end_pos != std::end(m_c))
				end_pos = std::next(end_pos);
			m_currWord = Container{ beg, end_pos };
			return m_currWord;
		}
		
		Container prev_word()
		{
			return ImplDetail::prev_word(*this, typename std::iterator_traits<C_iter>::iterator_category());
		}

		splitter& operator >> (Container& dest)
		{
			dest = next_word();
			return *this;
		}

		splitter& operator + (const C_diff_t & wordAt)
		{
			auto temp = wordAt;
			while (temp--)
			{
				if (next_word() == Container{})
					break;
			}
			return *this;
		}

		splitter& operator += (const C_diff_t & wordAt)
		{
			return *this + wordAt;
		}
	};
}
#endif //NAMESPACE_H