#pragma once
#include "1.h"
namespace pass1
{
    Pass<pass0::entry, pass0::entry> modules = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        array<str> forms {
            "noun", "verb", "adj", "conj", "decl", "pron",
            "adjectives", "anagram", "convert", "translit"
        };

        for (auto && [title, topic] : input)
        {
            if (title.starts_with("Template:"))
            {
                str kind;
                str name = title.from(9);
                if((name.size() >= 4 and name[3] == '-') or
                   (name.size() >= 3 and name[2] == '-'))
                {
                    if (auto ss = name.split_by(str::one_of{"- /"});
                        ss.size() >= 2 and ss[0] != "en" and
                        forms.contains (ss[1]))
                        kind = "xxx-" + ss[1];
                }

                if (kind == "")
                    if (name.ends_with("/documentation"))
                        kind = "documentation";

                if (kind == "") { Templates [name] = topic;
                result.report(pass0::entry{std::move(title), std::move(topic)}, "templates", true); } else
                result.reject(pass0::entry{std::move(title), std::move(topic)}, "templates " + kind);
                continue;
            }

            if (title.starts_with("Module:"))
            {
                str kind;
                str name = title.from(7);
                if((name.size() >= 4 and name[3] == '-') or
                   (name.size() >= 3 and name[2] == '-'))
                {
                    if (auto ss = name.split_by(str::one_of{"- /"});
                        ss.size() >= 2 and ss[0] != "en" and
                        forms.contains (ss[1]))
                        kind = "xxx-" + ss[1];
                }

                if (kind == "")
                    if (name.ends_with("/documentation"))
                        kind = "documentation";

                if (kind == "") { Modules [name] = topic;
                result.report(pass0::entry{std::move(title), std::move(topic)}, "modules", true); } else
                result.reject(pass0::entry{std::move(title), std::move(topic)}, "modules " + kind);
                continue;
            }

            static bool once = false;
            if (not once) { once = true;
                print("modules ", Modules.size());
                print("templates ", Templates.size());
                // better disable sort for performance
            }

            output.push(pass0::entry{
                std::move(title),
                std::move(topic)});
        }

        if (not Modules.empty()) {
            print("total modules ", Modules.size());
            print("total templates ", Templates.size());
        }
    };
}


