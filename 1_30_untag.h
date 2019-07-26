Pass <entry, entry> untag = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    array<str> forms { "noun", "verb", "adj", "conj", "decl" };

    auto erase_all = [&result](str & title, str & topic, str from, str to, str report)
    {
        for (int e, b = 0; ; )
        {
            b = topic.find(from, str::start_from(b)); if (b == str::nope){ break; }
            e = topic.find(to  , str::start_from(b)); if (e == str::nope){

                report += b + from.size() == topic.size() ? " final" : " broken";

                result.reject("\n#################################################################\n"
                    + title + "\n#################################################################\n"
                    + topic + "\n=================================================================\n"
                    + topic.from (b), report); break;
            }

            result.reject (topic.substr (b, e-b+to.size()), report);
                        
            topic.erase (b, e-b+to.size());
        }
    };

    for (auto && [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("untag   ", nn, " entries ", input.cargo, " cargo ");

        erase_all (title, topic, "&lt;!--", "--&gt;", "tag xml comments");

        erase_all (title, topic, "&lt;ref&gt;", "&lt;/ref&gt;", "tag ref");

        erase_all (title, topic, "&lt;noinclude&gt;", "&lt;/noinclude&gt;", "tag noinclude");

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

            if (kind == "") if (name.ends_with("/documentation")) kind = "documentation";

            if (kind == "") if (topic.found("====")) kind = "====";
            if (kind == "") if (topic.found("===")) kind = "===";
            if (kind == "") if (topic.found("==")) kind = "==";

            if (kind == "") { templates [name] = topic;
            result.report (entry {std::move(title), std::move(topic)}, "templates", true); } else
            result.reject (entry {std::move(title), std::move(topic)}, "templates " + kind);
            continue;
        }

        result.accept (entry {std::move(title), std::move(topic)}, "", true);
    }
};
