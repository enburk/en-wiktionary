#pragma once
#include "5.h"
namespace pass5
{
    Pass <entry, entry> altforms = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
            for (auto & [header, forms, content] : topic)
            {
                if (header != "alternative forms")
                    continue;

                str report = "alt ";
                str & f = forms;
                str & t = title;

                auto alt = [&] (str form)
                {
                    std::lock_guard lock{lexforms_mutex};

                    bool found = false; str list;
                    for (auto & [k, ack, w] : lexforms[t]) {
                        bool aha = k.contains("Alternative");
                        if (aha and w == form) { found = true; ack = "+"; }
                        if (aha) list += "[" + w + "]";
                    }

                    if (not found and form != t and list != "") result.report(
                        t + " | " + f + " => " + form + " <> " + list,
                        "- conflicts");

                    if (not found and form != t and list == "") result.report(
                        t + " | " + f + " => " + form,
                        "- unacknowledged");

                    if (not found and form != t)
                        lexforms[t] += lexform{"Alternative form of", "?", form};

                    return "'''" + form + "'''";
                };

                int complexity = 0;

                for (str s : content)
                {
                    int kind = 10;
                    if (s.contains_only(str::one_of(alnum))) kind = 0; else
                    if (s.contains_only(str::one_of(alnum+" -"))) kind = 1; else
                    if (s.contains_only(str::one_of(alnum+" -.!?"))) kind = 2; else
                    if (s.contains_only(str::one_of(alnum+" -.!?'"))) kind = 3; else
                    if (s.contains_only(str::one_of(ALNUM))
                    && !s.contains(str::one_of("/(){}[]")))
                    {
                        if (!title.contains(","))
                        {
                            kind = s.contains(",") ? 5 : 4;
                            auto list = s.split_by(",");
                            for (str i : list) { i.strip(); alt(i); }
                        }
                        else
                        {
                            int n = 0; for (char c : title) if (c == ',') n++;
                            int m = 0; for (char c : s    ) if (c == ',') m++;
                            if (m == n) { kind = 6; alt(s); }
                        }
                    }

                    if (kind < 4) alt(s);
                            
                    complexity = max(kind, complexity);
                }

                report += complexity < 10 ? std::to_string(complexity) : "quest";

                result.report ("===== " + title + " ===== \n" + str(content) + "\n", report);
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
