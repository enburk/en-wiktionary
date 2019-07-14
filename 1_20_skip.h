Pass <entry, entry> skip = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    for (auto && [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 150'000 == 0) print("skip    ", nn, " entries ", input.cargo, " cargo ");

        bool accept = true; str report = "";

        if (title.starts_with("Unsupported titles/" )) { report = "Unsupported titles"; accept = false; } else

        if (title.starts_with("Template:"  )) { report = "meta Template"  ; accept = true;  } else
        if (title.starts_with("Wiktionary:")) { report = "meta Wiktionary"; accept = false; } else
        if (title.starts_with("Appendix:"  )) { report = "meta Appendix"  ; accept = false; } else
        if (title.starts_with("Category:"  )) { report = "meta Category"  ; accept = false; } else
        if (title.starts_with("Index:"     )) { report = "meta Index"     ; accept = false; } else
        if (title.found      (":"          )) { report = "meta"           ; accept = false; } else
        {
            bool latin = false; for (signed char c : title) if (c > ' ') { latin = true; break; }
            if (!latin) { report = "non-English titles"; accept = false; }
        }

        if (accept)
        if (topic.starts_with("#REDIRECT")
        ||  topic.starts_with("#redirect"))
        {
            int b = topic.find("[[", str::start_from(9)); if (b == str::nope){ result.report (entry {title, topic}, "redirect broken"); break; }
            int e = topic.find("]]", str::start_from(b)); if (e == str::nope){ result.report (entry {title, topic}, "redirect broken"); break; }

            str where = topic.substr (b+2, e-b-2);

            if (!title.starts_with("Template:")
            &&  !where.starts_with("Template:")
            &&  !where.starts_with("template:"))
            {
                 redirects [title] = where;
                 result.report (title + " => " + where, "redirects");
            }
            else
            if ( title.starts_with("Template:")
            &&  !where.starts_with("Template:")
            &&  !where.starts_with("template:"))
            {
                 redirects [title] = where;
                 result.report (title + " => " + where, "redirects templates broken");
            }
            else
            {
                 redirects_templates [title.from(9)] = where.from(9);
                 result.report (title.from(9) + " => " + where.from(9), "redirects templates");
            }
                        
            if(e+2 < topic.size())
            result.report (entry {title, topic.from (e+2)}, "redirect skip"); 
            result.report (entry {std::move(title), std::move(topic)}, "redirect topics");
            continue;
        }

        if (accept
        && !topic.found("English")
        && !topic.found("Translingual")
        && !title.starts_with("Template:")
        ) { report = "non-English topics"; accept = false; }

        accept ?
        result.accept (entry {std::move(title), std::move(topic)}, report, true):
        result.reject (entry {std::move(title), std::move(topic)}, report, true);
    }
};
