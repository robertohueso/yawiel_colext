#ifndef YAWIEL_COLEXT_EAM_EP_G5_IMPL_CPP
#define YAWIEL_COLEXT_EAM_EP_G5_IMPL_CPP

#include "g5.hpp"

using namespace std;

namespace yawiel{
namespace colext{

template<typename AMType, typename CounterType>
double G5<AMType, CounterType>::Evaluate(const vector<size_t>& ngram) const
{
  const size_t n = ngram.size();
  double sum = 0;

  for (size_t i = 1; i < n; ++i)
  {
    const vector<size_t> splitFirst(ngram.begin(), ngram.begin() + i);
    const vector<size_t> splitSecond(ngram.begin() + i, ngram.end());
    sum += am.Evaluate(splitFirst, splitSecond);
  }

  return sum / (n - 1);
}

}
}

#endif
