#ifndef YAWIEL_COLEXT_EAM_EP_G1_HPP
#define YAWIEL_COLEXT_EAM_EP_G1_HPP

#include <yawiel/prereqs.hpp>
#include <vector>

namespace yawiel{
namespace colext{

template<typename AMType, typename CounterType = typename AMType::AMCounterType>
class G1
{
 private:
  //! Counter from which counts will be obtained.
  CounterType& counter;

  //! Association measure that will be applied.
  AMType am;

 public:
  //! Convenience typedef.
  typedef AMType EPAMType;

  //! Constructor.
  G1(CounterType& counter, AMType am) :
      counter(counter),
      am(am)
  {}

  //! Get the AM.
  AMType& AM() { return am; }

  /**
   * Precompute all counts that will be needed for the EP.
   *
   * This is NOT a thread safe method.
   *
   * @param maxN Maximum size of the ngram that the EP will be used with.
   */
  void Precompute(const size_t maxN) { am.Precompute(maxN); }

  /**
   * Evaluate the EP on the given ngram.
   *
   * This is a thread safe method.
   *
   * @pre Precomputations have to be made before calling evaluation.
   * @param ngram.
   */
  double Evaluate(const std::vector<size_t>& ngram) const;
};

}
}

#include "g1_impl.cpp"

#endif
