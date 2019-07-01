#include <yawiel/core/text/corpus.hpp>
#include <yawiel/core/text/gram_traverser.hpp>
#include <boost/test/unit_test.hpp>

using namespace yawiel;
using namespace yawiel::text;

BOOST_AUTO_TEST_SUITE(CorpusTest);

template<typename StringType>
class TestRule
{
 private:
  std::vector<StringType> ngrams;

  const std::vector<size_t>& tokens;

  const Corpus<StringType>& corpus;

 public:
  TestRule(const std::vector<size_t>& tokens, const Corpus<StringType>& corpus):
    tokens(tokens),
    corpus(corpus)
  {}

  void Base(const typename Corpus<StringType>::NGram ngram)
  {
    const containers::Vocabulary<StringType>& vocab = corpus.GetVocabulary();
    StringType str;
    for (size_t i = ngram.first; i != ngram.second; ++i)
      str += vocab.at(tokens.at(i)) + L"-";
    str.pop_back();
    ngrams.push_back(str);
  }

  std::vector<StringType> getNGrams(){ return ngrams; }
};

BOOST_AUTO_TEST_CASE(InstantiationTest)
{
  yawiel::text::Corpus<> corpus;
}

BOOST_AUTO_TEST_CASE(GramLeftToRightTest)
{
  const std::wstring text = L"hi my name is Yawiel";
  Corpus<std::wstring> corpus;
  corpus.LoadString(text);
  TestRule<std::wstring> rules(corpus.GetTokens(), corpus);
  typedef GramTraverser<TestRule<std::wstring>, std::wstring> TRV;
  TRV traverser(rules, corpus);
  traverser.Traverse(2);
  std::vector<std::wstring> ngrams = std::move(rules.getNGrams());
  BOOST_REQUIRE(ngrams[0] == L"hi-my");
  BOOST_REQUIRE(ngrams[1] == L"my-name");
  BOOST_REQUIRE(ngrams[2] == L"name-is");
  BOOST_REQUIRE(ngrams[3] == L"is-yawiel");
}

/*
BOOST_AUTO_TEST_CASE(FileLoadTest)
{
  yawiel::text::Corpus<> corpus;
  corpus.loadFile("SmallText.txt");
  for (auto token: corpus.GetTokens())
    break;
}
*/

BOOST_AUTO_TEST_SUITE_END();
