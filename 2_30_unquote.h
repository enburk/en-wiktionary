Pass <entry, entry> unquote = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    array<str> rejected
    {
        "{{quote-", "{{cite-"
    };

    for (auto [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("unquote ", nn, " entries ", input.cargo, " cargo ");

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
                str prefix = s.head(15);
                prefix.replace_all("##", "#");
                prefix.replace_all("  ", "");
                prefix.replace_all(" ", "");

                for (str r : rejected)
                {
                    if (prefix.starts_with ("#*" + r) ||
                        prefix.starts_with ("#:" + r))
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
                str prefix = s.head(15);
                prefix.replace_all("##", "#");
                prefix.replace_all("  ", "");
                prefix.replace_all(" ", "");

                if (prefix.starts_with ("#*:"       )) kind = kind; else
                if (prefix.starts_with ("#*'''"     )) kind = "'''"; else
                if (prefix.starts_with ("#*ca.'''"  )) kind = "ca.'''"; else
                if (prefix.starts_with ("#*{{rfdate")) kind = "{{rfdate"; else

                kind = "accepted";
            }
            else kind = "accepted";

            topics [kind] += s;
        }

        for (auto [kind, body] : topics)
            if (kind == "accepted")
                result.accept (entry {title, std::move(body)}); else
                result.reject (entry {title, std::move(body)}, kind);
    }
};