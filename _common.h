std::map<str,str> redirect;
std::map<str,str> redirect_modules;
std::map<str,str> redirect_templates;
std::map<str,str> Templates;
std::map<str,str> Modules;
std::map<str,str> Languages;
std::map<str,lua::syntax::analysis> Repo;

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
    "noun", "pronoun", "proper noun", "verb",
	"adjective", "adverb", "numeral", "number", "article", "particle", 
    "preposition", "postposition", "determiner", "conjunction", "interjection",
    "letter", "symbol", "punctuation mark", "diacritical mark",
    "suffix", "prefix", "infix", "affix", "interfix",
    "phrase", "prepositional phrase", "proverb", 
    "initialism", "contraction", "acronym", 
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
    "pronunciation", "hyphenation", "etymology", "usage notes", "trivia",
};

struct lexform { str form, ack, word; };

std::map<str, array<lexform>> lexforms;

std::mutex lexforms_mutex;

std::map<str, std::set<str>> templates_usage;
std::map<str, std::map<str, int>> templates_statistics;
template<class Entry>  void  dump_templates_statistics (Result<Entry> & result)
{
    std::multimap<int, str, std::greater<int>> templates; int total = 0;
    for (auto & [name, n] : templates_statistics [result.passpath.string()])
	{ templates.emplace(n, name); total += n; }

	for (auto & [n, name] : templates)
	{
		array<str> redirs;
		for (auto [from, to] : redirect_templates)
			if (to == name) redirs += from;
		if (not redirs.empty())
			name += " (" + str(redirs, ", ") + ")";
	}

	for (auto & [n, name] : templates)
	result.report (std::to_string(n) + " " + name        , "# templates statistics");
    result.report ("====================================", "# templates statistics");
    result.report (std::to_string(total >> 00) + " total", "# templates statistics");
    result.report (std::to_string(total >> 10) + " K    ", "# templates statistics");
    result.report (std::to_string(total >> 20) + " M    ", "# templates statistics");
}

void logout (str pass, int64_t entries, int64_t cargo)
{
    auto c = std::to_string(cargo);
    auto e = std::to_string(entries);
    pass += str(' ', max(0, 10 - pass.size()));
    c.insert(0, max(0, 7 - (int)c.size()), ' ');
    e.insert(0, max(0, 7 - (int)e.size()), ' ');
    print(pass, " ", e, " entries ", c, " cargo ");
}

void strip (array<str> & content)
{
    for (auto & s : content) s.strip (" \n");
    
    while (not
		content.empty() and
		content.back().empty())
		content.pop_back();

    content.erase(content.begin(),
        std::find_if(content.begin(), content.end(),
            [](auto & s){ return not s.empty(); }));

    content.erase(std::unique(content.begin(), content.end(),
        [](auto & s1, auto & s2){ return s1.empty() && s2.empty(); }),
        content.end());
}

str esc = "###################################";

