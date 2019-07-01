#ifndef YAWIEL_CORE_CONTAINERS_VOCABULARY_HPP
#define YAWIEL_CORE_CONTAINERS_VOCABULARY_HPP

#include <yawiel/prereqs.hpp>
#include <unordered_map>
#include <vector>

namespace yawiel{
namespace containers{

template<typename StringType>
class Vocabulary
{
 private:
  //! Map strings to IDs.
  std::unordered_map<StringType, size_t>* wordsIndex;

  //! Strings sorted by ID.
  std::vector<StringType>* words;

  //! Size of the vocabulary.
  size_t size;

 public:
  //! Default constructor.
  Vocabulary();

  /**
   * Construct vocabulary object as a copy of other vocabulary.
   * This might be computationally intensive.
   *
   * @param other Vocabulary to copy.
   */
  Vocabulary(const Vocabulary& other);

  /**
   * Construct vocabulary object taking ownership of the given object.
   *
   * @param other Vocabulary to copy.
   */
  Vocabulary(Vocabulary&& other);

  //! Destructor.
  ~Vocabulary();

  /**
   * Add a new token to the vocabulary.
   *
   * @param token Token to add.
   */
  void AddToken(StringType token);

  //! Get the ID of a word.
  inline size_t at(const StringType& word) const
      { return (wordsIndex->find(word))->second; }

  //! Get the word of an ID.
  inline const StringType& at(const size_t index) const
      { return words->at(index); }

  //! Get the size of the vocabulary.
  inline size_t Size() const { return size; }

  //! Serialize the vocabulary.
  template<typename Archive>
  void serialize(Archive& ar, const unsigned int /* version */);
};

}
}

#include "vocabulary_impl.cpp"

#endif
