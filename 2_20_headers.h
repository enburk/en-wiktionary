Pass <entry, entry> headers = [](auto & input, auto & output)
{
    Result result {__FILE__, output, true};

    array<str> accepted 
    {
        "noun", "pronoun", "proper noun", "verb", "adjective", "adverb", "numeral", "number", "article", "particle", 
        "preposition", "postposition", "determiner", "conjunction", "interjection",
        "abbreviation", "initialism", "contraction", "acronym", 
        "letter", "symbol", "punctuation mark", "diacritical mark",
        "suffix", "prefix", "infix", "affix", "interfix",
        "phrase", "prepositional phrase", "proverb", 
        "synonyms", "antonyms", "hypernyms", "hyponyms", "meronyms", "holonyms", "troponyms",
        "alternative forms", "abbreviations",
        "derived terms", "related terms", "coordinate terms",
        "pronunciation", "etymology",
        "usage notes", "see also", "trivia",
        "END"
    };

    array<str> rejected
    {
        "conjugation",
        "translations", "descendants",
        "references", "further reading", "quotations", "citations", "external links", "notes",
        "anagrams", "gallery", "statistics",
        "END"
    };

    std::sort(accepted.begin(), accepted.end());
    std::sort(rejected.begin(), rejected.end());

    std::map<str,int> other_headers;

    for (auto [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 10'000 == 0) print("headers ", nn, " entries ", input.cargo, " cargo ");

        if (title.starts_with("Template:")) { output.push (entry {std::move(title), std::move(topic)}); continue; }

        std::map<str, array<str>> topics; str kind = "prelude";

        for (str s : topic)
        {
            if (s.starts_with ("=="))
            {
                str prefix, header; s.split_by (" ", prefix, header);
                
                header = header.lowercased();

                if (header.starts_with ("noun "         )) header = "noun";
                if (header.starts_with ("etymology "    )) header = "etymology";
                if (header.starts_with ("pronunciation ")) header = "pronunciation";

                if (accepted.binary_found (header)) kind = "accepted"; else
                if (rejected.binary_found (header)) kind = header; else

                { kind = "zzz other headers"; other_headers [header]++; }

                s = "== " + header;
            }

            if (s.starts_with("#REDIRECT")
            ||  s.starts_with("#redirect"))
            topics ["accepted"] += s; else

            if (kind == "prelude" && s.starts_with ("{{wiki"  )) topics ["prelude wiki"] += s; else
            if (kind == "prelude" && s.starts_with ("[[File:" )) topics ["prelude file"] += s; else
            if (kind == "prelude" && s.starts_with ("[[Image:")) topics ["prelude file"] += s; else

            topics [kind] += s;
        }

        for (auto [kind, body] : topics)
            if (kind == "accepted")
                result.accept (entry {title, std::move(body)}); else
                result.reject (entry {title, std::move(body)}, kind);
    }

    for (auto [s, n] : other_headers) result.report (s + " : " + std::to_string(n), "zzz other headers");
};

