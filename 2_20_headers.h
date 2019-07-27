Pass <entry, entry> headers = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    std::unordered_set<str> accepted;
    for (auto h : lexical_items) accepted.emplace(h);
    for (auto h : related_items) accepted.emplace(h);
    for (auto h : lexical_notes) accepted.emplace(h);

    std::unordered_set<str> rejected
    {
        "conjugation",
        "translations", "descendants",
        "references", "further reading", "quotations", "citations", "external links", "notes",
        "anagrams", "gallery", "statistics",
    };

    std::map<str,int> other_headers;

    for (auto [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("headers   ", nn, " entries ", input.cargo, " cargo ");

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

                if (accepted.find (header) != accepted.end()) kind = "accepted"; else
                if (rejected.find (header) != rejected.end()) kind = header; else

                { kind = "zzz other headers"; other_headers [header]++; }

                s = "==== " + header;
            }

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


