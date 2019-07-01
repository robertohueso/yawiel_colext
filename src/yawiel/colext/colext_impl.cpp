#ifndef YAWIEL_COLEXT_IMPL_CPP
#define YAWIEL_COLEXT_IMPL_CPP

#include "colext.hpp"
#include <yawiel/colext/eam/ep/g4.hpp>
#include <omp.h>

using namespace std;
using namespace yawiel::util;
using namespace yawiel::text;

namespace yawiel{
namespace colext{

template<typename EPType, typename StringType>
Colext<EPType, StringType>::Colext() :
    counter(nullptr),
    ownCounter(false),
    trained(false)
{
  // Nothing to do here.
}

template<typename EPType, typename StringType>
Colext<EPType, StringType>::Colext(util::NGramCounter<StringType>* counter) :
  counter(counter),
  ownCounter(false),
  trained(true)
{
  // Nothing to do here.
}

template<typename EPType, typename StringType>
Colext<EPType, StringType>::~Colext()
{
  if (ownCounter)
    delete counter;
}

template<typename EPType, typename StringType>
void Colext<EPType, StringType>::
LoadCorpus(const text::Corpus<StringType>& corpus)
{
  // Free resources.
  if (ownCounter)
    delete counter;

  // Create counter.
  counter = new util::NGramCounter<StringType>(corpus);
  ownCounter = true;
  trained = true;
}

template<typename EPType, typename StringType>
void Colext<EPType, StringType>::Precompute(const size_t n, EPType& ep)
{
  if (std::is_same<EPType, G4<typename EPType::EPAMType,
                              NGramCounter<StringType>>>::value)
  {
    ep.Precompute(2 * (n - 1));
  }
  else
  {
    ep.Precompute(n);
  }
}

template<typename EPType, typename StringType>
void Colext<EPType, StringType>::
NormalizeScores(vector<pair<vector<size_t>, double>>& scores)
{
  #pragma omp parallel for
  for (size_t i = 0; i < scores.size(); ++i)
    scores[i].second = i;
}

template<typename EPType, typename StringType>
void Colext<EPType, StringType>::
GetScores(unordered_map<vector<size_t>, double>& scores,
          const size_t n,
          bool normalizeScore)
{
  if (!trained)
  {
    throw std::runtime_error("Can't get scores: Colext model needs to be "
                             "trained");
  }

  typename EPType::EPAMType am(*counter);
  EPType ep(*counter, am);

  // Precompute counts.
  Precompute(n, ep);

  // Get scores ready.
  scores.clear();
  const unordered_map<vector<size_t>, size_t>& counts = counter->GetCounts(n);

  // Actual scores computations.
  for (auto it = counts.begin(); it != counts.end(); ++it)
    scores[it->first] = ep.Evaluate(it->first);

  // Normalize scores if required.
  if (normalizeScore)
  {
    vector<pair<vector<size_t>, double>> sortedScores;
    sortedScores.reserve(counts.size());
    sortedScores.resize(counts.size());
    size_t index = 0;
    for (auto kv = scores.cbegin(); kv != scores.cend(); ++kv, ++index)
      sortedScores[index].first = kv->first;

    // Actual normalization.
    std::sort(sortedScores.begin(), sortedScores.end(), SortScoresPred());
    NormalizeScores(sortedScores);

    // Store values back in the hash table.
    for (size_t i = sortedScores.size() - 1; i >= 0; --i)
    {
      scores[sortedScores[i].first] = sortedScores[i].second;
      sortedScores.pop_back();
    }
  }
}

template<typename EPType, typename StringType>
void Colext<EPType, StringType>::
GetSortedScores(vector<pair<vector<size_t>, double>>& scores,
                const size_t n,
                bool normalizeScore)
{
  if (!trained)
  {
    throw std::runtime_error("Can't get scores: Colext model needs to be "
                             "trained");
  }

  typename EPType::EPAMType am(*counter);
  EPType ep(*counter, am);

  // Precompute counts.
  Precompute(n, ep);

  // Get scores ready.
  const unordered_map<vector<size_t>, size_t>& counts = counter->GetCounts(n);
  scores.clear();
  scores.reserve(counts.size());
  scores.resize(counts.size());

  // Add ngrams to scores.
  size_t index = 0;
  for (auto kv = counts.cbegin(); kv != counts.cend(); ++kv, ++index)
    scores[index].first = kv->first;

  // Compute scores using parallelization if possible.
  #pragma omp parallel for
  for (size_t i = 0; i < scores.size(); ++i)
    scores[i].second = ep.Evaluate(scores[i].first);

  // Sort results
  std::sort(scores.begin(), scores.end(), SortScoresPred());

  // Normalize scores if required.
  if (normalizeScore)
    NormalizeScores(scores);
}

template<typename EPType, typename StringType>
template<typename IterablePairType>
void Colext<EPType, StringType>::
ScoresToCSV(const IterablePairType& scores,
            const std::string filePath,
            const typename StringType::value_type csvSeparator) const
{
  const text::Corpus<StringType>& corpus = counter->Corpus();
  // Open file.
  std::basic_ofstream<typename StringType::value_type>
      file(filePath.c_str(), std::ios::trunc | std::ios::out);

  // Write header.
  file << "ngram" << csvSeparator
       << "score" << csvSeparator
       << "frequency" << std::endl;

  // Write data.
  for (const auto it = scores.cbegin(); it != scores.cend(); ++it)
  {
    // Write string to file.
    file << QUOTE << corpus.VectorToString(it->first, STRING_SEPARATOR)
         << QUOTE;
    // Write score.
    file << csvSeparator << it->second;
    // Write frequency.
    file << csvSeparator << counter->GetCounts(it->first)
         << std::endl;
  }
  file.close();
}

template<typename EPType, typename StringType>
void Colext<EPType, StringType>::SaveModel(const std::string& filePath) const
{
  if (!trained)
    throw std::runtime_error("Can't save model: Colext model needs to be "
                             "trained");

  // Open file.
  std::ofstream file(filePath.c_str(), std::ios::trunc | std::ios::out);

  // Save to archive.
  {
    boost::archive::text_oarchive outputArchive(file);
    outputArchive << *counter;
  }
  file.close();
}

template<typename EPType, typename StringType>
void Colext<EPType, StringType>::LoadModel(const std::string& filePath)
{
  if (!counter)
  {
    counter = new util::NGramCounter<StringType>;
    ownCounter = true;
  }

  // Open file.
  std::ifstream file(filePath.c_str());

  // Load from archive.
  {
    boost::archive::text_iarchive inputArchive(file);
    inputArchive >> *counter;
  }
  trained = true;
  file.close();
}

}
}

#endif
