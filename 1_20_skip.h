Pass <entry, entry> skip = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false}; // don't generate reports by default - they are huge

    for (auto && [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 20'000 == 0) print("skip    ", nn, " entries ", input.cargo, " cargo ");

        bool accept = true; str report = "";

        if (title.starts_with("Template:"  )) { report = "meta Template"  ; accept = true;  } else
        if (title.starts_with("Wiktionary:")) { report = "meta Wiktionary"; accept = false; } else
        if (title.starts_with("Appendix:"  )) { report = "meta Appendix"  ; accept = false; } else
        if (title.starts_with("Category:"  )) { report = "meta Category"  ; accept = false; } else
        if (title.starts_with("Index:"     )) { report = "meta Index"     ; accept = false; } else
        if (title.found      (":"          )) { report = "meta"           ; accept = false; } else
        {
            bool latin = false; for (signed char c : title) if (c > 0) { latin = true; break; }
            if (!latin) { report = "non-English titles"; accept = false; }
        }

        if (accept
        && !title.starts_with("Template:")
        && !topic.found("English")
        && !topic.found("Translingual")
        && !topic.found("#redirect")
        && !topic.found("#REDIRECT")) { report = "non-English topics"; accept = false; }

        if (accept)
        {
            for (int e, b = 0; ; )
            {
                b = topic.find("&lt;!--", str::start_from(b)); if (b == str::nope){ break; }
                e = topic.find("--&gt;" , str::start_from(b)); if (e == str::nope){ break; }

                result.reject (topic.substr (b, e-b+6 ), "xml comments");
                        
                topic.erase (b, e-b+6);
            }
        }

        accept ?
        result.accept (entry {std::move(title), std::move(topic)}, report, true):
        result.reject (entry {std::move(title), std::move(topic)}, report, true);
    }
};
