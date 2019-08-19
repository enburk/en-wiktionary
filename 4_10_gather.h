// gathering essential before processing templates

Pass <pass2::entry, entry> gather = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    std::vector<pass2::entry> content; content.reserve (1024*1024);

    auto start = std::chrono::high_resolution_clock::now ();

    for (auto && entry : input) content.emplace_back (std::move(entry));

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

Pass <entry, entry> brackets = [](auto & input, auto & output)
{
    Result result {__FILE__, output, true};

    for (auto && [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("brackets  ", nn, " entries ", input.cargo, " cargo ");

        for (auto & [header, content] : topic)
        {
            bracketer b;
            b.proceed_sbrakets  = [&] (str s) { s =   "[" + s + "]";   result.report(s, "["  ); return s; };
            b.proceed_qbrakets  = [&] (str s) { s =   "{" + s + "}";   result.report(s, "{"  ); return s; };
            b.proceed_parameter = [&] (str s) { s = "{{{" + s + "}}}"; result.report(s, "{{{"); return s; };
            b.proceed_template  = [&] (str s) { return proceed_template (title, header, s, result); };
            b.proceed_link      = [&] (str s) { return proceed_link     (title, header, s, result); };
            b.proceed(content);
            
            if (b.report.size() > 0)
            {
                entry report;
                report.title = title;
                report.topic += paragraph {header, content};
                report.topic.back().content += "\n=================================================================\n";
                report.topic.back().content += str(b.report);
                result.report(report, "broken brackets");
            }
        
            content = std::move(b.output);
        }

        result.accept (entry {std::move(title), std::move(topic)});
    }
};
