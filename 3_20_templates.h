Pass <entry, entry> templating = [](auto & input, auto & output)
{
    Result result {__FILE__, output, true};

    for (auto && [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("templates ", nn, " entries ", input.cargo, " cargo ");

        bracketer b;
        b.proceed_template = [&](str s) { return proceed_template (title, "/", s, result); };
        b.proceed(topic);

        topic = std::move(b.output);

        result.accept (entry {std::move(title), std::move(topic)});
    }
};
