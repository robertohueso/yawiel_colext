#ifndef YAWIEL_COLEXT_EAM_EP_G1_IMPL_CPP
#define YAWIEL_COLEXT_EAM_EP_G1_IMPL_CPP

#include "g1.hpp"

using namespace std;

namespace yawiel{
namespace colext{

template<typename AMType, typename CounterType>
double G1<AMType, CounterType>::Evaluate(const vector<size_t>& ngram) const
{
  const vector<size_t> firstSplitFirst{ngram[0]};
  const vector<size_t> firstSplitSecond(ngram.begin() + 1, ngram.end());
  const vector<size_t> secondSplitFirst(ngram.begin(), ngram.end() - 1);
  const vector<size_t> secondSplitSecond{ngram[ngram.size() - 1]};

  const double am1 = am.Evaluate(firstSplitFirst, firstSplitSecond);
  const double am2 = am.Evaluate(secondSplitFirst, secondSplitSecond);
  return (am1 + am2) / 2.0;
}

}
}

#endif
