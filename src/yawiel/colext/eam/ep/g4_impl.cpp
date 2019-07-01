#ifndef YAWIEL_COLEXT_EAM_EP_G4_IMPL_CPP
#define YAWIEL_COLEXT_EAM_EP_G4_IMPL_CPP

#include "g4.hpp"

using namespace std;

namespace yawiel{
namespace colext{

template<typename AMType, typename CounterType>
double G4<AMType, CounterType>::Evaluate(const vector<size_t>& ngram) const
{
  const vector<size_t> splitFirst(ngram.begin(), ngram.end() - 1);
  const vector<size_t> splitSecond(ngram.begin() + 1, ngram.end());

  return am.Evaluate(splitFirst, splitSecond);
}

}
}

#endif
