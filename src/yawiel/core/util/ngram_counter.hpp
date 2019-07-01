#ifndef YAWIEL_CORE_UTIL_COUNT_NGRAMS_HPP
#define YAWIEL_CORE_UTIL_COUNT_NGRAMS_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/text/corpus.hpp>
#include <yawiel/core/util/vector_hash.cpp>

namespace yawiel{
namespace util{

template<typename StringType = std::string>
class NGramCounter
{
 public:
  //! Convenience typedef.
  typedef std::unordered_map<std::vector<size_t>, size_t> CountsType;

 private:
  //! Corpus to compute counts from.
  text::Corpus<StringType>* corpus;

  //! Counts from the corpus organized by size n.
  std::map<size_t, CountsType>* counts;

  //! If this is true, the counter will be responsible for the corpus
  //! destruction.
  bool ownCorpus;

  //! If this is true, the counter will be responsible for the counts
  //! destruction.
  bool ownCounts;

  /**
   * Count all ngrams of a given size in the corpus.
   *
   * @param n Size of the ngrams to count.
   * @param map Hash table where counts will be stored.
   */
  void Count(const size_t n,
             CountsType& map);

 public:
  //! Default constructor.
  NGramCounter();

  /**
   * Construct a counter for a given corpus. This will copy the corpus, if you
   * don't want the corpus to be copied then you can use the move constructor
   * with std::move().
   *
   * @pre Corpus needs to have been already tokenized and processed.
   * @param corpus Corpus to create a counter from.
   */
  NGramCounter(const text::Corpus<StringType>& corpus);

  /**
   * Construct a counter for a given corpus. This will take ownership of the
   * corpus object.
   *
   * @pre Corpus needs to have been already tokenized and processed.
   * @param corpus Corpus to create a counter from.
   */
  NGramCounter(text::Corpus<StringType>&& corpus);

  //! Copy constructor
  NGramCounter(const NGramCounter<StringType>& other);

  //! Move constructor
  NGramCounter(NGramCounter<StringType>&& other);

  //! Destructor
  ~NGramCounter();

  /**
   * Compute counts of all corpus n-grams for a given n.
   * Algorithmic complexity is O(n).
   *
   * This is NOT a thread_safe method.
   *
   * @param n Size of the n-grams to count.
   */
  void ComputeCounts(const size_t n);

  /**
   * Get all ngrams of a given size and their counts.
   * Algorithmic complexity is O(1).
   *
   * This is a thread safe method.
   *
   * @pre Counts for this size need to have been already computed.
   * @param n Size of the n-grams to get the count from.
   */
  inline const CountsType& GetCounts(const size_t n) const
      { return counts->at(n); }

  /**
   * Get the count of a given ngram. If the ngram does not show up in the corpus
   * then the count will be 0.
   * Algorithmic complexity is O(1).
   *
   * This is a thread safe method.
   *
   * @pre Counts for the size of the ngram need to have been already computed.
   * @param n Size of the n-grams to get the count from.
   */
  size_t GetCounts(const std::vector<size_t>& ngram) const;

  //! Get total amount of ngrams of size n in the corpus.
  inline size_t GetNumberOfNGrams(const size_t n) const
      { return corpus->TotalNGrams(n); }

  //! Get corpus.
  const text::Corpus<StringType>& Corpus() const;

  //! Serialize the counts to some archive.
  template<typename Archive>
  void serialize(Archive& ar, const unsigned int /* version */);
};

template<typename CorpusType>
class NGramCounterRule
{
 private:
  std::unordered_map<std::vector<size_t>, size_t>& counts;

  const CorpusType& corpus;

  const size_t n;

 public:
  NGramCounterRule(std::unordered_map<std::vector<size_t>, size_t>& counts,
                   const CorpusType& corpus,
                   const size_t n) :
    counts(counts),
    corpus(corpus),
    n(n)
  {};

  void Base(const typename CorpusType::NGram ngram);
};

}
}

#include "ngram_counter_impl.cpp"

#endif
