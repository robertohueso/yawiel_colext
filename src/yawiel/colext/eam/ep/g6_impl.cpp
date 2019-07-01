#ifndef YAWIEL_COLEXT_EAM_EP_G6_IMPL_CPP
#define YAWIEL_COLEXT_EAM_EP_G6_IMPL_CPP

#include "g6.hpp"

using namespace std;

namespace yawiel{
namespace colext{

template<typename AMType, typename CounterType>
double G6<AMType, CounterType>::Evaluate(const vector<size_t>& ngram) const
{
  return EvaluateAM<AMType>(ngram);
}

template<typename AMType, typename CounterType>
double G6<AMType, CounterType>::EvaluatePMI(const vector<size_t>& ngram) const
{
  const size_t totalN = counter.GetNumberOfNGrams(ngram.size());
  const size_t probNGram = (double) counter.GetCounts(ngram) / totalN;

  const size_t totalBi = counter.getNumberOfNGrams(2);
  double mult = 1;
  for (size_t i = 0; i < ngram.size(); i += 2)
  {
    mult *=
        (double) counter.GetCounts(vector<size_t> {ngram[i], ngram[i + 1]}) /
        totalBi;
  }

  return std::log2(probNGram / mult);
}

template<typename AMType, typename CounterType>
double G6<AMType, CounterType>::EvaluateDice(const vector<size_t>& ngram) const
{
  const size_t n = ngram.size();

  size_t sum = 0;
  for (size_t i = 0; i < n; i += 2)
    sum += counter.GetCounts(std::vector<size_t> {ngram[i], ngram[i + 1]});

  return (double) (n * counter.GetCounts(ngram)) / sum;
}

}
}

#endif
