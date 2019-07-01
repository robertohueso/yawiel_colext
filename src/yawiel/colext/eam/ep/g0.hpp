#ifndef YAWIEL_COLEXT_EAM_EP_G0_HPP
#define YAWIEL_COLEXT_EAM_EP_G0_HPP

#include <yawiel/prereqs.hpp>
#include <vector>
#include <yawiel/colext/eam/am/pmi.hpp>
#include <yawiel/colext/eam/am/dice.hpp>
#include <yawiel/colext/eam/am/chi_squared.hpp>
#include <yawiel/colext/eam/am/log_likelihood.hpp>

namespace yawiel{
namespace colext{

template<typename AMType, typename CounterType = typename AMType::AMCounterType>
class G0
{
  private:
  //! Counter from which counts will be obtained.
  CounterType& counter;

  //! Association measure that will be applied.
  AMType am;

  //! Evaluate PMI using G0.
  double EvaluatePMI(const std::vector<size_t>& ngram) const;

  //! Evaluate Dice using G0.
  double EvaluateDice(const std::vector<size_t>& ngram) const;

  //! SFINAE Evaluate for PMI AM.
  template<typename AMEvalType>
  inline double EvaluateAM(
      const std::vector<size_t>& ngram,
      const typename std::enable_if<
          std::is_same<AMEvalType, PMI<CounterType>>::value>::type* = 0) const
  { return EvaluatePMI(ngram); }

  //! SFINAE Evaluate for Dice AM.
  template<typename AMEvalType>
  inline double EvaluateAM(
      const std::vector<size_t>& ngram,
      const typename std::enable_if<
          std::is_same<AMEvalType, Dice<CounterType>>::value>::type* = 0)
  const
  { return EvaluateDice(ngram); }

  //! SFINAE Evaluate for ChiSquared AM.
  template<typename AMEvalType>
  inline double EvaluateAM(
      const std::vector<size_t>& ngram,
      const typename std::enable_if<
          std::is_same<AMEvalType, ChiSquared<CounterType>>::value>::type* = 0)
  const
  { return am.Evaluate(ngram, std::vector<size_t>()); }

  //! SFINAE Evaluate for LogLikelihood AM.
  template<typename AMEvalType>
  inline double EvaluateAM(
      const std::vector<size_t>& ngram,
      const typename std::enable_if<
          std::is_same<AMEvalType, LogLikelihood<CounterType>>::value
      >::type* = 0) const
  { return am.Evaluate(ngram, std::vector<size_t>()); }

 public:
  //! Convenience typedef.
  typedef AMType EPAMType;

  //! Constructor.
  G0(CounterType& counter, AMType am) :
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

#include "g0_impl.cpp"

#endif
