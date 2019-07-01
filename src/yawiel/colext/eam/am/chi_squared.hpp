#ifndef YAWIEL_COLEXT_EAM_AM_CHI_SQUARED_HPP
#define YAWIEL_COLEXT_EAM_AM_CHI_SQUARED_HPP

#include <yawiel/prereqs.hpp>

namespace yawiel{
namespace colext{

template<typename CounterType>
class ChiSquared
{
 private:
  //! Counter from which counts will be obtained.
  CounterType& counter;

  //! Evaluate binary case.
  double EvaluateBinary(const std::vector<size_t>& ngram1,
                        const std::vector<size_t>& ngram2) const;
 public:
  //! Convenience typedef.
  typedef CounterType AMCounterType;

  //! Constructor.
  ChiSquared(CounterType& counter) :
      counter(counter)
  {}

  /**
   * Precompute all counts that will be needed for the AM.
   * Algorithmic complexity O(n).
   *
   * This is NOT a thread safe method.
   *
   * @param maxN Sum of the sizes of the pairs of ngrams with the maximum size
   *             that the AM will be used with.
   */
  void Precompute(const size_t maxN);

  /**
   * Evaluate the AM on the given pair of ngrams.
   *
   * This is a thread safe method.
   *
   * @pre Precomputations have to be made before calling evaluation.
   * @param ngram1 First ngram.
   * @param ngram2 Second ngram.
   */
  double Evaluate(const std::vector<size_t>& ngram1,
                  const std::vector<size_t>& ngram2) const;
};

}
}

#include "chi_squared_impl.cpp"

#endif