void save_meta (std::ofstream && fstream)
{
    for (auto & [from, to] : redirect          ) fstream << from << " ====> " << to << "\n"; fstream << esc << "\n";
    for (auto & [from, to] : redirect_templates) fstream << from << " ====> " << to << "\n"; fstream << esc << "\n";
    for (auto & [from, to] : redirect_modules  ) fstream << from << " ====> " << to << "\n"; fstream << esc << "\n";

    for (auto & [origin, forms] : lexforms)
		for (auto form : forms)
			fstream << origin << " ====> "
			<< form.form << " ## "
			<< form.ack	 << " ## "
			<< form.word << "\n";
	
	fstream << esc << "\n";

    for (auto & [name, text] : Templates) fstream
        << esc << "\n" << ("Template:" + name) << "\n"
        << esc << "\n" << text << "\n";

    for (auto & [name, text] : Modules) fstream
        << esc << "\n" << ("Module:" + name) << "\n"
        << esc << "\n" << text << "\n";

	fstream << esc << "\n";
}
void load_meta (std::ifstream && fstream)
{
	str line, title, topic;

    while (std::getline(fstream, line))
    {
        if (line == esc) break;
        str from, to; line.split_by(" ====> ", from, to);
        redirect.emplace(from, to);
    }
    while (std::getline(fstream, line))
    {
        if (line == esc) break;
        str from, to; line.split_by(" ====> ", from, to);
        redirect_templates.emplace(from, to);
    }
    while (std::getline(fstream, line))
    {
        if (line == esc) break;
        str from, to; line.split_by(" ====> ", from, to);
        redirect_modules.emplace(from, to);
    }
    while (std::getline(fstream, line))
    {
        if (line == esc) break;
        str orig; line.split_by(" ====> ", orig, line);
		str form; line.split_by(" ## "   , form, line);
		str ack;  line.split_by(" ## "   , ack,  line);
		str word; line.split_by(" ## "   , word, line);
		lexforms[orig] += lexform{form, ack, word};
    }
    while (std::getline(fstream, line))
    {
        if (line == esc)
        {
			if (title == esc) break;
            if (topic != "") 
			{
                if (title.starts_with("Template:"))
                    Templates.emplace(title.from(9),
                        topic);
                else
                if (title.starts_with("Module:"))
                    Modules.emplace(title.from(7),
                        topic);
                else
					throw std::runtime_error
					("nonsense");

                title = esc;
				topic = "";
			}
        }
        else
        {
            if (title == "" or
				title == esc)
				title =  line; else
                topic += line + "\n";
        }
    }
}

