Pass <entry, entry> cleanup = [](auto & input, auto & output)
{
    Result result {__FILE__, output, true};

    for (auto [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 10'000 == 0) print("cleanup ", nn, " entries ", input.cargo, " cargo ");

        result.accept (entry {std::move(title), std::move(topic)});


    }
};
