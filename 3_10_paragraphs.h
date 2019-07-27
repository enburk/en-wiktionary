Pass <pass2::entry, entry> paragraphs = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    for (auto [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("paragraph ", nn, " entries ", input.cargo, " cargo ");

        entry e; e.title = title; paragraph current;

        topic += "==== "; // happy ending

        for (str & s : topic)
        {
            if (s.starts_with ("==== "))
            {
                current.strip(); if (!current.empty()) e.topic += std::move(current);
                current = paragraph{};
                current.header = s.from(5);
                continue;
            }

            current.content += std::move(s);
            current.content += "\n";
        }

        result.accept (std::move(e));
    }
};
