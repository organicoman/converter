#include "pch.h"
#include "CppUnitTest.h"
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<> std::wstring ToString(const std::unordered_set<std::string>& set)
	{
		std::wostringstream str(L"<");
		int i = 0;
		for (auto el : set)
		{
			str << el.c_str() << (i < 2 ? L", " : L"");
			++i;
		}
		str << L">";
		return str.str();
	}
}
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testSerializer
{
	TEST_CLASS(testSerializer)
	{
		std::unordered_set<std::string> tags = { "#", "v", "f", "vn", "vc", "vtx" };
		std::unordered_set<std::string> patterns = { "%tag% %?%","%tag% %id% %x% %y% %z%","%tag% %id% %vId1% %vId2% %vId3%",
			"%tag% %id% %i% %j% %k%","%tag% %id% %r% %g% %b% %a%","%tag% %id% %u% %v%" };
	public:
		
		TEST_METHOD(noDefaultCtor)
		{
			Assert::IsFalse(std::is_default_constructible<conv::Serializer>::value,
				L"Serializer class should not have a default constructor!");
		}

		TEST_METHOD(CtorWithFilenameParam)
		{
			//make sure to adjust this absolut path
			const std::string filename = "C:/Users/Nadir/source/Shapr3d/Tests/testSerializer/SampleFile.json";
			conv::Serializer s{ filename };
			// after construction the tags and patterns should be populated
			Assert::AreEqual(tags, conv::dumpAllTags(s), L"make sure the sample file used is the same, then check the result");
			Assert::AreEqual(patterns, conv::dumpAllPatterns(s), L"make sure the sample file used is the same, then check the result");
		}

		TEST_METHOD(SeserilizerAddStamper)
		{
			//make sure to adjust this absolut path
			const std::string filename = "C:/Users/Nadir/source/Shapr3d/Tests/testSerializer/SampleFile.json";
			conv::Serializer s{ filename };
			conv::Mesh3D<> testMesh;
			std::ostringstream sout;
			auto vfunc =
				[&](const conv::Mesh3D<>& mesh, const std::string& patern, std::ostream& out)
			{out << "called"; };
			s.addStamper("v", vfunc);
			s.streamWriter(testMesh, sout);
			Assert::AreEqual(std::string{"called"}, sout.str() , L"priting 'called' for vertex tag 'v'!");
			// after construction the tags and patterns should be populated
			Assert::AreEqual(tags, conv::dumpAllTags(s), L"make sure the sample file used is the same, then check the result");
			Assert::AreEqual(patterns, conv::dumpAllPatterns(s), L"make sure the sample file used is the same, then check the result");
		}
	};
}
