#ifndef YAWIEL_COLEXT_HPP
#define YAWIEL_COLEXT_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/text/corpus.hpp>
#include <yawiel/core/util/ngram_counter.hpp>

namespace yawiel{
namespace colext{

template<typename EPType, typename StringType = std::string>
class Colext
{
 private:
  // Default strings.
  static const typename StringType::value_type QUOTE = '\"';

  static const typename StringType::value_type STRING_SEPARATOR = ' ';

  static const typename StringType::value_type CSV_SEPARATOR = ',';

  //! Comparator for sorting result scores.
  struct SortScoresPred
  {
    inline bool operator()(const std::pair<std::vector<size_t>, double>& left,
                           const std::pair<std::vector<size_t>, double>& right)
    {
        return left.second < right.second;
    }
  };

  /**
   * Precompute counts needed for the EP, AM and n-gram size.
   *
   * @param n Size of n-gram collocations that we want to extract.
   * @param ep Extension for which counts will be precomputed.
   */
  void Precompute(const size_t n, EPType& ep);

  /**
   * Normalize scores.
   *
   * This method uses parallelization when available.
   *
   * @param scores Sorted vector of scores to normalize.
   */
  void NormalizeScores(
      std::vector<std::pair<std::vector<size_t>, double>>& scores);

  //! NGrams counter for the given corpus.
  util::NGramCounter<StringType>* counter;

  //! If true, the Colext model will be responsible for deleting the counter.
  bool ownCounter;

  //! Whether the model has been trained or not.
  bool trained;

 public:
  //! Default constructor. After its use, it is needed to load a corpus.
  Colext();

  /**
   * Construct Collext from an existing corpus and its counter.
   *
   * @param counter Counter of ngrams in the corpus.
   */
  Colext(util::NGramCounter<StringType>* counter);

  //! Destructor.
  ~Colext();

  /**
   * Load corpus.
   *
   * @param corpus Corpus object to train the model from.
   * @post Model will have been trained.
   */
  void LoadCorpus(const text::Corpus<StringType>& corpus);

  /**
   * Compute scores for a given size, EP and AM for n-grams in the corpus.
   *
   * This method CANNOT use paralellization.
   *
   * @pre Model needs to be trained.
   * @param scores Hash table where scores from ngrams will be stored.
   * @param n Size of the n-grams for which scores will be computed.
   * @param normalizeScore Whether to normalize scores or not.
   */
  void GetScores(std::unordered_map<std::vector<size_t>, double>& scores,
                 const size_t n,
                 bool normalizeScore = false);

  /**
   * Compute scores for a given size, EP and AM for n-grams in the corpus.
   *
   * This method can use paralellization when available.
   *
   * @pre Model needs to be trained.
   * @param scores Vector where sorted scores from ngrams will be stored.
   * @param n Size of the n-grams for which scores will be computed.
   * @param normalizeScore Whether to normalize scores or not.
   */
  void GetSortedScores(
      std::vector<std::pair<std::vector<size_t>, double>>& scores,
      const size_t n,
      bool normalizeScore = false);

  /**
   * Write computed scores to a CSV file.
   *
   * @param scores Iterable pair (i.e. pair<vector<size_t>, double>) to save in
   *               a file.
   * @param filePath Path where the CSV file will be writen.
   * @param csvSeparator Separator for column values of the CSV file.
   */
  template<typename IterablePairType>
  void ScoresToCSV(
      const IterablePairType& scores,
      const std::string filePath,
      const typename StringType::value_type csvSeparator = CSV_SEPARATOR) const;

  /**
   * Save model to file.
   *
   * @param filePath Path to the file where the model will be stored.
   */
  void SaveModel(const std::string& filePath) const;

  /**
   * Load model from file.
   *
   * @param filePath Path to the file that contains the model.
   */
  void LoadModel(const std::string& filePath);

  //! Whether the model has been trained or not.
  bool Trained() const { return trained; }

  //! Get the corpus.
  const text::Corpus<StringType>& Corpus() const;

  //! Get the counter.
  const util::NGramCounter<StringType>& Counter() const;
};

}
}

#include "colext_impl.cpp"

#endif
