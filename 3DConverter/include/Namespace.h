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
#include "../../packages/nlohmann.json.3.9.1/build/native/include/nlohmann/json.hpp"

using json = nlohmann::json;

namespace conv
{
	constexpr size_t BIG_SIZE = 500'000;
	enum shader_e {VERTEX_SHD, GEOMETRY_SHD, FRAGMENT_SHD};

	enum feature_e {VERTEX, VNORMAL, VTEXTCOORD, VCOLOR, FACE};
	constexpr auto vertex = u8"vertex";
	constexpr auto normal = u8"vertexNormal";
	constexpr auto face = u8"face";
	constexpr auto textCoord = u8"textureCoord";
	constexpr auto vertColor = u8"vertexColor";

	enum RET_CODE {REPLACED = -1, ADDED = 1};
	
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

	using parser_t = std::function<bool(const std::string&, const std::string&, Mesh3D<>&)>;
	using stamper_t = std::function<void(const Mesh3D<>&, const std::string&, std::ostream&)>;
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
		Container prev_word(splitter<Container>& self, typename Container::iterator)
		{
			static_assert(has_value_type_member<Container>::value);
			auto rbeg = std::make_reverse_iterator(self.rend_pos);

			if (rbeg == std::rend(self.m_c))
			{
				self.m_currWord = Container{};
				return self.m_currWord;
			}
			 
			while (*rbeg == self.m_sep)
			{
				rbeg = std::next(rbeg);
				if (rbeg == std::rend(self.m_c))
				{
					self.m_currWord = Container{};
					return self.m_currWord;
				}
			}
			self.rend_pos = rbeg.base();
			self.end_pos = rbeg.base();

			while (rbeg != std::rend(self.m_c) and *rbeg != self.m_sep)
				rbeg = std::next(rbeg);
			self.rend_pos = rbeg.base();

			if (self.isFullsize())
			{
				self.m_currWord = Container{};
				return self.m_currWord;
			}

			self.m_currWord = Container{ self.rend_pos, self.end_pos};
			return self.m_currWord;
		}

		template<typename Container>
		Container prev_word(splitter<Container>& self, std::input_iterator_tag)
		{
			static_assert(has_value_type_member<Container>::value);
			return Container{};
		}

		template<typename Container>
		void reset(splitter<Container>& self, typename Container::iterator)
		{
			self.end_pos = self.start;
			self.rend_pos = self.start;
			self.m_currWord = {};
		}

		template<typename Container>
		void reset(splitter<Container>& self, std::input_iterator_tag)
		{
			return;
		}
	};

	template<typename Container>
	class splitter
	{
		using v_t = typename Container::value_type;
		using C_iter = typename Container::iterator;
		using C_diff_t = typename Container::difference_type;

		const Container& m_c;
		v_t m_sep;
		const C_iter start;
		C_iter end_pos;
		C_iter rend_pos;
		Container m_currWord;
			   
		friend Container ImplDetail::prev_word<Container>(splitter<Container>& self, C_iter);
		friend void ImplDetail::reset<Container>(splitter<Container>& self, C_iter);
	public:
		splitter() = delete;
		splitter(const Container& C, v_t&& sep, const C_iter& beg):
			m_c(C), m_sep(sep), start(beg), end_pos(beg), rend_pos(beg), m_currWord()
		{
		}

		splitter(const splitter& other) = default; // deleted by default
		splitter(splitter&& other) = default; // deleted by default?

		splitter& operator =(const splitter& other) = delete; // becuase we are using  `m_c` as a reference to a Container
		splitter& operator =(splitter&& other) = delete; // and `m_sep` is a constant value cannot be re-assigned

		Container curr_word() const
		{
			return m_currWord;
		}

		bool isFullsize() const
		{
			auto currentSz = end_pos - rend_pos;
			auto size = std::end(m_c) - std::begin(m_c);
			return currentSz == size;
		}

		v_t getSeperator() const
		{
			return m_sep;
		}

		void setSepartor(const v_t& newSep)
		{
			m_sep = newSep;
			return;
		}

		void rewind()
		{
			return ImplDetail::reset(*this, C_iter{});
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
			rend_pos = end_pos;
			while (*end_pos != m_sep and end_pos != std::end(m_c))
				end_pos = std::next(end_pos);

			// if the container doesn't contain the separator then return empty
			if (isFullsize())
			{
				m_currWord = Container{};
				return m_currWord;
			}

			m_currWord = Container{ rend_pos, end_pos };
			return m_currWord;
		}
		
		Container prev_word()
		{
			return ImplDetail::prev_word(*this, C_iter{});
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