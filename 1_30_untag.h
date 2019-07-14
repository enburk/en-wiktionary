Pass <entry, entry> untag = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    array<str> forms { "noun", "verb", "adj", "conj", "decl" };

    for (auto && [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 80'000 == 0) print("untag   ", nn, " entries ", input.cargo, " cargo ");

        for (int e, b = 0; ; )
        {
            b = topic.find("&lt;!--", str::start_from(b)); if (b == str::nope){ break; }
            e = topic.find("--&gt;" , str::start_from(b)); if (e == str::nope){ break; }

            result.reject (topic.substr (b, e-b+6 ), "tag xml comments");
                        
            topic.erase (b, e-b+6);
        }

        for (int e, b = 0; ; )
        {
            b = topic.find("&lt;ref&gt;", str::start_from(b)); if (b == str::nope){ break; }
            e = topic.find("&lt;/ref&gt;" , str::start_from(b)); if (e == str::nope){ break; }

            result.reject (topic.substr (b, e-b+12 ), "tag ref");
                        
            topic.erase (b, e-b+12);
        }

        if (title.starts_with("Template:"))
        {
            str name = title.from(9);
            str kind = 
                name.starts_with ("User "    ) ? "User"      :  
                name.starts_with ("RQ:"      ) ? "RQ"        :  
                name.starts_with ("R:"       ) ? "R"         :  
                "";

            if (kind == "")
            if (name.size() >= 4 && name[3] == '-' ||
                name.size() >= 3 && name[2] == '-' )
            {
                if (auto ss = name.split_by(str::one_of{"- /"});
                    ss.size() >= 2 && ss[0] != "en" && forms.found(ss[1]))
                    kind = "xxx-" + ss[1];
            }

            if (kind == "") { templates [name] = topic;
            result.report (entry {std::move(title), std::move(topic)}, "templates", true); } else
            result.reject (entry {std::move(title), std::move(topic)}, "templates " + kind);
            continue;
        }

        result.accept (entry {std::move(title), std::move(topic)}, "", true);
    }
};
