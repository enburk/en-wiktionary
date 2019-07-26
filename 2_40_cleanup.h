Pass <entry, entry> cleanup = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    array<str> related { "derived terms", "related terms", "coordinate terms" };

    for (auto [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("cleanup ", nn, " entries ", input.cargo, " cargo ");

        std::map<str, array<str>> topics;

        str header; array<str> paragraph;

        topic += "==== ";

        for (str & s : topic)
        {
            if (s == "----") continue;

            if (s.starts_with ("==== "))
            {
                str kind = "accepted";

                if (paragraph.size () == 00) kind = "empty paragraph"; else
                if (paragraph.size () >= 50 && related.found(header)) kind = "related 50+"; else
            //  if (paragraph.size () >= 40 && related.found(header)) kind = "related 40+"; else
            //  if (paragraph.size () >= 30 && related.found(header)) kind = "related 30+"; else
                {}

                for (auto & ss : paragraph) topics [kind] += std::move(ss);

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
