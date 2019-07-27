Pass <pass1::entry, entry> english = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    std::map<str,int> languages, sequals, headers;

    for (auto && [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("english   ", nn, " entries ", input.cargo, " cargo ");

        std::unordered_map<str, array<str>> topics; str kind = "prelude";

        int n0 = 0;
        while (n0 < topic.size())
        {
            int n1 = topic.find('\n', str::start_from(n0)); if (n1 == str::nope) n1 = topic.size();
            str s = topic.substr(n0, n1-n0);
            n0 = n1 + 1; 

            if (s.found ("=="))
            {
                if (!s.starts_with ( "==" )) sequals [s]++; else
                {
                    int level  = s.find (str::one_not_of {"="});
                    str prefix = s.head (level);
                    str header = s;
                    header.triml ("= ");
                    header.trimr (" =");

                    if (header.found ("English"))
                    {
                        if (prefix != "=="            ) kind = "Probably English"; else
                        if (header == "English"       ) kind = header; else
                        if (header == "Old English"   ) kind = header; else
                        if (header == "Middle English") kind = header; else
                                                        kind = "Other English";
                    }
                    else if (prefix == "==" && header == "Translingual" ) header = kind = "English";
                    else if (prefix == "==") { kind = "Foreign"; languages [s]++; }

                    else headers [s]++;

                    s = prefix + " " + header;
                }
            }

            if (!result.on && kind != "English") continue;

            if (s != "== English") topics [kind] += std::move(s);
        }

        for (auto [kind, body] : topics)
            if (kind == "English")
                result.accept (entry {title, std::move(body)}); else
                result.reject (entry {title, std::move(body)}, kind);
    }

    for (auto [s, n] : languages) result.report (s + " " + std::to_string(n), "languages");
    for (auto [s, n] : sequals  ) result.report (s + " " + std::to_string(n), "sequals"  );
    for (auto [s, n] : headers  ) result.report (s + " " + std::to_string(n), "headers"  );
};


