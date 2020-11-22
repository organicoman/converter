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
}
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace faceTest
{
	TEST_CLASS(faceTest)
	{
	public:
		
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
			Assert::AreEqual(f.getID(), ID, L"Face ID that you set is Not what you get!");
			Assert::AreEqual(fset.size(), (size_t)3, L"the given Vertex IDs are different but the stored IDs are not!");
			Assert::AreEqual(fset, std::unordered_set<uint64_t>{vID1, vID2, vID3}, L"the given vIDs are in a wrong order!");
		}

	};
}
