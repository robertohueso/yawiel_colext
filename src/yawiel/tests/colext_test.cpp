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

BOOST_AUTO_TEST_CASE(BasicTest)
{
  const std::string text = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";

  //Corpus<> corpus;
  //std::cout << "Loading file..." << std::endl;
  //corpus.loadFile("big.txt");
  //std::cout << "File loaded..." << std::endl;
  //corpus.LoadText(text);
  Corpus<> corpus;
  corpus.LoadFile(std::string("../big.txt"));
  Colext<G1<PMI<NGramCounter<>>>> colext;
  colext.LoadCorpus(corpus);
  vector<pair<vector<size_t>, double>> scores;
  colext.GetSortedScores(scores, 3, true);
  
  for (auto kv : scores)
  {
    //std::cout<< colext.Corpus().VectorToString(kv.first, ' ') << " :: " << kv.second << std::endl;
  }
  
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

/*
void PrintExtremes(const vector<pair<vector<size_t>, double>>& scores,
                   const Corpus<std::string>& corpus,
                   size_t n = 150)
{
  std::cout << std::endl << "WORST:" << std::endl;
  for (size_t i = 0; i < n; ++i)
  {
    for (auto i : scores[i].first)
      std::cout << (corpus.GetVocabulary()).at(i) << " ";
    std::cout <<" :: " << scores[i].second << std::endl;
  }

  std::cout << std::endl << "BEST:" << std::endl;
  for (size_t i = scores.size() - n; i < scores.size(); ++i)
  {
    for (auto i : scores[i].first)
      std::cout << (corpus.GetVocabulary()).at(i) << " ";
    std::cout <<" :: " << scores[i].second << std::endl;
  }
}

template<typename StringType,
         typename EP,
         typename AM>
void EvalColext(Corpus<StringType>& corpus,
                NGramCounter<StringType>& counter,
                size_t n,
                StringType filePath)
{
  vector<pair<vector<size_t>, double>> scores;
  Colext<StringType, EP, AM> colext(corpus,counter);
  colext.GetSortedScores(scores, n);
  //PrintExtremes(scores, corpus);
  //colext.ScoresToCSV(scores, filePath);
  scores.clear();
}

BOOST_AUTO_TEST_CASE(SortedScoresTest)
{
  Corpus<std::string> corpus;
  std::cout << "Loading file..." << std::endl;
  corpus.LoadFile("big.txt");
  std::cout << "File loaded..." << std::endl;
  NGramCounter<std::string> counter(corpus);
  vector<pair<vector<size_t>, double>> scores;

  // Some tests.
  std::cout << "--- G0 - LogLike - n3 ------" << std::endl;
  EvalColext<string, G0<string, LogLikelihood<string>>, LogLikelihood<string>>
    (corpus, counter, 3, "scores_G0_LogLike_n3.csv");

  std::cout << "--- G0 - LogLike - n2 ------" << std::endl;
  EvalColext<string, G0<string, LogLikelihood<string>>, LogLikelihood<string>>
    (corpus, counter, 2, "scores_G0_LogLike_n2.csv");

  std::cout << "--- G0 - ChiSquared - n3 ---" << std::endl;
  EvalColext<string, G0<string, ChiSquared<string>>, ChiSquared<string>>
    (corpus, counter, 3, "scores_G0_ChiSquared_n3.csv");

  std::cout << "--- G0 - ChiSquared - n2 ---" << std::endl;
  EvalColext<string, G0<string, ChiSquared<string>>, ChiSquared<string>>
    (corpus, counter, 2, "scores_G0_ChiSquared_n2.csv");

  std::cout << "--- G1 - PMI - n4 ----------" << std::endl;
  EvalColext<string, G1<string, PMI<string>>, PMI<string>>
    (corpus, counter, 4, "scores_G1_PMI_n4.csv");

  std::cout << "--- G1 - PMI - n3 ----------" << std::endl;
  EvalColext<string, G1<string, PMI<string>>, PMI<string>>
    (corpus, counter, 3, "scores_G1_PMI_n3.csv");

  std::cout << "--- G1 - PMI - n2 ----------" << std::endl;
  EvalColext<string, G1<string, PMI<string>>, PMI<string>>
    (corpus, counter, 2, "scores_G1_PMI_n2.csv");

  std::cout << "--- G1 - Dice - n4 ----------" << std::endl;
  EvalColext<string, G1<string, Dice<string>>, Dice<string>>
    (corpus, counter, 4, "scores_G1_Dice_n4.csv");

  std::cout << "--- G1 - Dice - n3 ----------" << std::endl;
  EvalColext<string, G1<string, Dice<string>>, Dice<string>>
    (corpus, counter, 3, "scores_G1_Dice_n3.csv");

  std::cout << "--- G1 - Dice - n2 ----------" << std::endl;
  EvalColext<string, G1<string, Dice<string>>, Dice<string>>
    (corpus, counter, 2, "scores_G1_Dice_n2.csv");

  std::cout << "--- G1 - LogLikelihood - n4 ----------" << std::endl;
  EvalColext<string, G1<string, LogLikelihood<string>>, LogLikelihood<string>>
    (corpus, counter, 4, "scores_G1_LogLikelihood_n4.csv");

  std::cout << "--- G1 - LogLikelihood - n3 ----------" << std::endl;
  EvalColext<string, G1<string, LogLikelihood<string>>, LogLikelihood<string>>
    (corpus, counter, 3, "scores_G1_LogLikelihood_n3.csv");

  std::cout << "--- G1 - LogLikelihood - n2 ----------" << std::endl;
  EvalColext<string, G1<string, LogLikelihood<string>>, LogLikelihood<string>>
    (corpus, counter, 2, "scores_G1_LogLikelihood_n2.csv");

  std::cout << "--- G1 - ChiSquared - n4 ----------" << std::endl;
  EvalColext<string, G1<string, ChiSquared<string>>, ChiSquared<string>>
    (corpus, counter, 4, "scores_G1_ChiSquared_n4.csv");

  std::cout << "--- G1 - ChiSquared - n3 ----------" << std::endl;
  EvalColext<string, G1<string, ChiSquared<string>>, ChiSquared<string>>
    (corpus, counter, 3, "scores_G1_ChiSquared_n3.csv");

  std::cout << "--- G1 - ChiSquared - n2 ----------" << std::endl;
  EvalColext<string, G1<string, ChiSquared<string>>, ChiSquared<string>>
    (corpus, counter, 2, "scores_G1_ChiSquared_n2.csv");

  std::cout << "--- G2 - PMI - n4 ----------" << std::endl;
  EvalColext<string, G2<string, PMI<string>>, PMI<string>>
    (corpus, counter, 4, "scores_G2_PMI_n4.csv");

  std::cout << "--- G2 - PMI - n3 ----------" << std::endl;
  EvalColext<string, G2<string, PMI<string>>, PMI<string>>
    (corpus, counter, 3, "scores_G2_PMI_n3.csv");

  std::cout << "--- G2 - PMI - n2 ----------" << std::endl;
  EvalColext<string, G2<string, PMI<string>>, PMI<string>>
    (corpus, counter, 2, "scores_G2_PMI_n2.csv");

  std::cout << "--- G2 - Dice - n4 ----------" << std::endl;
  EvalColext<string, G2<string, Dice<string>>, Dice<string>>
    (corpus, counter, 4, "scores_G2_Dice_n4.csv");

  std::cout << "--- G2 - Dice - n3 ----------" << std::endl;
  EvalColext<string, G2<string, Dice<string>>, Dice<string>>
    (corpus, counter, 3, "scores_G2_Dice_n3.csv");

  std::cout << "--- G2 - Dice - n2 ----------" << std::endl;
  EvalColext<string, G2<string, Dice<string>>, Dice<string>>
    (corpus, counter, 2, "scores_G2_Dice_n2.csv");

  std::cout << "--- G2 - LogLikelihood - n4 ----------" << std::endl;
  EvalColext<string, G2<string, LogLikelihood<string>>, LogLikelihood<string>>
    (corpus, counter, 4, "scores_G2_LogLikelihood_n4.csv");

  std::cout << "--- G2 - LogLikelihood - n3 ----------" << std::endl;
  EvalColext<string, G2<string, LogLikelihood<string>>, LogLikelihood<string>>
    (corpus, counter, 3, "scores_G2_LogLikelihood_n3.csv");

  std::cout << "--- G2 - LogLikelihood - n2 ----------" << std::endl;
  EvalColext<string, G2<string, LogLikelihood<string>>, LogLikelihood<string>>
    (corpus, counter, 2, "scores_G2_LogLikelihood_n2.csv");

  std::cout << "--- G2 - ChiSquared - n4 ----------" << std::endl;
  EvalColext<string, G2<string, ChiSquared<string>>, ChiSquared<string>>
    (corpus, counter, 4, "scores_G2_ChiSquared_n4.csv");

  std::cout << "--- G2 - ChiSquared - n3 ----------" << std::endl;
  EvalColext<string, G2<string, ChiSquared<string>>, ChiSquared<string>>
    (corpus, counter, 3, "scores_G2_ChiSquared_n3.csv");

  std::cout << "--- G2 - ChiSquared - n2 ----------" << std::endl;
  EvalColext<string, G2<string, ChiSquared<string>>, ChiSquared<string>>
    (corpus, counter, 2, "scores_G2_ChiSquared_n2.csv");
  /*
  std::cout << "--- G2 - Dice - n3 ---------" << std::endl;
  EvalColext<string, G2<string, Dice<string>>, Dice<string>>
    (corpus, counter, 3);

  std::cout << "--- G4 - Dice - n2 ---------" << std::endl;
  EvalColext<string, G4<string, Dice<string>>, Dice<string>>
    (corpus, counter, 2);

  std::cout << "--- G5 - PMI - n2 ----------" << std::endl;
  EvalColext<string, G4<string, PMI<string>>, PMI<string>>
    (corpus, counter, 2);
  */
  //}

BOOST_AUTO_TEST_SUITE_END();
