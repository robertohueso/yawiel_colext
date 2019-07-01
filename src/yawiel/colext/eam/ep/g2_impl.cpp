#ifndef YAWIEL_COLEXT_EAM_EP_G2_IMPL_CPP
#define YAWIEL_COLEXT_EAM_EP_G2_IMPL_CPP

#include "g2.hpp"

using namespace std;

namespace yawiel{
namespace colext{

template<typename AMType, typename CounterType>
double G2<AMType, CounterType>::Evaluate(const vector<size_t>& ngram) const
{
  const size_t splitIndex = ngram.size() / 2;
  const vector<size_t> firstSplitFirst(ngram.begin(),
                                       ngram.begin() + splitIndex);
  const vector<size_t> firstSplitSecond(ngram.begin() + splitIndex,
                                        ngram.end());
  const vector<size_t> secondSplitFirst(ngram.begin(),
                                        ngram.begin() + splitIndex + 1);
  const vector<size_t> secondSplitSecond(ngram.begin() + splitIndex + 1,
                                         ngram.end());

  const double am1 = am.Evaluate(firstSplitFirst, firstSplitSecond);
  const double am2 = am.Evaluate(secondSplitFirst, secondSplitSecond);
  return (am1 + am2) / 2.0;
}

}
}

#endif
