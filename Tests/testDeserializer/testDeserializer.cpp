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

namespace testDeserializer
{
	TEST_CLASS(testDeserializer)
	{
		std::unordered_set<std::string> tags = { "#", "v", "f", "vn", "vc", "vtx" };
		std::unordered_set<std::string> patterns = { "%tag% %?% 0","%tag% %id% %x% %y% %z% 0","%tag% %id% %vId1% %vId2% %vId3% 0",
			"%tag% %id% %i% %j% %k% 0","%tag% %id% %r% %g% %b% %a% 0","%tag% %id% %u% %v% 0" };
	public:
		
		TEST_METHOD(noDefaultCtor)
		{
			Assert::IsFalse(std::is_default_constructible<conv::Deserializer>::value,
				L"Deserializer Class should not have default constructor!");
		}

		TEST_METHOD(CtorWithFilename)
		{
			//make sure to adjust this absolut path
			const std::string filename = "C:/Users/Nadir/source/Shapr3d/Tests/testDeserializer/SampleFile.json";
			conv::Deserializer d{filename};
			// after construction the tags and patterns should be populated
			Assert::AreEqual(tags, conv::dumpAllTags(d), L"make sure the sample file used is the same, then check the result");
			Assert::AreEqual(patterns, conv::dumpAllPatterns(d), L"make sure the sample file used is the same, then check the result");
		}

		TEST_METHOD(DeserilizerAddParser)
		{
			//make sure to adjust this absolut path
			const std::string filename = "C:/Users/Nadir/source/Shapr3d/Tests/testDeserializer/SampleFile.json";
			conv::Deserializer d{ filename };
			const std::string meshfile = "C:/Users/Nadir/source/Shapr3d/Tests/testDeserializer/meshSample.txt";
			conv::Mesh3D<> testMesh;
			auto vfunc = 
				[&](const std::string& pat, const std::string& in, conv::Mesh3D<>& des )->bool
			{return true; };
			d.addParser("v", vfunc);
			Assert::AreEqual(std::string{}, d.streamReader(meshfile, testMesh), L"a parser that does nothing except returning true");
			// after construction the tags and patterns should be populated
			Assert::AreEqual(tags, conv::dumpAllTags(d), L"make sure the sample file used is the same, then check the result");
			Assert::AreEqual(patterns, conv::dumpAllPatterns(d), L"make sure the sample file used is the same, then check the result");
		}
	};
}
