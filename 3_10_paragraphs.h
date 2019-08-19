Pass <pass2::entry, entry> paragraphs = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    for (auto && [title, topic] : input)
    {
        entry e; e.title = title; paragraph current;

        topic += "==== "; // ending

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
