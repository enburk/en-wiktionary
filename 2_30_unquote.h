Pass <entry, entry> unquote = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    array<str> rejected
    {
        "{{quote-", "{{cite-"
    };

    for (auto [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 10'000 == 0) print("unquote ", nn, " entries ", input.cargo, " cargo ");

        if (title.starts_with("Template:quote-")) { result.reject (entry {std::move(title), std::move(topic)}, "Template quote-"); continue; }
        if (title.starts_with("Template:cite-" )) { result.reject (entry {std::move(title), std::move(topic)}, "Template cite-" ); continue; }
        if (title.starts_with("Template:RQ:"   )) { result.reject (entry {std::move(title), std::move(topic)}, "Template RQ"    ); continue; }
        if (title.starts_with("Template:R:"    )) { result.reject (entry {std::move(title), std::move(topic)}, "Template R"     ); continue; }
        if (title.starts_with("Template:"      )) { result.accept (entry {std::move(title), std::move(topic)}, "Template"       ); continue; }

        std::map<str, array<str>> topics; str kind = "accepted"; int braces = 0;

        for (str s : topic)
        {
            if (kind != "accepted" && braces == 0) kind = "accepted";

            if (kind != "accepted" && s.starts_with ("#"))
            {
                topics ["broken braces"] += topics [kind].back();
                topics [kind].pop_back();
                kind = "accepted";
                braces = 0;
            }

            if (kind == "accepted" && s.starts_with ("#"))
            {
                for (str r : rejected)
                {
                    if (s.starts_with ("#* " + r) ||
                        s.starts_with ("##* " + r))
                    {
                        kind = r; break;
                    }
                }
            }

            if (kind != "accepted")
            {
                for (int n=0; n<s.size()-1; n++)
                {
                    if (s[n] == '{' && s[n+1] == '{') { braces++; n++; continue; }
                    if (s[n] == '}' && s[n+1] == '}') { braces--; n++;
                        if (braces == 0) {
                            str remainder = s.from(n+1);
                            remainder.strip(); if (remainder != "")
                            topics ["remainder"] += remainder;
                            s.truncate(n+1);
                            break;
                        }
                    }
                }
            }

            topics [kind] += s;
        }

        topic = std::move(topics ["accepted"]); kind = "accepted";

        for (str s : topic)
        {
            if (s.starts_with ("#"))
            {
                if (s.starts_with ("#* '''")) kind = "#*"; else
                if (s.starts_with ("##* '''")) kind = "##*"; else
                if (s.starts_with (kind + ":")) kind = kind; else
                kind = "accepted";
            }
            else kind = "accepted";

            topics [kind] += s;
        }

        for (auto [kind, body] : topics)
            if (kind == "accepted")
                result.accept (entry {title, std::move(body)}); else { str report = kind; report.replace_all("*", "'''");
                result.reject (entry {title, std::move(body)}, report); }
    }
};
