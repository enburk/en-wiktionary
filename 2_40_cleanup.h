Pass <entry, entry> cleanup = [](auto & input, auto & output)
{
    Result result {__FILE__, output, true};

    for (auto [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 10'000 == 0) print("cleanup ", nn, " entries ", input.cargo, " cargo ");

        if (title.starts_with("Template:Swadesh"  )) { result.reject (entry {std::move(title), std::move(topic)}, "Template Swadesh"); continue; }
        if (title.starts_with("Template:Icelandic")) { result.reject (entry {std::move(title), std::move(topic)}, "Template Icelandic"); continue; }
        if (title.starts_with("Template:"         )) { result.accept (entry {std::move(title), std::move(topic)}, "Template"   ); continue; }

        result.accept (entry {std::move(title), std::move(topic)});


    }
};
