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

