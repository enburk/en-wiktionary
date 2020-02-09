inline std::unordered_map<str,str> redirect;
inline std::unordered_map<str,str> redirect_modules;
inline std::unordered_map<str,str> redirect_templates;
inline std::unordered_map<str,str> Templates;
inline std::unordered_map<str,str> Modules;

str ellipsis   =  "\xE2" "\x80" "\xA6"; // …
str mdash      =  "\xE2" "\x80" "\x94"; // m-dash
str ndash      =  "\xE2" "\x80" "\x93"; // n-dash
str oquot      =  "\xE2" "\x80" "\x9C"; // “
str cquot      =  "\xE2" "\x80" "\x9D"; // ”
str apostrophe =  "\xE2" "\x80" "\x99"; // ’

str digit = "0123456789";
str Latin = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
str latin = "abcdefghijklmnopqrstuvwxyz";
str alnum = Latin + latin + digit;
str Alnum = alnum + (char*)(u8"'- ");
str ALnum = alnum + (char*)(u8"'- ,");
str ALNum = alnum + (char*)(u8"'- ,&+/?!.") + ellipsis + mdash + ndash + oquot + cquot + apostrophe;
str ALNUm = ALNum + (char*)(u8"()");
str ALNUM = ALNUm + (char*)
(
    u8"Α α Β β Γ γ Δ δ Ε ε Ζ ζ Η η Θ θ Ι ι"
    u8"Κ κ Λ λ Μ μ Ν ν Ξ ξ Ο ο Π π Ρ ρ Σ σ ς Τ τ Υ υ Φ φ Χ χ Ψ ψ Ω ω"
    // Page 00 ((Control characters,) Basic Latin, Latin-1 Supplement
    u8"€  ‚ ƒ „ … † ‡ ˆ ‰ Š ‹ Œ  Ž   ‘ ’ “ ” • – — ˜ ™ š › œ  ž Ÿ     "
    u8"  ¡ ¢ £ ¤ ¥ ¦ § ¨ © ª « ¬ ­ ® ¯ ° ± ² ³ ´ µ ¶ · ¸ ¹ º » ¼ ½ ¾ ¿ "
    u8"À Á Â Ã Ä Å Æ Ç È É Ê Ë Ì Í Î Ï Ð Ñ Ò Ó Ô Õ Ö × Ø Ù Ú Û Ü Ý Þ ß"
    u8"à á â ã ä å æ ç è é ê ë ì í î ï ð ñ ò ó ô õ ö ÷ ø ù ú û ü ý þ ÿ"
    // Page 01 (Latin Extended-A, Latin Extended-B)
    u8"Ā ā Ă ă Ą ą Ć ć Ĉ ĉ Ċ ċ Č č Ď ď Đ đ Ē ē Ĕ ĕ Ė ė Ę ę Ě ě Ĝ ĝ Ğ ğ"
    u8"Ġ ġ Ģ ģ Ĥ ĥ Ħ ħ Ĩ ĩ Ī ī Ĭ ĭ Į į İ ı Ĳ ĳ Ĵ ĵ Ķ ķ ĸ Ĺ ĺ Ļ ļ Ľ ľ Ŀ"
    u8"ŀ Ł ł Ń ń Ņ ņ Ň ň ŉ Ŋ ŋ Ō ō Ŏ ŏ Ő ő Œ œ Ŕ ŕ Ŗ ŗ Ř ř Ś ś Ŝ ŝ Ş ş"
    u8"Š š Ţ ţ Ť ť Ŧ ŧ Ũ ũ Ū ū Ŭ ŭ Ů ů Ű ű Ų ų Ŵ ŵ Ŷ ŷ Ÿ Ź ź Ż ż Ž ž ſ"
    u8"ƀ Ɓ Ƃ ƃ Ƅ ƅ Ɔ Ƈ ƈ Ɖ Ɗ Ƌ ƌ ƍ Ǝ Ə Ɛ Ƒ ƒ Ɠ Ɣ ƕ Ɩ Ɨ Ƙ ƙ ƚ ƛ Ɯ Ɲ ƞ Ɵ"
    u8"Ơ ơ Ƣ ƣ Ƥ ƥ Ʀ Ƨ ƨ Ʃ ƪ ƫ Ƭ ƭ Ʈ Ư ư Ʊ Ʋ Ƴ ƴ Ƶ ƶ Ʒ Ƹ ƹ ƺ ƻ Ƽ ƽ ƾ ƿ"
    u8"ǀ ǁ ǂ ǃ Ǆ ǅ ǆ Ǉ ǈ ǉ Ǌ ǋ ǌ Ǎ ǎ Ǐ ǐ Ǒ ǒ Ǔ ǔ Ǖ ǖ Ǘ ǘ Ǚ ǚ Ǜ ǜ ǝ Ǟ ǟ"
    u8"Ǡ ǡ Ǣ ǣ Ǥ ǥ Ǧ ǧ Ǩ ǩ Ǫ ǫ Ǭ ǭ Ǯ ǯ ǰ Ǳ ǲ ǳ Ǵ ǵ Ǻ ǻ Ǽ ǽ Ǿ ǿ"
);

array<str> lexical_items
{
    "noun", "pronoun", "proper noun", "verb", "adjective", "adverb", "numeral", "number", "article", "particle", 
    "preposition", "postposition", "determiner", "conjunction", "interjection",
    "initialism", "contraction", "acronym", 
    "letter", "symbol", "punctuation mark", "diacritical mark",
    "suffix", "prefix", "infix", "affix", "interfix",
    "phrase", "prepositional phrase", "proverb", 
};
array<str> related_items
{
    "alternative forms", "abbreviations",
    "synonyms", "antonyms", "hypernyms", "hyponyms", "meronyms", "holonyms", "troponyms",
    "derived terms", "related terms", "coordinate terms",
    "see also", 
};
array<str> lexical_notes
{
    "pronunciation", "etymology",
    "usage notes", "trivia",
};
