#ifndef YAWIEL_CORE_TEXT_CORPUS_GRAM_TRAVERSER_HPP
#define YAWIEL_CORE_TEXT_CORPUS_GRAM_TRAVERSER_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/text/corpus.hpp>

namespace yawiel{
namespace text{

template<typename RuleType, typename StringType>
class GramTraverser
{
 public:
  GramTraverser(RuleType& rule, const Corpus<StringType>& corpus);

  void Traverse(const size_t n = 2,
                const size_t startIndex = 0,
                const size_t endIndex = 0,
                const bool leftToRight = true);

 private:
  RuleType& rule;

  const Corpus<StringType>& corpus;
};

}
}

#include "gram_traverser_impl.cpp"

#endif
