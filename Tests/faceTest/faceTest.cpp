#include "pch.h"
#include "CppUnitTest.h"
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<> std::wstring ToString(const std::unordered_set<uint64_t>& set)
	{
		std::wostringstream str(L"<");
		int i = 0;
		for (auto el : set)
		{
			str << std::to_wstring(el) << (i < 2 ? L", ": L"");
			++i;
		}
		str << L">";
		return str.str();
	}

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

namespace faceTest
{
	TEST_CLASS(faceTest)
	{
		conv::Mesh3D<> testMesh;
		conv::Vertex<> v1{ 1, 0, 0, 0 };
		conv::Vertex<> v2;
		conv::Vertex<> v3;
	public:
		faceTest():
			testMesh(),v2(2, 1, 1, 0), v3(3, 0, 1, 1)
		{
			testMesh.addVertex(v1);
			testMesh.addVertex(v2);
			testMesh.addVertex(v3);
		}
		
		TEST_METHOD(noDefaultCtor)
		{
			Assert::IsFalse(std::is_default_constructible<conv::Face>::value,
				L"Face class should not have a Default constructor!");
		}

		TEST_METHOD(FaceCtorWithParams)
		{
			uint64_t ID = 1830;
			uint64_t vID1 = 23;
			uint64_t vID2 = 24;
			uint64_t vID3 = 25;
			conv::Face f(ID, vID1, vID2, vID3);
			auto fset = f.getVertIDs();
			Assert::AreEqual(ID, f.getID(), L"Face ID that you set is Not what you get!");
			Assert::AreEqual((size_t)3, fset.size(), L"the given Vertex IDs are different but the stored IDs are not!");
			Assert::AreEqual(std::unordered_set<uint64_t>{vID1, vID2, vID3}, fset, L"the given vIDs are in a wrong order!");
		}

		TEST_METHOD(FaceArea)
		{
			uint64_t ID = 1830;
			conv::Face f(ID, v1.getID(), v2.getID(), v3.getID());
			Assert::AreEqual((100*0.87), std::round(100* f.area(testMesh)), L"face Area is wrong!");
			// ----no side effect ----
			auto fset = f.getVertIDs();
			Assert::AreEqual(ID, f.getID(), L"Face ID that you set is Not what you get!");
			Assert::AreEqual((size_t)3, fset.size(), L"the given Vertex IDs are different but the stored IDs are not!");
			Assert::AreEqual(std::unordered_set<uint64_t>{v1.getID(), v2.getID(), v3.getID()}, fset, L"the given vIDs are in a wrong order!");
		}

		TEST_METHOD(FaceNormal)
		{
			uint64_t ID = 1830;
			conv::Face f(ID, v1.getID(), v2.getID(), v3.getID());
			auto expected = conv::pos3D<>{1, -1, 1};
			auto got = f.normal(testMesh);
			Assert::AreEqual(expected, got, L"Face normal is wrong!");
			// ----no side effect ----
			auto fset = f.getVertIDs();
			Assert::AreEqual(ID, f.getID(), L"Face ID that you set is Not what you get!");
			Assert::AreEqual((size_t)3, fset.size(), L"the given Vertex IDs are different but the stored IDs are not!");
			Assert::AreEqual(std::unordered_set<uint64_t>{v1.getID(), v2.getID(), v3.getID()}, fset, L"the given vIDs are in a wrong order!");
		}

		TEST_METHOD(FaceCenter)
		{
			uint64_t ID = 1830;
			conv::Face f(ID, v1.getID(), v2.getID(), v3.getID());
			auto expected = conv::pos3D<>{ 100 * 0.33, 100 * 0.67, 100 * 0.33 };
			auto got = f.center(testMesh);
			got[0] = std::round(got[0] * 100);
			got[1] = std::round(got[1] * 100);
			got[2] = std::round(got[2] * 100);
			Assert::AreEqual(expected, got, L"Face center is wrong!");
			// ----no side effect ----
			auto fset = f.getVertIDs();
			Assert::AreEqual(ID, f.getID(), L"Face ID that you set is Not what you get!");
			Assert::AreEqual((size_t)3, fset.size(), L"the given Vertex IDs are different but the stored IDs are not!");
			Assert::AreEqual(std::unordered_set<uint64_t>{v1.getID(), v2.getID(), v3.getID()}, fset, L"the given vIDs are in a wrong order!");
		}
	};
}
