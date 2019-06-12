Pass <pass1::entry, entry> english = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    std::map<str,int> languages, sequals, headers;

    for (auto [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 10'000 == 0) print("english ", nn, " entries ", input.cargo, " cargo ");

        for (int e, b = 0; ; )
        {
            b = topic.find("&lt;ref&gt;", str::start_from(b)); if (b == str::nope){ break; }
            e = topic.find("&lt;/ref&gt;" , str::start_from(b)); if (e == str::nope){ break; }

            result.reject (topic.substr (b, e-b+12 ), "untag ref");
                        
            topic.erase (b, e-b+12);
        }

        array<str> ss = topic.split_by ("\n");

        if (topic.starts_with("#REDIRECT")
        ||  topic.starts_with("#redirect"))
        {
            for (str s : ss)
            {
                if (s.starts_with("#REDIRECT")
                ||  s.starts_with("#redirect"))
                {
                    s.erase(0,9); s.strip();
                    s.replace_all("[[", "");
                    s.replace_all("]]", "");
                    redirect [title] = s;
                    result.report (title + " => " + s, "redirect");
                }
                else result.report (s, "redirect skip");
            }
            result.report (entry {std::move(title), std::move(ss)}, "redirect topics");
            continue;
        }

        if (title.starts_with("Template:"))
        {
            str kind = 
                title.size() >= 9+4 && title[9+4-1] == '-' ? "xxx-" :
                title.size() >= 9+3 && title[9+3-1] == '-' && !title.starts_with("Template:en-") ? "xx-" : "";
            if (kind == "")
            result.accept (entry {std::move(title), std::move(ss)}, "templates", true);
            result.reject (entry {std::move(title), std::move(ss)}, "templates " + kind, true);
            continue;
        }

        std::map<str, array<str>> topics; str kind = "prelude";

        for (str s : ss)
        {
            if (s.found ("=="))
            {
                if (!s.starts_with ( "==" )) sequals [s]++; else
                {
                    int level  = s.find (str::one_not_of {"="});
                    str prefix = s.head (level);
                    str header = s;
                    header.triml ("= ");
                    header.trimr (" =");

                    if (header.found ("English"))
                    {
                        if (prefix != "=="            ) kind = "Probably English"; else
                        if (header == "English"       ) kind = header; else
                        if (header == "Old English"   ) kind = header; else
                        if (header == "Middle English") kind = header; else
                                                        kind = "Other English";
                    }
                    else if (prefix == "==" && header == "Translingual" ) header = kind = "English";
                    else if (prefix == "==") { kind = "Foreign"; languages [s]++; }

                    else headers [s]++;

                    s = prefix + " " + header;
                }
            }

            if (s != "== English") topics [kind] += s;
        }

        for (auto [kind, body] : topics)
            if (kind == "English")
                result.accept (entry {title, std::move(body)}); else
                result.reject (entry {title, std::move(body)}, kind);
    }

    for (auto [s, n] : languages) result.report (s + " " + std::to_string(n), "languages");
    for (auto [s, n] : sequals  ) result.report (s + " " + std::to_string(n), "sequals"  );
    for (auto [s, n] : headers  ) result.report (s + " " + std::to_string(n), "headers"  );
};


