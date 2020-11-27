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
		conv::Vertex<> v1{ 1, -20.0, -20.0, 0.0 };
		conv::Vertex<> v2{ 2, 20.0, -20.0, 0.0 };
		conv::Vertex<> v3{ 3, 20.0, 20.0, 0.0 };
		conv::Vertex<> v4{ 4, -20.0, 20.0, 0.0 };
		conv::Vertex<> v5{ 5, -20.0, -20.0, 40.0 };
		conv::Vertex<> v6{ 6, 20.0, -20.0, 40.0 };
		conv::Vertex<> v7{ 7, 20.0, 20.0, 40.0 };
		conv::Vertex<> v8{ 8, -20.0, 20.0, 40.0 };
		conv::Face f1{ 1, v1.getID(), v3.getID(), v2.getID() };
		conv::Face f2{ 2, v1.getID(), v4.getID(), v3.getID() };
		conv::Face f3{ 3, v1.getID(), v2.getID(), v6.getID() };
		conv::Face f4{ 4, v1.getID(), v6.getID(), v5.getID() };
		conv::Face f5{ 5, v1.getID(), v8.getID(), v4.getID() };
		conv::Face f6{ 6, v1.getID(), v5.getID(), v8.getID() };
		conv::Face f7{ 7, v2.getID(), v3.getID(), v7.getID() };
		conv::Face f8{ 8, v2.getID(), v7.getID(), v6.getID() };
		conv::Face f9{ 9, v5.getID(), v6.getID(), v7.getID() };
		conv::Face f10{ 10, v5.getID(), v7.getID(), v8.getID() };
		conv::Face f11{ 11, v4.getID(), v7.getID(), v3.getID() };
		conv::Face f12{ 12, v4.getID(), v8.getID(), v7.getID() };
	
	public:

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
			Assert::AreEqual(p, testMesh.getVertex(p.getID()), L"Failed to get the same Vertex!");
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
			Assert::AreNotEqual(testMesh.getFace(p.getID()), p, L"Failed to get the same Vertex!");
			Assert::IsFalse(testMesh.isFaceEmpty(), L"Function Implementation needs to be checked!");
			Assert::IsTrue(testMesh.isVertexEmpty(), L"we added 2 Faces but no vertices!");
			// ---- no side effect ----
			Assert::AreEqual(ID, testMesh.getID(), L"failed to initialize the ID correctly!");
			Assert::IsFalse(testMesh.isFaceEmpty(), L"at Construction no Face available!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::VERTEX_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::GEOMETRY_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::FRAGMENT_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{ "null" }, testMesh.listExtraFeatures(), L"default Construction should be empty!");
		}

		TEST_METHOD(MeshSurfaceArea)
		{
			testMesh.addVertex(v1);
			testMesh.addVertex(v2);
			testMesh.addVertex(v3);
			testMesh.addVertex(v4);
			testMesh.addVertex(v5);
			testMesh.addVertex(v6);
			testMesh.addVertex(v7);
			testMesh.addVertex(v8);

			testMesh.addFace(f1);
			testMesh.addFace(f2);
			testMesh.addFace(f3);
			testMesh.addFace(f4);
			testMesh.addFace(f5);
			testMesh.addFace(f6);
			testMesh.addFace(f7);
			testMesh.addFace(f8);
			testMesh.addFace(f9);
			testMesh.addFace(f10);
			testMesh.addFace(f11);
			testMesh.addFace(f11);
			testMesh.addFace(f12);

			Assert::AreEqual(9600.0, testMesh.area(), L"a 40 units per side cube should have a surface area equal 64000units!");
			// ---- no side effect ----
			Assert::AreEqual((uint64_t)1830, testMesh.getID(), L"failed to initialize the ID correctly!");
			Assert::IsFalse(testMesh.isFaceEmpty(), L"a Cube has 12 Triangle Faces!");
			Assert::IsFalse(testMesh.isVertexEmpty(), L"a Cube has 8 vertices!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::VERTEX_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::GEOMETRY_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::FRAGMENT_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{ "null" }, testMesh.listExtraFeatures(), L"default Construction should be empty!");
		}

		TEST_METHOD(MeshVolume)
		{
			testMesh.addVertex(v1);
			testMesh.addVertex(v2);
			testMesh.addVertex(v3);
			testMesh.addVertex(v4);
			testMesh.addVertex(v5);
			testMesh.addVertex(v6);
			testMesh.addVertex(v7);
			testMesh.addVertex(v8);

			testMesh.addFace(f1);
			testMesh.addFace(f2);
			testMesh.addFace(f3);
			testMesh.addFace(f4);
			testMesh.addFace(f5);
			testMesh.addFace(f6);
			testMesh.addFace(f7);
			testMesh.addFace(f8);
			testMesh.addFace(f9);
			testMesh.addFace(f10);
			testMesh.addFace(f11);
			testMesh.addFace(f11);
			testMesh.addFace(f12);

			Assert::AreEqual(64000.0, testMesh.volume(), L"a Unit cube should have a volume equal 1!");
			// ---- no side effect ----
			Assert::AreEqual((uint64_t)1830, testMesh.getID(), L"failed to initialize the ID correctly!");
			Assert::IsFalse(testMesh.isFaceEmpty(), L"a Cube has 12 Triangle Faces!");
			Assert::IsFalse(testMesh.isVertexEmpty(), L"a Cube has 8 vertices!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::VERTEX_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::GEOMETRY_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::FRAGMENT_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{ "null" }, testMesh.listExtraFeatures(), L"default Construction should be empty!");
		}

		TEST_METHOD(MeshIsPointInsideTest)
		{
			testMesh.addVertex(v1);
			testMesh.addVertex(v2);
			testMesh.addVertex(v3);
			testMesh.addVertex(v4);
			testMesh.addVertex(v5);
			testMesh.addVertex(v6);
			testMesh.addVertex(v7);
			testMesh.addVertex(v8);

			testMesh.addFace(f1);
			testMesh.addFace(f2);
			testMesh.addFace(f3);
			testMesh.addFace(f4);
			testMesh.addFace(f5);
			testMesh.addFace(f6);
			testMesh.addFace(f7);
			testMesh.addFace(f8);
			testMesh.addFace(f9);
			testMesh.addFace(f10);
			testMesh.addFace(f11);
			testMesh.addFace(f11);
			testMesh.addFace(f12);

			conv::pos3D<> center{ 0, 0, 20 };
			conv::pos3D<> point{ -30, 20, 0 };
			Assert::IsTrue(testMesh.isInside(center), L"center of Cube should be inside it!");
			Assert::IsFalse(testMesh.isInside(point), L"point is outside this Cube!");
			// ---- no side effect ----
			Assert::AreEqual((uint64_t)1830, testMesh.getID(), L"failed to initialize the ID correctly!");
			Assert::IsFalse(testMesh.isFaceEmpty(), L"a Cube has 12 Triangle Faces!");
			Assert::IsFalse(testMesh.isVertexEmpty(), L"a Cube has 8 vertices!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::VERTEX_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::GEOMETRY_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{}, testMesh.getShader(conv::shader_e::FRAGMENT_SHD), L"default Construction should be empty!");
			Assert::AreEqual(std::string{ "null" }, testMesh.listExtraFeatures(), L"default Construction should be empty!");
		}
		// FIX ME: TEST_METHOD(MeshTransform)
	};
}
