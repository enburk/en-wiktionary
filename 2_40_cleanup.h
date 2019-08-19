Pass <entry, entry> cleanup = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    for (auto [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("cleanup   ", nn, " entries ", input.cargo, " cargo ");

        std::map<str, array<str>> topics;

        str header; array<str> paragraph;

        topic += "==== "; // ending

        for (str & s : topic)
        {
            if (s == "----") continue;

            if (s.starts_with ("==== "))
            {
                str kind = "accepted";

                if (related_items.find(header) != related_items.end()) {
                    if (paragraph.size () >= 50) kind = "related items 50+"; else
                //  if (paragraph.size () >= 40) kind = "related items 40+"; else
                //  if (paragraph.size () >= 30) kind = "related items 30+"; else
                    {}
                }
                if (lexical_notes.find(header) != lexical_notes.end()) {
                    if (paragraph.size () >= 50) kind = "lexical notes 50+"; else
                    if (paragraph.size () >= 40) kind = "lexical notes 40+"; else
                //  if (paragraph.size () >= 30) kind = "lexical notes 30+"; else
                    {}
                }

                for (auto & line : paragraph) topics [kind] += std::move(line);

                paragraph.clear();
                header = s.from(5);
            }

            paragraph += std::move(s);
        }

        for (auto [kind, body] : topics)
            if (kind == "accepted")
                result.accept (entry {title, std::move(body)}); else
                result.reject (entry {title, std::move(body)}, kind);
    }
};
