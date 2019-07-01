#include <yawiel/core/util/ngram_counter.hpp>
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
#include <yawiel/colext/eam/ep/g6.hpp>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace yawiel;
using namespace yawiel::text;
using namespace yawiel::util;
using namespace yawiel::colext;

BOOST_AUTO_TEST_SUITE(EAMTest);

typedef NGramCounter<std::string> Counter;
const std::string text1 = "this is a test a test";

BOOST_AUTO_TEST_CASE(PMITest)
{
  Corpus<string> corpus;
  corpus.LoadString(text1);
  Counter counter(corpus);
  PMI<Counter> pmi(counter);
  pmi.Precompute(3);

  vector<size_t> gram1 {0, 1};
  vector<size_t> gram2 {2};

  const double result = pmi.Evaluate(gram1, gram2);
  // Compare against a manually calculated PMI value.
  BOOST_REQUIRE_CLOSE(result, 1.90833401257, 0.1);
}

BOOST_AUTO_TEST_CASE(DiceTest)
{
  Corpus<string> corpus;
  corpus.LoadString(text1);
  Counter counter(corpus);
  Dice<Counter> dice(counter);
  dice.Precompute(3);
  const auto& vocab = corpus.GetVocabulary();

  vector<size_t> gram1 {vocab.at("this"), vocab.at("is")};
  vector<size_t> gram2 {vocab.at("a")};

  const double result = dice.Evaluate(gram1, gram2);
  // Compare against a manually calculated Dice value.
  BOOST_REQUIRE_CLOSE(result, 0.6666666666666, 0.1);
}

BOOST_AUTO_TEST_CASE(G1Test)
{
  Corpus<string> corpus;
  corpus.LoadString(text1);
  Counter counter(corpus);
  vector<size_t> aTestA {2, 3, 2};
  PMI<Counter> pmi(counter);
  G1<PMI<Counter>> g1(counter, pmi);
  g1.Precompute(3);

  const double G1Result = g1.Evaluate(aTestA);

  // Manual result.
  vector<size_t> aTest {2, 3};
  vector<size_t> a {2};
  vector<size_t> testA {3, 2};
  const double PMIResult1 = pmi.Evaluate(aTest, a);
  const double PMIResult2 = pmi.Evaluate(a, testA);
  const double ManualResult = (PMIResult1 + PMIResult2) / 2.0;
  BOOST_REQUIRE_CLOSE(G1Result, ManualResult, 1e-8);
}

BOOST_AUTO_TEST_CASE(G0PMITest)
{
  Corpus<string> corpus;
  corpus.LoadString(text1);
  Counter counter(corpus);
  vector<size_t> aTestA {2, 3, 2};
  PMI<Counter> pmi(counter);
  G0<PMI<Counter>> g0(counter, pmi);
  g0.Precompute(3);

  const double g0Result = g0.Evaluate(aTestA);

  // Manual result.
  const double numerator = 1.0 / 4.0;
  const double denominator = std::pow((2.0 / 6.0), 3.0);
  const double ManualResult = std::log2(numerator / denominator);
  BOOST_REQUIRE_CLOSE(g0Result, ManualResult, 1e-8);
}

BOOST_AUTO_TEST_CASE(G2Test)
{
  Corpus<string> corpus;
  corpus.LoadString(text1);
  Counter counter(corpus);
  const auto& vocab = corpus.GetVocabulary();
  Dice<Counter> dice(counter);
  G2<Dice<Counter>> g2(counter, dice);
  g2.Precompute(4);

  vector<size_t> gram {vocab.at("is"),
                       vocab.at("a"),
                       vocab.at("test"),
                       vocab.at("a")};

  const double g2Result = g2.Evaluate(gram);

  // Manual result.
  vector<size_t> isA {gram[0], gram[1]};
  vector<size_t> testA {gram[2], gram[3]};
  vector<size_t> isATest {gram[0],gram[1], gram[2]};
  vector<size_t> a {gram[3]};
  const double diceResult1 = dice.Evaluate(isA, testA);
  const double diceResult2 = dice.Evaluate(isATest, a);
  const double manualResult = (diceResult1 + diceResult2) / 2.0;
  BOOST_REQUIRE_CLOSE(g2Result, manualResult, 1e-8);
}

