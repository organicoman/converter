#include "pch.h"
#include "CppUnitTest.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>	std::wstring __cdecl ToString<conv::pos3D<>>(const conv::pos3D<>& a)
	{
		std::wstring out = std::wstring(L"<") + std::to_wstring(a[0]) + std::wstring(L", ") +
			std::to_wstring(a[1]) + std::wstring(L", ") + std::to_wstring(a[2]) + std::wstring(L" >");
		return out;
	}

	template<>	std::wstring __cdecl ToString<conv::pos2D<>>(const conv::pos2D<>& a)
	{
		std::wstring out = std::wstring(L"<") + std::to_wstring(a[0]) + std::wstring(L", ") +
			std::to_wstring(a[1]) + std::wstring(L" >");
		return out;
	}

	template<>	std::wstring __cdecl ToString<conv::color>(const conv::color& a)
	{
		std::wstring out = std::wstring(L"<") + std::to_wstring(a[0]) + std::wstring(L", ") +
			std::to_wstring(a[1]) + std::wstring(L", ") + std::to_wstring(a[2]) + 
			std::wstring(L", ") + std::to_wstring(a[3]) + std::wstring(L" >");
		return out;
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vertexTest
{
	TEST_CLASS(vertexTest)
	{
	public:
		
		TEST_METHOD(noDefaultCtor)
		{
			Assert::IsFalse(std::is_default_constructible<conv::Vertex<>>::value,
				L"Vertex class should not have a default constructor!");
		}
		TEST_METHOD(CtorwithID)
		{
			uint64_t ID = 1830;
			conv::Vertex<> v(ID);
			Assert::AreEqual(v.getID(), ID, L"failed to get back the Correct ID!");
			Assert::AreEqual(v.getPos(), conv::pos3D<>{0, 0, 0}, L"position not default initialized!");
			Assert::AreEqual(v.getNormal(), conv::pos3D<>{0, 0, 0},L"normal not default initialized!");
			Assert::AreEqual(v.getTexCoord(), conv::pos2D<>{0, 0}, L"texture coordinate not defaulted!");
			Assert::AreEqual(v.getColor(), conv::color{ 0,0,0,0 }, L"color not default initilized!");
		}

		TEST_METHOD(CtorWithXYZ)
		{
			uint64_t ID = 1830;
			double x = 3.14;
			double y = 2.7182;
			double z = 0.214;
			conv::Vertex<> v(ID, x, y, z);
			Assert::AreEqual(v.getID(), ID, L"Ctor with XYZ failed to get the right ID!");
			Assert::AreEqual(v.getPos_X(), x, L"X coordiante <> X coordinate at construction!");
			Assert::AreEqual(v.getPos_Y(), y, L"Y coordinate <> Y coordinate at construction!");
			Assert::AreEqual(v.getPos_Z(), z, L"Z coordinate <> Z coordinate at construction!");
			Assert::AreEqual(v.getPos(), conv::pos3D<>{x, y, z}, L"{X,Y,Z} coordinate in wrong order!");
			// ---- no side effect -----
			Assert::AreEqual(v.getNormal(), conv::pos3D<>{0, 0, 0}, L"normal not default initialized!");
			Assert::AreEqual(v.getTexCoord(), conv::pos2D<>{0, 0}, L"texture coordinate not defaulted!");
			Assert::AreEqual(v.getColor(), conv::color{ 0, 0, 0, 0 }, L"color not default initilized!");
		}

		TEST_METHOD(SetPosGetPos)
		{
			uint64_t ID = 1830;
			double x = 3.14;
			double y = 2.7182;
			double z = 0.214;
			conv::Vertex<> v(ID);
			v.setPos_X(x);
			v.setPos_Y(y);
			v.setPos_Z(z);
			Assert::AreEqual(v.getPos_X(), x, L"set/get `X` coordinate mismatch!");
			Assert::AreEqual(v.getPos_Y(), y, L"set/get `Y` coordinate mismatch!");
			Assert::AreEqual(v.getPos_Z(), z, L"set/get `Z` coordinate mismatch!");
			Assert::AreEqual(v.getPos(), conv::pos3D<>{x, y, z}, L"{X, Y, Z} in wrong order!");
			// ---- no side effect -----
			Assert::AreEqual(v.getNormal(), conv::pos3D<>{0, 0, 0}, L"normal not default initialized!");
			Assert::AreEqual(v.getTexCoord(), conv::pos2D<>{0, 0}, L"texture coordinate not defaulted!");
			Assert::AreEqual(v.getColor(), conv::color{ 0, 0, 0, 0 }, L"color not default initilized!");
		}

		TEST_METHOD(SetGetNormal_ijk)
		{
			uint64_t ID = 1830;
			double i = 3.14;
			double j = 2.7182;
			double k = 0.214;
			conv::Vertex<> v(ID);
			v.setNormal(i, j, k);
			Assert::AreEqual(v.getNormal_i(), i, L"set/get `i` normal component mismatch!");
			Assert::AreEqual(v.getNormal_j(), j, L"set/get `j` normal component mismatch!");
			Assert::AreEqual(v.getNormal_k(), k, L"set/get `k` normal component mismatch!");
			Assert::AreEqual(v.getNormal(), conv::pos3D<>{i, j, k}, L"normal vector coponent in wrong order!");
			// ---- no side effect -----
			Assert::AreEqual(v.getPos(), conv::pos3D<>{0, 0, 0}, L"position not default initialized!");
			Assert::AreEqual(v.getTexCoord(), conv::pos2D<>{0, 0}, L"texture coordinate not defaulted!");
			Assert::AreEqual(v.getColor(), conv::color{ 0, 0, 0, 0 }, L"color not default initilized!");
		}

		TEST_METHOD(SetGetNormal)
		{
			uint64_t ID = 1830;
			double i = 3.14;
			double j = 2.7182;
			double k = 0.214;
			conv::Vertex<> v(ID);
			v.setNormal(conv::pos3D<>{i, j, k});
			Assert::AreEqual(v.getNormal_i(), i, L"set/get `i` normal component mismatch!");
			Assert::AreEqual(v.getNormal_j(), j, L"set/get `j` normal component mismatch!");
			Assert::AreEqual(v.getNormal_k(), k, L"set/get `k` normal component mismatch!");
			Assert::AreEqual(v.getNormal(), conv::pos3D<>{i, j, k}, L"normal vector coponent in wrong order!");
			// ---- no side effect -----
			Assert::AreEqual(v.getPos(), conv::pos3D<>{0, 0, 0}, L"position not default initialized!");
			Assert::AreEqual(v.getTexCoord(), conv::pos2D<>{0, 0}, L"texture coordinate not defaulted!");
			Assert::AreEqual(v.getColor(), conv::color{ 0, 0, 0, 0 }, L"color not default initilized!");
		}

		TEST_METHOD(SetGetColor_rgba)
		{
			uint64_t ID = 1830;
			uint8_t r = 128;
			uint8_t g = 128;
			uint8_t b = 128;
			uint8_t a = 255;
			conv::Vertex<> v(ID);
			v.setColor(r, g, b, a);
			Assert::AreEqual(v.getColor_R(), r, L"set/get `r` color channel mismatch!");
			Assert::AreEqual(v.getColor_G(), g, L"set/get `g` color channel mismatch!");
			Assert::AreEqual(v.getColor_B(), b, L"set/get `b` color channel mismatch!");
			Assert::AreEqual(v.getColor_A(), a, L"set/get `a` color channel mismatch!");
			Assert::AreEqual(v.getColor(), conv::color{r, g, b, a}, L"color channels in wrong order!");
			// ---- no side effect -----
			Assert::AreEqual(v.getPos(), conv::pos3D<>{0, 0, 0}, L"position not default initialized!");
			Assert::AreEqual(v.getNormal(), conv::pos3D<>{0, 0, 0}, L"normal not default initialized!");
			Assert::AreEqual(v.getTexCoord(), conv::pos2D<>{0, 0}, L"texture coordinate not defaulted!");
		}

		TEST_METHOD(SetGetColor)
		{
			uint64_t ID = 1830;
			uint8_t r = 128;
			uint8_t g = 128;
			uint8_t b = 128;
			uint8_t a = 255;
			conv::Vertex<> v(ID);
			v.setColor(conv::color{ r, g, b, a });
			Assert::AreEqual(v.getColor_R(), r, L"set/get `r` color channel mismatch!");
			Assert::AreEqual(v.getColor_G(), g, L"set/get `g` color channel mismatch!");
			Assert::AreEqual(v.getColor_B(), b, L"set/get `b` color channel mismatch!");
			Assert::AreEqual(v.getColor_A(), a, L"set/get `a` color channel mismatch!");
			Assert::AreEqual(v.getColor(), conv::color{ r, g, b, a }, L"color channels in wrong order!");
			// ---- no side effect -----
			Assert::AreEqual(v.getPos(), conv::pos3D<>{0, 0, 0}, L"position not default initialized!");
			Assert::AreEqual(v.getNormal(), conv::pos3D<>{0, 0, 0}, L"normal not default initialized!");
			Assert::AreEqual(v.getTexCoord(), conv::pos2D<>{0, 0}, L"texture coordinate not defaulted!");
		}

		TEST_METHOD(SetGetTextureCoord)
		{
			uint64_t ID = 1830;
			double u = 3.14;
			double v = 2.7182;			
			conv::Vertex<> p(ID);
			p.setTexCoord(conv::pos2D<>{u, v});
			Assert::AreEqual(p.getTexCoord_u(), u, L"set/get `u` texutre coordinate mismatch!");
			Assert::AreEqual(p.getTexCoord_v(), v, L"set/get `v` texture coordinate mismatch!");			
			Assert::AreEqual(p.getTexCoord(), conv::pos2D<>{ u, v}, L"texture coordinates in wrong order!");
			// ---- no side effect -----
			Assert::AreEqual(p.getPos(), conv::pos3D<>{0, 0, 0}, L"position not default initialized!");
			Assert::AreEqual(p.getNormal(), conv::pos3D<>{0, 0, 0}, L"normal not default initialized!");
			Assert::AreEqual(p.getColor(), conv::color{ 0, 0, 0, 0 }, L"color not default initilized!");
		}

		TEST_METHOD(SetGetTextureCoord_uv)
		{
			uint64_t ID = 1830;
			double u = 3.14;
			double v = 2.7182;
			conv::Vertex<> p(ID);
			p.setTexCoord(u, v);
			Assert::AreEqual(p.getTexCoord_u(), u, L"set/get `u` texutre coordinate mismatch!");
			Assert::AreEqual(p.getTexCoord_v(), v, L"set/get `v` texture coordinate mismatch!");
			Assert::AreEqual(p.getTexCoord(), conv::pos2D<>{ u, v}, L"texture coordinates in wrong order!");
			// ---- no side effect -----
			Assert::AreEqual(p.getPos(), conv::pos3D<>{0, 0, 0}, L"position not default initialized!");
			Assert::AreEqual(p.getNormal(), conv::pos3D<>{0, 0, 0}, L"normal not default initialized!");
			Assert::AreEqual(p.getColor(), conv::color{ 0, 0, 0, 0 }, L"color not default initilized!");
		}

		TEST_METHOD(VertexMinusVertex)
		{
			uint64_t ID = 1830;
			double x = 3.14;
			double y = 2.7182;
			double z = 0.214;
			conv::Vertex<> p(ID);
			p.setPos(x, y, z);
			auto diff = p - p;
			Assert::AreEqual(diff[0], 0.0, L"failed to substract the X coordinate!");
			Assert::AreEqual(diff[1], 0.0, L"fialed to substract the Y coordinate!");
			Assert::AreEqual(diff[2], 0.0, L"fialed to substract the Z coordinate!");
			// ---- no side effect -----
			Assert::AreEqual(p.getID(), ID, L"failed to get back the Correct ID!");
			Assert::AreEqual(p.getPos(), conv::pos3D<>{x, y, z}, L"position not default initialized!");
			Assert::AreEqual(p.getNormal(), conv::pos3D<>{0, 0, 0}, L"normal not default initialized!");
			Assert::AreEqual(p.getTexCoord(), conv::pos2D<>{0, 0}, L"texture coordinate not defaulted!");
			Assert::AreEqual(p.getColor(), conv::color{ 0,0,0,0 }, L"color not default initilized!");
		}

		TEST_METHOD(VertexAtOriginBool)
		{
			uint64_t ID = 1830;
			conv::Vertex<> p(ID);
			auto text = ToString(p.getPos()) + std::wstring(L" is Vertex not at Origin?");
			Assert::IsFalse((bool)p, text.c_str());
			// ---- no side effect ----
			Assert::AreEqual(p.getID(), ID, L"failed to get back the Correct ID!");
			Assert::AreEqual(p.getPos(), conv::pos3D<>{0, 0, 0}, L"position not default initialized!");
			Assert::AreEqual(p.getNormal(), conv::pos3D<>{0, 0, 0}, L"normal not default initialized!");
			Assert::AreEqual(p.getTexCoord(), conv::pos2D<>{0, 0}, L"texture coordinate not defaulted!");
			Assert::AreEqual(p.getColor(), conv::color{ 0,0,0,0 }, L"color not default initilized!");
		}
	};
}
