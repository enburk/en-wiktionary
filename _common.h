inline std::unordered_map<str,str> redirect;
inline std::unordered_map<str,str> redirect_modules;
inline std::unordered_map<str,str> redirect_templates;
inline std::unordered_map<str,str> templates;
inline std::unordered_map<str,str> modules;

std::unordered_set<str> lexical_items
{
    "noun", "pronoun", "proper noun", "verb", "adjective", "adverb", "numeral", "number", "article", "particle", 
    "preposition", "postposition", "determiner", "conjunction", "interjection",
    "abbreviation", "initialism", "contraction", "acronym", 
    "letter", "symbol", "punctuation mark", "diacritical mark",
    "suffix", "prefix", "infix", "affix", "interfix",
    "phrase", "prepositional phrase", "proverb", 
};
std::unordered_set<str> related_items
{
    "alternative forms", "abbreviations",
    "synonyms", "antonyms", "hypernyms", "hyponyms", "meronyms", "holonyms", "troponyms",
    "derived terms", "related terms", "coordinate terms",
    "see also", 
};
std::unordered_set<str> lexical_notes
{
    "pronunciation", "etymology",
    "usage notes", "trivia",
};

