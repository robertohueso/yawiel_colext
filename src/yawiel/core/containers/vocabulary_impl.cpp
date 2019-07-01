#ifndef YAWIEL_CORE_CONTAINERS_VOCABULARY_IMPL_CPP
#define YAWIEL_CORE_CONTAINERS_VOCABULARY_IMPL_CPP

#include "vocabulary.hpp"

namespace yawiel{
namespace containers{

template<typename StringType>
Vocabulary<StringType>::Vocabulary() :
  wordsIndex(new std::unordered_map<StringType, size_t>()),
  words(new std::vector<StringType>()),
  size(0)
{
  // Nothing to do.
}

template<typename StringType>
Vocabulary<StringType>::Vocabulary(const Vocabulary& other) :
  wordsIndex(new std::unordered_map<StringType, size_t>(*other.wordsIndex)),
  words(new std::vector<StringType>(*other.words)),
  size(other.size)
{
  // Nothing to do.
}

template<typename StringType>
Vocabulary<StringType>::Vocabulary(Vocabulary&& other) :
  wordsIndex(other.wordsIndex),
  words(other.words),
  size(other.size)
{
  other.wordsIndex = nullptr;
  other.words = nullptr;
  other.size = 0;
}

template<typename StringType>
void Vocabulary<StringType>::AddToken(StringType token)
{
  const auto it = wordsIndex->find(token);
  if (it == wordsIndex->end())
  {
    wordsIndex->emplace(token, size);
    words->push_back(std::move(token));
    ++size;
  }
}

template<typename StringType>
Vocabulary<StringType>::~Vocabulary()
{
  delete wordsIndex;
  delete words;
}

template<typename StringType>
template<typename Archive>
void Vocabulary<StringType>::
serialize(Archive& ar, const unsigned int /* version */)
{
  ar & BOOST_SERIALIZATION_NVP(size);

  // Clean memory if necessary.
  if (Archive::is_loading::value)
  {
    delete wordsIndex;
    delete words;
  }

  ar & BOOST_SERIALIZATION_NVP(wordsIndex);
  ar & BOOST_SERIALIZATION_NVP(words);
}

}
}

#endif
