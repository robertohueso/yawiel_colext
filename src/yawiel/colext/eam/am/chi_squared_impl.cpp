#ifndef YAWIEL_COLEXT_EAM_AM_CHI_SQUARED_IMPL_CPP
#define YAWIEL_COLEXT_EAM_AM_CHI_SQUARED_IMPL_CPP

#include "chi_squared.hpp"
#include <yawiel/prereqs.hpp>

using namespace std;

namespace yawiel{
namespace colext{

template<typename CounterType>
void ChiSquared<CounterType>::Precompute(const size_t maxN)
{
  for (size_t i = maxN; i > 0; --i)
    counter.ComputeCounts(i);
}

template<typename CounterType>
double ChiSquared<CounterType>::EvaluateBinary(
    const vector<size_t>& ngram1, const vector<size_t>& ngram2) const
{
  // Calculate contingency table.
  // Counts both.
  const size_t sizeNGram = ngram1.size() + ngram2.size();
  vector<size_t> bothGrams;
  bothGrams.reserve(sizeNGram);
  bothGrams.insert(bothGrams.end(), ngram1.begin(), ngram1.end());
  bothGrams.insert(bothGrams.end(), ngram2.begin(), ngram2.end());
  const size_t both = counter.GetCounts(bothGrams);
  // Counts ngram1 only.
  const size_t only1 = counter.GetCounts(ngram1) - both;
  // Counts ngram2 only.
  const size_t only2 = counter.GetCounts(ngram2) - both;
  // Total count of ngrams.
  size_t totalCount = 0;
  for (size_t i = 1; i <= bothGrams.size(); ++i)
    totalCount += counter.GetNumberOfNGrams(i);
  // Counts neither ngram1 nor ngram2.
  const size_t neither = totalCount - both - only1 - only2;

  // Marginal probabilities.
  const double prob1 = (double) (both + only1) / totalCount;
  const double prob2 = (double) (both + only2) / totalCount;

  // Expected values.
  const double expBoth = totalCount * prob1 * prob2;
  const double expOnly1 = totalCount * prob1 * (1 - prob2);
  const double expOnly2 = totalCount * (1 - prob1) * prob2;
  const double expNeither = totalCount * (1 - prob1) * (1 - prob2);

  // Coefficient.
  const double diffBoth = both - expBoth;
  const double diffOnly1 = only1 - expOnly1;
  const double diffOnly2 = only2 - expOnly2;
  const double diffNeither = neither - expNeither;
  const double C2 = (diffBoth * diffBoth) / expBoth +
                    (diffOnly1 * diffOnly1) / expOnly1 +
                    (diffOnly2 * diffOnly2) / expOnly2 +
                    (diffNeither * diffNeither) / expNeither;

  return C2;
}

template<typename CounterType>
double ChiSquared<CounterType>::Evaluate(const vector<size_t>& ngram1,
                                         const vector<size_t>& ngram2) const
{
  double sum = 0;
  const size_t sizeNGram = ngram1.size() + ngram2.size();
  vector<size_t> bothGrams;
  bothGrams.reserve(sizeNGram);
  bothGrams.insert(bothGrams.end(), ngram1.begin(), ngram1.end());
  bothGrams.insert(bothGrams.end(), ngram2.begin(), ngram2.end());

  // Binary evaluate all possible combinations of ngram.
  for (size_t i = 1; i < sizeNGram; ++i)
  {
    vector<size_t> gram1(bothGrams.begin(), bothGrams.begin() + i);
    vector<size_t> gram2(bothGrams.begin() + i, bothGrams.end());
    sum += EvaluateBinary(gram1, gram2);
  }

  return sum;
}

}
}

#endif
