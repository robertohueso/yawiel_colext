#ifndef YAWIEL_COLEXT_EAM_AM_PMI_IMPL_CPP
#define YAWIEL_COLEXT_EAM_AM_PMI_IMPL_CPP

#include "pmi.hpp"
#include <yawiel/prereqs.hpp>
#include <iostream>
using namespace std;

namespace yawiel{
namespace colext{

template<typename CounterType>
void PMI<CounterType>::Precompute(const size_t maxN)
{
  for (size_t i = maxN; i > 0; --i)
    counter.ComputeCounts(i);
}

template<typename CounterType>
double PMI<CounterType>::Evaluate(const vector<size_t>& ngram1,
                                  const vector<size_t>& ngram2) const
{
  const size_t sizeNGram1 = ngram1.size();
  const size_t sizeNGram2 = ngram2.size();
  const size_t sizeNGram = sizeNGram1 + sizeNGram2;
  const size_t totalN1 = counter.GetNumberOfNGrams(sizeNGram1);
  const size_t totalN2 = counter.GetNumberOfNGrams(sizeNGram2);
  const size_t totalN = counter.GetNumberOfNGrams(sizeNGram);

  vector<size_t> bothGrams;
  bothGrams.reserve(sizeNGram);
  bothGrams.insert(bothGrams.end(), ngram1.begin(), ngram1.end());
  bothGrams.insert(bothGrams.end(), ngram2.begin(), ngram2.end());

  const size_t counts1 = counter.GetCounts(ngram1);
  const size_t counts2 = counter.GetCounts(ngram2);
  const size_t countsBoth = counter.GetCounts(bothGrams);

  const double prob1 = (double) counts1 / totalN1;
  const double prob2 = (double) counts2 / totalN2;
  const double probBoth = (double) countsBoth / totalN;

  return std::log2(probBoth / (prob1 * prob2));
}

}
}

#endif
