#include "pch.h"
#include "CppUnitTest.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<> std::wstring ToString(const std::unordered_set<uint64_t>& set)
	{
		std::wostringstream str;
		str << L"<";
		int i = 0;
		for (auto el : set)
		{
			str << std::to_wstring(el) << (i < 2 ? L", " : L"");
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

	template<> std::wstring __cdecl ToString<conv::Vertex<double>>(const conv::Vertex<>& a)
	{
		std::wostringstream out;
		out << L"Vertex<" << a.getID() << L", {" << ToString(a.getPos()) << L"}";
		return out.str();
	}

	template<> std::wstring __cdecl ToString<conv::Face>(const conv::Face& a)
	{
		std::wostringstream out;
		out << L"Face<" << a.getID() << L", {" << ToString(a.getVertIDs()) << L"}";
		return out.str();
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace meshTest
{
	TEST_CLASS(meshTest)
	{
		conv::Mesh3D<> testMesh{1830};
		conv::Vertex<> v1{ 1, 1, 0, 0 };
		conv::Vertex<> v2{ 2, 0, 1, 1 };
		conv::Vertex<> v3{ 3, 1, 1, 0 };
	
	public:

		meshTest()
		{

		}
		
		TEST_METHOD(defaultCtor)
		{
			Assert::IsTrue(std::is_default_constructible<conv::Mesh3D<>>::value, L"Mesh3D shoudl be default constructible!");
			// ---- no side effect ----
			conv::Mesh3D<> m;
			Assert::AreEqual((uint64_t)0, m.getID(), L"defautl Construction should be null!");
			Assert::IsTrue(m.isVertexEmpty(), L"at Construction no Vertex available!");
			Assert::IsTrue(m.isFaceEmpty(), L"at Construction no Face available!");
			Assert::AreEqual(std::string{}, m.getShader(conv::shader_e::VERTEX_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, m.getShader(conv::shader_e::GEOMETRY_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, m.getShader(conv::shader_e::FRAGMENT_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{"null"}, m.listExtraFeatures(), L"default Construction should be empty!");	
		}

		TEST_METHOD(CtorWithParam)
		{
			uint64_t ID = 1830;
			conv::Mesh3D<> m(ID);
			Assert::AreEqual(ID, m.getID(), L"failed to initialize the ID correctly!");
			// ---- no side effect ----
			Assert::IsTrue(m.isVertexEmpty(), L"at Construction no Vertex available!");
			Assert::IsTrue(m.isFaceEmpty(), L"at Construction no Face available!");
			Assert::AreEqual(std::string{}, m.getShader(conv::shader_e::VERTEX_SHD), L"vertex shader:default Construction should be empty!");
			Assert::AreEqual(std::string{}, m.getShader(conv::shader_e::GEOMETRY_SHD), L"geometry shader:default Construction should be empty!");
			Assert::AreEqual(std::string{}, m.getShader(conv::shader_e::FRAGMENT_SHD), L"fragment shader :default Construction should be empty!");
			Assert::AreEqual(std::string{ "null" }, m.listExtraFeatures(), L"default Construction should be empty!");
		}

		TEST_METHOD(MeshIsVertexEmpty)
		{
			uint64_t ID = 1830;
			conv::Mesh3D<> m(ID);
			m.clearVertex();
			Assert::IsTrue(m.isVertexEmpty(), L"after a clear operation, the Mesh Verties should be empty!");
			// ---- no side effect ----
			Assert::AreEqual(ID, m.getID(), L"failed to initialize the ID correctly!");
			Assert::IsTrue(m.isFaceEmpty(), L"at Construction no Face available!");
			Assert::AreEqual(std::string{}, m.getShader(conv::shader_e::VERTEX_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, m.getShader(conv::shader_e::GEOMETRY_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, m.getShader(conv::shader_e::FRAGMENT_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{ "null" }, m.listExtraFeatures(), L"default Construction should be empty!");
		}

		TEST_METHOD(MeshIsFaceEmpty)
		{
			uint64_t ID = 1830;
			conv::Mesh3D<> m(ID);
			m.clearFaces();
			Assert::IsTrue(m.isFaceEmpty(), L"after a clear operation, the Mesh Faces should be empty!");
			// ---- no side effect ----
			Assert::AreEqual(ID, m.getID(), L"failed to initialize the ID correctly!");
			Assert::IsTrue(m.isVertexEmpty(), L"default Construction should no Vertecies be available!");
			Assert::AreEqual(std::string{}, m.getShader(conv::shader_e::VERTEX_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, m.getShader(conv::shader_e::GEOMETRY_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, m.getShader(conv::shader_e::FRAGMENT_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{ "null" }, m.listExtraFeatures(), L"default Construction should be empty!");
		}

		TEST_METHOD(MeshAddVertex)
		{
			uint64_t ID = 1830;
			conv::Vertex<> p(ID);
			testMesh.addVertex(std::move(p));

			Assert::AreEqual(v1, testMesh.getVertex(this->v1.getID()), L"Failed to get the same Vertex!");
			Assert::AreEqual(p, testMesh.getVertex(p.getID()), L"Failed to get the same Vertex!");//this is a move from variable??
			Assert::IsFalse(testMesh.isVertexEmpty(), L"Function Implementation needs to be checked!");
			// ---- no side effect ----
			Assert::AreEqual(ID, testMesh.getID(), L"failed to initialize the ID correctly!");
			Assert::IsTrue(testMesh.isFaceEmpty(), L"at Construction no Face available!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::VERTEX_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::GEOMETRY_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::FRAGMENT_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{ "null" }, testMesh.listExtraFeatures(), L"default Construction should be empty!");
		}

		TEST_METHOD(MeshAddFace)
		{
			uint64_t ID = 1830;
			conv::Face f(ID, v1.getID(), v2.getID(), v3.getID());
			testMesh.addFace(f);
			conv::Face p(ID+1, v2.getID(), v1.getID(), v3.getID());
			testMesh.addFace(std::move(p));

			Assert::AreEqual(testMesh.getFace(f.getID()), f, L"Failed to get the same Face!");
			Assert::AreNotEqual(testMesh.getFace(p.getID()), p, L"Failed to get the same Vertex!");//this is a move from variable??
			Assert::IsFalse(testMesh.isFaceEmpty(), L"Function Implementation needs to be checked!");
			// ---- no side effect ----
			Assert::AreEqual(ID, testMesh.getID(), L"failed to initialize the ID correctly!");
			Assert::IsFalse(testMesh.isFaceEmpty(), L"at Construction no Face available!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::VERTEX_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::GEOMETRY_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::FRAGMENT_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{ "null" }, testMesh.listExtraFeatures(), L"default Construction should be empty!");
		}
	};
}
