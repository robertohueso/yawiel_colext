#ifndef YAWIEL_CORE_TEXT_CORPUS_IMPL_CPP
#define YAWIEL_CORE_TEXT_CORPUS_IMPL_CPP

#include "corpus.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>

using namespace yawiel;

namespace yawiel{
namespace text{

template<typename StringType>
Corpus<StringType>::Corpus():
  vocabulary(nullptr),
  corpusTokens(nullptr),
  ownVocabulary(false),
  ownCorpusTokens(false),
  loaded(false)
{
  SetSeparators(GetDefaultSeparators());
}

template<typename StringType>
Corpus<StringType>::Corpus(const Corpus<StringType>& other):
  vocabulary(new containers::Vocabulary<StringType>(*other.vocabulary)),
  corpusTokens(new std::vector<size_t>(*other.corpusTokens)),
  separators(StringType(other.separators)),
  ownVocabulary(true),
  ownCorpusTokens(true),
  loaded(other.loaded)
{
  // Nothing to do here.
}

template<typename StringType>
Corpus<StringType>::Corpus(Corpus<StringType>&& other):
  vocabulary(other.vocabulary),
  corpusTokens(other.corpusTokens),
  separators(std::move(other.separators)),
  ownVocabulary(other.ownVocabulary),
  ownCorpusTokens(other.ownCorpusTokens),
  loaded(other.loaded)
{
  other.vocabulary = nullptr;
  other.corpusTokens = nullptr;
  other.ownVocabulary = false;
  other.ownCorpusTokens = false;
}

template<typename StringType>
void Corpus<StringType>::LoadFile(const std::string& filePath)
{
  if (ownVocabulary)
    delete vocabulary;
  if (ownCorpusTokens)
    delete corpusTokens;

  vocabulary = new containers::Vocabulary<StringType>;
  corpusTokens = new std::vector<size_t>;

  ownVocabulary = true;
  ownCorpusTokens = true;

  std::basic_ifstream<typename StringType::value_type> file(filePath);
  StringType tempString;
  std::vector<StringType> tokens;
  while (std::getline<typename StringType::value_type>(file, tempString))
  {
    TokenizeLine(tempString, tokens);
    // Add new tokens to vocabulary.
    for (auto token = tokens.begin(); token != tokens.end(); ++token)
      vocabulary->AddToken(*token);
    // Add new tokens to the corpus.
    for (auto token = tokens.cbegin(); token != tokens.cend(); ++token)
      corpusTokens->push_back(vocabulary->at(*token));
  }

  loaded = true;
}

template<typename StringType>
void Corpus<StringType>::LoadString(StringType text)
{
  if (ownVocabulary)
    delete vocabulary;
  if (ownCorpusTokens)
    delete corpusTokens;

  vocabulary = new containers::Vocabulary<StringType>;
  corpusTokens = new std::vector<size_t>;

  ownVocabulary = true;
  ownCorpusTokens = true;

  std::vector<StringType> tokens;

  TokenizeLine(text, tokens);
  // Add new tokens to vocabulary.
  for (auto token = tokens.begin(); token != tokens.end(); ++token)
    vocabulary->AddToken(*token);
  // Add new tokens to the corpus.
  for (auto token = tokens.cbegin(); token != tokens.cend(); ++token)
    corpusTokens->push_back(vocabulary->at(*token));

  loaded = true;
}

template<typename StringType>
Corpus<StringType>::~Corpus()
{
  if (ownVocabulary)
    delete vocabulary;
  if (ownCorpusTokens)
    delete corpusTokens;
}

template<typename StringType>
void Corpus<StringType>::
SetSeparators(StringType newSeparators)
{
  separators = std::move(newSeparators);
}

template<typename StringType>
StringType Corpus<StringType>::
VectorToString(const std::vector<size_t>& ngram,
               typename StringType::value_type separator) const
{
  StringType string;
  for (size_t i = 0; i < ngram.size(); ++i)
    string += vocabulary->at(ngram[i]) + separator;
  string.pop_back();
  return string;
}

template<typename StringType>
size_t Corpus<StringType>::TotalNGrams(const size_t n) const
{
  return corpusTokens->size() - n + 1;
}

template<typename StringType>
void Corpus<StringType>::TokenizeLine(StringType& text,
                                      std::vector<StringType>& tokensStrings)
{
  tokensStrings.clear();

  // Lowercase.
  boost::algorithm::to_lower(text);

  // Tokenization.
  typedef boost::tokenizer<
      boost::char_separator<typename StringType::value_type>,
      typename StringType::const_iterator,
      StringType> tokenizer;

  boost::char_separator<typename StringType::value_type>
      sep(separators.c_str());
  tokenizer tokens(text, sep);

  // Save tokens sequence.
  for (auto it = tokens.begin(); it != tokens.end(); ++it)
    tokensStrings.push_back(*it);
}

template<typename StringType>
template<typename Archive>
void Corpus<StringType>::serialize(Archive& ar,
                                   const unsigned int /* version */)
{
  ar & BOOST_SERIALIZATION_NVP(separators);
  ar & BOOST_SERIALIZATION_NVP(loaded);

  // Clean memory if necessary.
  if (Archive::is_loading::value)
  {
    if (ownVocabulary)
      delete vocabulary;
    if (ownCorpusTokens)
      delete corpusTokens;

    // After loading we own the objects.
    ownVocabulary = true;
    ownCorpusTokens = true;
  }

  // Serialize the rest of values.
  ar & BOOST_SERIALIZATION_NVP(vocabulary);
  ar & BOOST_SERIALIZATION_NVP(corpusTokens);
}

}
}

#include "corpus_impl.cpp"

#endif
