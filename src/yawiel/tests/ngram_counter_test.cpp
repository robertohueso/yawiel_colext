#include <yawiel/core/util/ngram_counter.hpp>
#include <boost/test/unit_test.hpp>
#include <unordered_map>
#include <vector>

using namespace yawiel;
using namespace yawiel::text;
using namespace yawiel::util;
using namespace std;

BOOST_AUTO_TEST_SUITE(NGramCounterTest);

BOOST_AUTO_TEST_CASE(BasicCounter)
{
  const std::string text = "this is a test a test";
  Corpus<std::string> corpus;
  corpus.LoadString(text);
  NGramCounter<std::string> counter(corpus);
  counter.ComputeCounts(2);
  auto counts = counter.GetCounts(2);

  vector<size_t> bi1 {0, 1};
  vector<size_t> bi2 {1, 2};
  vector<size_t> bi3 {2, 3};
  vector<size_t> bi4 {3, 2};

  BOOST_REQUIRE_EQUAL(counts[bi1], 1);
  BOOST_REQUIRE_EQUAL(counts[bi2], 1);
  BOOST_REQUIRE_EQUAL(counts[bi3], 2);
  BOOST_REQUIRE_EQUAL(counts[bi4], 1);
}

BOOST_AUTO_TEST_CASE(SerializedCounter)
{
  const std::string text = "this is a test a test";
  const std::string dummyText = "nothing";
  const std::string fileName = "test_serialized_counter.bin";
  Corpus<std::string> corpusText;
  corpusText.LoadString(text);
  Corpus<std::string> corpusDummy;
  corpusDummy.LoadString(text);
  NGramCounter<std::string> counter1(corpusText);
  NGramCounter<std::string> counter2(corpusDummy);
  counter1.ComputeCounts(2);
  auto counts1 = counter1.GetCounts(2);

  // Save counter.
  std::ofstream outputFile(fileName);
  {
    boost::archive::binary_oarchive outputArchive(outputFile);
    outputArchive << BOOST_SERIALIZATION_NVP(counter1);
  }
  // Load counter.
  std::ifstream inputFile(fileName);
  assert(inputFile.good());
  {
    boost::archive::binary_iarchive inputArchive(inputFile);
    inputArchive >> BOOST_SERIALIZATION_NVP(counter2);
  }
  auto counts2 = counter2.GetCounts(2);

  // Check counts are the same.
  vector<size_t> bi1 {0, 1};
  vector<size_t> bi2 {1, 2};
  vector<size_t> bi3 {2, 3};
  vector<size_t> bi4 {3, 2};

  BOOST_REQUIRE_EQUAL(counts1[bi1], counts2[bi1]);
  BOOST_REQUIRE_EQUAL(counts1[bi2], counts2[bi2]);
  BOOST_REQUIRE_EQUAL(counts1[bi3], counts2[bi3]);
  BOOST_REQUIRE_EQUAL(counts1[bi4], counts2[bi4]);
}

BOOST_AUTO_TEST_SUITE_END();
