#ifndef YAWIEL_CORE_TEXT_CORPUS_HPP
#define YAWIEL_CORE_TEXT_CORPUS_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/containers/vocabulary.hpp>
#include <vector>

namespace yawiel{
namespace text{

template<typename StringType = std::string>
class Corpus
{
 private:
  //! Vocabulary of the corpus.
  containers::Vocabulary<StringType>* vocabulary;

  //! Corpus as a sequence of tokens IDs.
  std::vector<size_t>* corpusTokens;

  //! Separators for the corpus tokens.
  StringType separators;

  //! If true, the Corpus object is responsible for deleting the vocabulary.
  bool ownVocabulary;

  //! If true, the Corpus object is responsible for deleting the corpus sequence
  //! of IDs.
  bool ownCorpusTokens;

  //! Whether a corpus has already been loaded or not.
  bool loaded;

  /**
   * Tokenize a piece of text.
   *
   * @param text Text string to tokenize.
   * @param tokensStrings Tokenized text strings.
   */
  void TokenizeLine(StringType& text,
                    std::vector<StringType>& tokensStrings);

 public:
  //! N-Gram as a pair of the first index in the sequence and the last.
  typedef std::pair<size_t, size_t> NGram;

  //! Corpus default constructor.
  Corpus();

  //! Copy constructor.
  Corpus(const Corpus<StringType>& other);

  //! Move constructor.
  Corpus(Corpus<StringType>&& other);

  //! Corpus destructor.
  ~Corpus();

  /**
   * Load a corpus from a text file.
   *
   * @param filePath Path to the file that is going to be loaded.
   */
  void LoadFile(const std::string& filePath);

  /**
   * Load a corpus from string.
   *
   * @param text Text string to be loaded.
   */
  void LoadString(StringType text);

  /**
   * Modify separators for the current corpus.
   *
   * Has to bee called before loading the corpus.
   *
   * @param newSeparators Separators for the corpus.
   */
  void SetSeparators(StringType newSeparators);

  /**
   * Transform a sequence of tokens IDs into strings.
   *
   * @pre The corpus has to be previously loaded.
   * @param ngram Sequence of tokens IDs.
   * @param separator CharType used to separate the token strings.
   */
  StringType VectorToString(const std::vector<size_t>& ngram,
                            typename StringType::value_type separator) const;

  //! Total ammount of NGrams contained in the corpus for a given N.
  size_t TotalNGrams(const size_t n) const;

  //! Get sequence of token IDs in the corpus.
  const std::vector<size_t>& GetTokens() const { return *corpusTokens; };

  //! Get the vocabulary of the corpus.
  const containers::Vocabulary<StringType>& GetVocabulary() const
      { return *vocabulary; }

  //! Get whether a corpus has already been loaded.
  bool Loaded() const { return loaded; }

  template<typename T = StringType>
  T GetDefaultSeparators(const typename std::enable_if<
                         std::is_same<T, std::string>::value>::type* = 0)
  { return " :!?=,.()[]\"/_-\'*+#"; }

  template<typename T = StringType>
  T GetDefaultSeparators(const typename std::enable_if<
                         std::is_same<T, std::wstring>::value>::type* = 0)
  { return L" :!?=,.()[]\"/_-\'*+#"; }

  template<typename T = StringType>
  T GetDefaultSeparators(const typename std::enable_if<
                         std::is_same<T, std::u16string>::value>::type* = 0)
  { return u" :!?=,.()[]\"/_-\'*+#"; }

  template<typename T = StringType>
  T GetDefaultSeparators(const typename std::enable_if<
                         std::is_same<T, std::u32string>::value>::type* = 0)
  { return U" :!?=,.()[]\"/_-\'*+#"; }

  //! Serialize corpus into an archive.
  template<typename Archive>
  void serialize(Archive& ar, const unsigned int /* version */);
};

}
}

#include "corpus_impl.cpp"

#endif
