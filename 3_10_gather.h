Pass <pass2::entry, entry> gather = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    std::vector<std::pair<str,str>> content; content.reserve (1024*1024);

    auto start = std::chrono::high_resolution_clock::now ();

    for (auto && [title, topic_] : input)
    {
        static int64_t nn = 0; if (++nn % 40'000 == 0) print("gather  ", nn, " entries ", input.cargo, " cargo ");

        str topic; for (auto s : topic_) {
            topic += s;
            topic += '\n';
        }
        if (topic != "") topic.pop_back();

        content.emplace_back (std::move(title), std::move(topic));
    }

    auto stop  = std::chrono::high_resolution_clock::now ();

    auto duration = stop - start;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration); duration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration); duration -= seconds;

    std::cout << std::endl << "total: "
    << minutes.count ()  << " minutes "
    << seconds.count ()  << " seconds " << std::endl; 

    print ("");
    print (content.size(), " entries, gathered them all, start to seed...");
    print ("");

    for (auto && [title, topic] : content)
    {
        result.accept (entry {std::move(title), std::move(topic)});
    }
};
