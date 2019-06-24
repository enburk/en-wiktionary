Pass <entry, entry> untag = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    for (auto && [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 80'000 == 0) print("skip    ", nn, " entries ", input.cargo, " cargo ");

        for (int e, b = 0; ; )
        {
            b = topic.find("&lt;!--", str::start_from(b)); if (b == str::nope){ break; }
            e = topic.find("--&gt;" , str::start_from(b)); if (e == str::nope){ break; }

            result.reject (topic.substr (b, e-b+6 ), "xml comments");
                        
            topic.erase (b, e-b+6);
        }

        for (int e, b = 0; ; )
        {
            b = topic.find("&lt;ref&gt;", str::start_from(b)); if (b == str::nope){ break; }
            e = topic.find("&lt;/ref&gt;" , str::start_from(b)); if (e == str::nope){ break; }

            result.reject (topic.substr (b, e-b+12 ), "untag ref");
                        
            topic.erase (b, e-b+12);
        }

        if (title.starts_with("Template:"))
        {
            str name = title.from(9);
            str kind = 
                name.size() >= 4 && name[3] == '-' ? "xxx-"  :
                name.size() >= 3 && name[2] == '-' && !name.starts_with ("en-") ? "xx-" :

                name.starts_with ("unicode " ) ? "unicode"   :
                name.starts_with ("index/"   ) ? "index"     : 
                name.starts_with ("User "    ) ? "User"      :  

                name.starts_with ("quote-"   ) ? "quote-"    :  
                name.starts_with ("cite-"    ) ? "cite-"     :  
                name.starts_with ("RQ:"      ) ? "RQ"        :  
                name.starts_with ("R:"       ) ? "R"         :  

                name.starts_with ("Swadesh"  ) ? "Swadesh"   :  
                name.starts_with ("Icelandic") ? "Icelandic" :  

                "";

            if (kind == "") { templates [name] = topic;
            result.report (entry {std::move(title), std::move(topic)}, "templates", true); } else
            result.reject (entry {std::move(title), std::move(topic)}, "templates " + kind);
            continue;
        }

        result.accept (entry {std::move(title), std::move(topic)}, "", true);
    }
};
