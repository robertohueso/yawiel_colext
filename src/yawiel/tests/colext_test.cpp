#include <yawiel/colext/colext.hpp>
#include <yawiel/colext/eam/am/pmi.hpp>
#include <yawiel/colext/eam/am/dice.hpp>
#include <yawiel/colext/eam/am/chi_squared.hpp>
#include <yawiel/colext/eam/am/log_likelihood.hpp>
#include <yawiel/colext/eam/ep/g0.hpp>
#include <yawiel/colext/eam/ep/g1.hpp>
#include <yawiel/colext/eam/ep/g2.hpp>
#include <yawiel/colext/eam/ep/g3.hpp>
#include <yawiel/colext/eam/ep/g4.hpp>
#include <yawiel/colext/eam/ep/g5.hpp>
#include <boost/test/unit_test.hpp>
#include <unordered_map>
#include <map>

using namespace std;
using namespace yawiel;
using namespace yawiel::text;
using namespace yawiel::util;
using namespace yawiel::colext;

BOOST_AUTO_TEST_SUITE(ColextTest);

const std::string text1 = "Lorem Ipsum is simply dummy text of the printing "
    "and typesetting industry. Lorem Ipsum has been the industry's "
    "standard dummy text ever since the 1500s, when an unknown printer "
    "took a galley of type and scrambled it to make a type specimen book.";

BOOST_AUTO_TEST_CASE(BasicTestWithNormalization)
{
  Corpus<> corpus;
  corpus.LoadString(text1);
  Colext<G1<PMI<NGramCounter<>>>> colext;
  colext.LoadCorpus(corpus);
  unordered_map<vector<size_t>, double> scores;
  BOOST_REQUIRE_NO_THROW(colext.GetScores(scores, 3, true));
}

BOOST_AUTO_TEST_CASE(BasicTestWithoutNormalization)
{
  Corpus<> corpus;
  corpus.LoadString(text1);
  Colext<G1<PMI<NGramCounter<>>>> colext;
  colext.LoadCorpus(corpus);
  unordered_map<vector<size_t>, double> scores;
  BOOST_REQUIRE_NO_THROW(colext.GetScores(scores, 4));
}

BOOST_AUTO_TEST_CASE(BasicTestSortedScores)
{
  Corpus<> corpus;
  corpus.LoadString(text1);
  Colext<G1<PMI<NGramCounter<>>>> colext;
  colext.LoadCorpus(corpus);
  vector<pair<vector<size_t>, double>> scores;
  BOOST_REQUIRE_NO_THROW(colext.GetSortedScores(scores, 3, true));
}

BOOST_AUTO_TEST_CASE(CountsSerialization)
{
  std::string text = "Hi hello hi hello this is a a a test test a";
  unordered_map<vector<size_t>, double> scores1, scores2;
  Corpus<> corpus;
  corpus.LoadString(text);
  NGramCounter<> counter(std::move(corpus));

  // Compute model 1.
  Colext<G1<PMI<NGramCounter<>>>> colext1(&counter);
  colext1.GetScores(scores1, 2);
  colext1.SaveModel(std::string("../colext_model_1.txt"));

  // Compute model 2.
  Colext<G1<PMI<NGramCounter<>>>> colext2;
  colext2.LoadModel(std::string("../colext_model_1.txt"));
  colext2.GetScores(scores2, 2);

  // Check results are equal.
  for (const auto kv : scores1)
  {
    const auto it = scores2.find(kv.first);
    const bool ngramsEqual = kv.first == it->first;
    BOOST_REQUIRE_EQUAL(ngramsEqual, true);
    BOOST_REQUIRE_CLOSE(kv.second, it->second, 1e-10);
  }
}

BOOST_AUTO_TEST_SUITE_END();
