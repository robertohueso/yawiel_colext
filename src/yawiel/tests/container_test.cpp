#include <yawiel/core/containers/vocabulary.hpp>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace yawiel;
using namespace yawiel::containers;

BOOST_AUTO_TEST_SUITE(VocabularyTest);

BOOST_AUTO_TEST_CASE(BasicVocabulary)
{
  Vocabulary<std::u16string> vocabulary;
  std::u16string a = u"linux";
  std::u16string b = u"ubuntu";
  std::u16string c = u"debian";

  vocabulary.AddToken(a);
  vocabulary.AddToken(b);
  vocabulary.AddToken(c);
  vocabulary.AddToken(b);

  const size_t idA = vocabulary.at(a);
  const size_t idB = vocabulary.at(b);
  const size_t idC = vocabulary.at(c);

  BOOST_REQUIRE_EQUAL(idA, vocabulary.at(vocabulary.at(idA)));
  BOOST_REQUIRE_EQUAL(idC, vocabulary.at(vocabulary.at(idC)));
  BOOST_REQUIRE_EQUAL(idB, vocabulary.at(vocabulary.at(idB)));
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  Vocabulary<std::u16string> vocabulary;
  std::u16string a = u"linux";
  std::u16string b = u"ubuntu";
  std::u16string c = u"debian";

  vocabulary.AddToken(a);
  vocabulary.AddToken(b);
  vocabulary.AddToken(c);
  vocabulary.AddToken(b);

  Vocabulary<std::u16string> copy(vocabulary);

  BOOST_REQUIRE_EQUAL(copy.at(a), vocabulary.at(a));
  BOOST_REQUIRE_EQUAL(copy.at(b), vocabulary.at(b));
  BOOST_REQUIRE_EQUAL(copy.at(c), vocabulary.at(c));
  BOOST_REQUIRE_EQUAL(copy.Size(), vocabulary.Size());
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  Vocabulary<std::u16string> vocabulary;
  std::u16string a = u"linux";
  std::u16string b = u"ubuntu";
  std::u16string c = u"debian";

  vocabulary.AddToken(a);
  vocabulary.AddToken(b);
  vocabulary.AddToken(c);
  vocabulary.AddToken(b);

  const size_t idA = vocabulary.at(a);
  const size_t idB = vocabulary.at(b);
  const size_t idC = vocabulary.at(c);

  Vocabulary<std::u16string> other(std::move(vocabulary));

  BOOST_REQUIRE_EQUAL(other.at(a), idA);
  BOOST_REQUIRE_EQUAL(other.at(b), idB);
  BOOST_REQUIRE_EQUAL(other.at(c), idC);
  BOOST_REQUIRE_EQUAL(other.Size(), 3);
  BOOST_REQUIRE_EQUAL(vocabulary.Size(), 0);
}

BOOST_AUTO_TEST_SUITE_END();
