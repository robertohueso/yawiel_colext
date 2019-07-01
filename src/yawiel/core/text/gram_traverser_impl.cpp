#ifndef YAWIEL_CORE_TEXT_CORPUS_GRAM_TRAVERSER_IMPL_CPP
#define YAWIEL_CORE_TEXT_CORPUS_GRAM_TRAVERSER_IMPL_CPP

#include "gram_traverser.hpp"

namespace yawiel{
namespace text{

template<typename RuleType, typename StringType>
GramTraverser<RuleType, StringType>::
GramTraverser(RuleType& rule, const Corpus<StringType>& corpus) :
  rule(rule),
  corpus(corpus)
{}

template<typename RuleType, typename StringType>
void GramTraverser<RuleType, StringType>::
Traverse(const size_t n,
         const size_t startIndex,
         const size_t endIndex,
         const bool leftToRight)
{
  size_t lastIndex;
  if (endIndex != 0)
    lastIndex = endIndex;
  else
    lastIndex = (corpus.GetTokens()).size();

  if (leftToRight)
  {
    size_t firstToken = startIndex;
    size_t lastToken = firstToken + n;
    for (; lastToken <= lastIndex; ++firstToken, ++lastToken)
      rule.Base(std::make_pair(firstToken, lastToken));
  }
  else
  {
    size_t firstToken = lastIndex;
    size_t lastToken = lastIndex - n;
    for (; lastToken >= startIndex; --firstToken, --lastToken)
      rule.Base(std::make_pair(firstToken, lastToken));
  }
}

}
}

#endif
