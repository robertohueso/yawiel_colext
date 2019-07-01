#ifndef YAWIEL_COLEXT_EAM_EP_G0_IMPL_CPP
#define YAWIEL_COLEXT_EAM_EP_G0_IMPL_CPP

#include "g0.hpp"

using namespace std;

namespace yawiel{
namespace colext{

template<typename AMType, typename CounterType>
double G0<AMType, CounterType>::Evaluate(const vector<size_t>& ngram) const
{
  return EvaluateAM<AMType>(ngram);
}

template<typename AMType, typename CounterType>
double G0<AMType, CounterType>::EvaluatePMI(const vector<size_t>& ngram) const
{
  const size_t totalN = counter.GetNumberOfNGrams(ngram.size());
  const double probNGram = (double) counter.GetCounts(ngram) / totalN;

  const size_t total1 = counter.GetNumberOfNGrams(1);
  double mult = 1;
  for (size_t i = 0; i < ngram.size(); ++i)
    mult *= (double) counter.GetCounts(std::vector<size_t> {ngram[i]}) / total1;

  return std::log2(probNGram / mult);
}

template<typename AMType, typename CounterType>
double G0<AMType, CounterType>::EvaluateDice(const vector<size_t>& ngram) const
{
  const size_t n = ngram.size();

  size_t sum = 0;
  for (size_t i = 0; i < n; ++i)
    sum += counter.GetCounts(std::vector<size_t> {ngram[i]});

  return (double) (n * counter.GetCounts(ngram)) / sum;
}

}
}

#endif