BOOST_AUTO_TEST_CASE(G3Test)
{
  Corpus<string> corpus;
  corpus.LoadString(text1);
  Counter counter(corpus);
  const auto& vocab = corpus.GetVocabulary();
  PMI<Counter> pmi(counter);
  G3<PMI<Counter>> g3(counter, pmi);
  g3.Precompute(5);

  vector<size_t> gram {vocab.at("is"),
                       vocab.at("a"),
                       vocab.at("test"),
                       vocab.at("a"),
                       vocab.at("test")};

  const double g3Result = g3.Evaluate(gram);

  // Manual result.
  vector<size_t> is {gram[0]};
  vector<size_t> a {gram[1]};
  vector<size_t> test {gram[2]};

  const double pmiResult1 = pmi.Evaluate(is, a);
  const double pmiResult2 = pmi.Evaluate(a, test);
  const double pmiResult3 = pmi.Evaluate(test, a);
  const double pmiResult4 = pmi.Evaluate(a, test);
  const double manualResult =
      (pmiResult1 + pmiResult2 + pmiResult3 + pmiResult4) / (5 - 1);

  BOOST_REQUIRE_CLOSE(g3Result, manualResult, 1e-8);
}

BOOST_AUTO_TEST_CASE(G4Test)
{
  Corpus<string> corpus;
  corpus.LoadString(text1);
  Counter counter(corpus);
  const auto& vocab = corpus.GetVocabulary();
  Dice<Counter> dice(counter);
  G4<Dice<Counter>> g4(counter, dice);
  g4.Precompute(8);

  vector<size_t> gram {vocab.at("is"),
                       vocab.at("a"),
                       vocab.at("test"),
                       vocab.at("a"),
                       vocab.at("test")};

  const double g4Result = g4.Evaluate(gram);

  // Manual result.
  vector<size_t> isATestA {gram[0], gram[1], gram[2], gram[3]};
  vector<size_t> aTestATest {gram[1], gram[2], gram[3], gram[4]};

  const double manualResult = dice.Evaluate(isATestA, aTestATest);

  BOOST_REQUIRE_CLOSE(g4Result, manualResult, 1e-8);
}

BOOST_AUTO_TEST_CASE(G5Test)
{
  Corpus<string> corpus;
  corpus.LoadString(text1);
  Counter counter(corpus);
  const auto& vocab = corpus.GetVocabulary();
  Dice<Counter> dice(counter);
  G5<Dice<Counter>> g5(counter, dice);
  g5.Precompute(5);

  vector<size_t> gram {vocab.at("is"),
                       vocab.at("a"),
                       vocab.at("test"),
                       vocab.at("a")};

  const double g5Result = g5.Evaluate(gram);

  // Manual result.
  vector<size_t> is {gram[0]};
  vector<size_t> aTestA {gram[1], gram[2], gram[3]};
  vector<size_t> isA {gram[0], gram[1]};
  vector<size_t> testA {gram[2], gram[3]};
  vector<size_t> isATest {gram[0], gram[1], gram[2]};
  vector<size_t> a {gram[3]};

  double manualResult = dice.Evaluate(is, aTestA) +
                        dice.Evaluate(isA, testA) +
                        dice.Evaluate(isATest, a);
  manualResult /= 3;

  BOOST_REQUIRE_CLOSE(g5Result, manualResult, 1e-8);
}

BOOST_AUTO_TEST_CASE(G6DiceTest)
{
  Corpus<string> corpus;
  corpus.LoadString(text1);
  Counter counter(corpus);
  const auto& vocab = corpus.GetVocabulary();
  Dice<Counter> dice(counter);
  G6<Dice<Counter>> g6(counter, dice);
  g6.Precompute(5);

  vector<size_t> gram {vocab.at("is"),
                       vocab.at("a"),
                       vocab.at("test"),
                       vocab.at("a")};

  const double g6Result = g6.Evaluate(gram);

  // Manual result.
  vector<size_t> isA {gram[0], gram[1]};
  vector<size_t> testA {gram[2], gram[3]};

  const size_t n = gram.size();
  const size_t countGram = counter.GetCounts(gram);
  const size_t countBi = counter.GetCounts(isA) + counter.GetCounts(testA);
  const double manualResult = (n * countGram) / countBi;

  BOOST_REQUIRE_CLOSE(g6Result, manualResult, 1e-8);
}

BOOST_AUTO_TEST_SUITE_END();