std::map<str, const char8_t*> Symbols
{
	{ "AElig", u8"Æ" },
	{ "Aacute", u8"Á" },
	{ "Acirc", u8"Â" },
	{ "Agrave", u8"À" },
	{ "Alpha", u8"Α" },
	{ "Aring", u8"Å" },
	{ "Atilde", u8"Ã" },
	{ "Auml", u8"Ä" },
	{ "Beta", u8"Β" },
	{ "Ccedil", u8"Ç" },
	{ "Chi", u8"Χ" },
	{ "Dagger", u8"‡" },
	{ "Delta", u8"Δ" },
	{ "ETH", u8"Ð" },
	{ "Eacute", u8"É" },
	{ "Ecirc", u8"Ê" },
	{ "Egrave", u8"È" },
	{ "Epsilon", u8"Ε" },
	{ "Eta", u8"Η" },
	{ "Euml", u8"Ë" },
	{ "Gamma", u8"Γ" },
	{ "Iacute", u8"Í" },
	{ "Icirc", u8"Î" },
	{ "Igrave", u8"Ì" },
	{ "Iota", u8"Ι" },
	{ "Iuml", u8"Ï" },
	{ "Kappa", u8"Κ" },
	{ "Lambda", u8"Λ" },
	{ "Mu", u8"Μ" },
	{ "Ntilde", u8"Ñ" },
	{ "Nu", u8"Ν" },
	{ "OElig", u8"Œ" },
	{ "Oacute", u8"Ó" },
	{ "Ocirc", u8"Ô" },
	{ "Ograve", u8"Ò" },
	{ "Omega", u8"Ω" },
	{ "Omicron", u8"Ο" },
	{ "Oslash", u8"Ø" },
	{ "Otilde", u8"Õ" },
	{ "Ouml", u8"Ö" },
	{ "Phi", u8"Φ" },
	{ "Pi", u8"Π" },
	{ "Prime", u8"″" },
	{ "Psi", u8"Ψ" },
	{ "Rho", u8"Ρ" },
	{ "Scaron", u8"Š" },
	{ "Sigma", u8"Σ" },
	{ "THORN", u8"Þ" },
	{ "Tau", u8"Τ" },
	{ "Theta", u8"Θ" },
	{ "Uacute", u8"Ú" },
	{ "Ucirc", u8"Û" },
	{ "Ugrave", u8"Ù" },
	{ "Upsilon", u8"Υ" },
	{ "Uuml", u8"Ü" },
	{ "Xi", u8"Ξ" },
	{ "Yacute", u8"Ý" },
	{ "Yuml", u8"Ÿ" },
	{ "Zeta", u8"Ζ" },
	{ "aacute", u8"á" },
	{ "acirc", u8"â" },
	{ "acute", u8"´" },
	{ "aelig", u8"æ" },
	{ "agrave", u8"à" },
	{ "alefsym", u8"ℵ" },
	{ "alpha", u8"α" },
	{ "amp", u8"&" },
	{ "and", u8"∧" },
	{ "ang", u8"∠" },
	{ "apos", u8"'" },
	{ "aring", u8"å" },
	{ "asymp", u8"≈" },
	{ "atilde", u8"ã" },
	{ "auml", u8"ä" },
	{ "bdquo", u8"„" },
	{ "beta", u8"β" },
	{ "brvbar", u8"¦" },
	{ "bull", u8"•" },
	{ "cap", u8"∩" },
	{ "ccedil", u8"ç" },
	{ "cedil", u8"¸" },
	{ "cent", u8"¢" },
	{ "chi", u8"χ" },
	{ "circ", u8"ˆ" },
	{ "clubs", u8"♣" },
	{ "cong", u8"≅" },
	{ "copy", u8"©" },
	{ "crarr", u8"↵" },
	{ "cup", u8"∪" },
	{ "curren", u8"¤" },
	{ "dArr", u8"⇓" },
	{ "dagger", u8"†" },
	{ "darr", u8"↓" },
	{ "deg", u8"°" },
	{ "delta", u8"δ" },
	{ "diams", u8"♦" },
	{ "divide", u8"÷" },
	{ "eacute", u8"é" },
	{ "ecirc", u8"ê" },
	{ "egrave", u8"è" },
	{ "empty", u8"∅" },
	{ "emsp", u8"\u2003" }, // u8"\xE2\x80\x83" },
	{ "ensp", u8"\u2002" }, // u8"\xE2\x80\x82" },
	{ "epsilon", u8"ε" },
	{ "equiv", u8"≡" },
	{ "eta", u8"η" },
	{ "eth", u8"ð" },
	{ "euml", u8"ë" },
	{ "euro", u8"€" },
	{ "exist", u8"∃" },
	{ "fnof", u8"ƒ" },
	{ "forall", u8"∀" },
	{ "frac12", u8"½" },
	{ "frac14", u8"¼" },
	{ "frac34", u8"¾" },
	{ "frasl", u8"⁄" },
	{ "gamma", u8"γ" },
	{ "ge", u8"≥" },
	{ "gt", u8">" },
	{ "hArr", u8"⇔" },
	{ "harr", u8"↔" },
	{ "hearts", u8"♥" },
	{ "hellip", u8"…" },
	{ "iacute", u8"í" },
	{ "icirc", u8"î" },
	{ "iexcl", u8"¡" },
	{ "igrave", u8"ì" },
	{ "image", u8"ℑ" },
	{ "infin", u8"∞" },
	{ "int", u8"∫" },
	{ "iota", u8"ι" },
	{ "iquest", u8"¿" },
	{ "isin", u8"∈" },
	{ "iuml", u8"ï" },
	{ "kappa", u8"κ" },
	{ "lArr", u8"⇐" },
	{ "lambda", u8"λ" },
	{ "lang", u8"〈" },
	{ "laquo", u8"«" },
	{ "larr", u8"←" },
	{ "lceil", u8"⌈" },
	{ "ldquo", u8"“" },
	{ "le", u8"≤" },
	{ "lfloor", u8"⌊" },
	{ "lowast", u8"∗" },
	{ "loz", u8"◊" },
	{ "lrm", u8"\u200E" }, // u8"\xE2\x80\x8E" },
	{ "lsaquo", u8"‹" },
	{ "lsquo", u8"‘" },
	{ "lt", u8"<" },
	{ "macr", u8"¯" },
	{ "mdash", u8"—" },
	{ "micro", u8"µ" },
	{ "middot", u8"·" },
	{ "minus", u8"−" },
	{ "mu", u8"μ" },
	{ "nabla", u8"∇" },
	{ "nbsp", u8"\u00A0" }, // u8"\xC2\xA0" },
	{ "ndash", u8"–" },
	{ "ne", u8"≠" },
	{ "ni", u8"∋" },
	{ "not", u8"¬" },
	{ "notin", u8"∉" },
	{ "nsub", u8"⊄" },
	{ "ntilde", u8"ñ" },
	{ "nu", u8"ν" },
	{ "oacute", u8"ó" },
	{ "ocirc", u8"ô" },
	{ "oelig", u8"œ" },
	{ "ograve", u8"ò" },
	{ "oline", u8"‾" },
	{ "omega", u8"ω" },
	{ "omicron", u8"ο" },
	{ "oplus", u8"⊕" },
	{ "or", u8"∨" },
	{ "ordf", u8"ª" },
	{ "ordm", u8"º" },
	{ "oslash", u8"ø" },
	{ "otilde", u8"õ" },
	{ "otimes", u8"⊗" },
	{ "ouml", u8"ö" },
	{ "para", u8"¶" },
	{ "part", u8"∂" },
	{ "permil", u8"‰" },
	{ "perp", u8"⊥" },
	{ "phi", u8"φ" },
	{ "pi", u8"π" },
	{ "piv", u8"ϖ" },
	{ "plusmn", u8"±" },
	{ "pound", u8"£" },
	{ "prime", u8"′" },
	{ "prod", u8"∏" },
	{ "prop", u8"∝" },
	{ "psi", u8"ψ" },
	{ "quot", u8"\"" },
	{ "rArr", u8"⇒" },
	{ "radic", u8"√" },
	{ "rang", u8"〉" },
	{ "raquo", u8"»" },
	{ "rarr", u8"→" },
	{ "rceil", u8"⌉" },
	{ "rdquo", u8"”" },
	{ "real", u8"ℜ" },
	{ "reg", u8"®" },
	{ "rfloor", u8"⌋" },
	{ "rho", u8"ρ" },
	{ "rlm", u8"\u200F" }, // u8"\xE2\x80\x8F" },
	{ "rsaquo", u8"›" },
	{ "rsquo", u8"’" },
	{ "sbquo", u8"‚" },
	{ "scaron", u8"š" },
	{ "sdot", u8"⋅" },
	{ "sect", u8"§" },
	{ "shy", u8"\u00AD" }, // u8"\xC2\xAD" },
	{ "sigma", u8"σ" },
	{ "sigmaf", u8"ς" },
	{ "sim", u8"∼" },
	{ "spades", u8"♠" },
	{ "sub", u8"⊂" },
	{ "sube", u8"⊆" },
	{ "sum", u8"∑" },
	{ "sup1", u8"¹" },
	{ "sup2", u8"²" },
	{ "sup3", u8"³" },
	{ "sup", u8"⊃" },
	{ "supe", u8"⊇" },
	{ "szlig", u8"ß" },
	{ "tau", u8"τ" },
	{ "there4", u8"∴" },
	{ "theta", u8"θ" },
	{ "thetasym", u8"ϑ" },
	{ "thinsp", u8"\u2009" }, // u8"\xE2\x80\x89" },
	{ "thorn", u8"þ" },
	{ "tilde", u8"˜" },
	{ "times", u8"×" },
	{ "trade", u8"™" },
	{ "uArr", u8"⇑" },
	{ "uacute", u8"ú" },
	{ "uarr", u8"↑" },
	{ "ucirc", u8"û" },
	{ "ugrave", u8"ù" },
	{ "uml", u8"¨" },
	{ "upsih", u8"ϒ" },
	{ "upsilon", u8"υ" },
	{ "uuml", u8"ü" },
	{ "weierp", u8"℘" },
	{ "xi", u8"ξ" },
	{ "yacute", u8"ý" },
	{ "yen", u8"¥" },
	{ "yuml", u8"ÿ" },
	{ "zeta", u8"ζ" },
	{ "zwj", u8"\u200D" }, // u8"\xE2\x80\x8D" },
	{ "zwnj", u8"\u200C" }, // u8"\xE2\x80\x8C" }
};
