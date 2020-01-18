#pragma once
#include "1.h"
namespace pass1
{
    Pass<pass0::entry, entry> lineup = [](auto & input, auto & output)
    {
        for (auto && [title, topic] : input)
        {
            output.push(std::move(entry{std::move(title), std::move(topic.split())}));
        }
    };

    Pass<entry, entry> english = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, false};

        std::map<str,int> languages, headers, equalequals;

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("english", nn, input.cargo);

            std::unordered_map<str, array<str>> topics; str kind = "prelude";

            for (str & s : topic)
            {
                if (s.contains ("=="))
                {
                    if (!s.starts_with ( "==" )) equalequals [s]++; else
                    {
                        int level  = s.find (str::one_not_of {"="}).offset;
                        str prefix = s.upto (level);
                        str header = s;
                        header.triml ("= ");
                        header.trimr (" =");

                        if (header.contains ("English"))
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

                if (!result.on && kind != "English") continue;

                if (s != "== English") topics [kind] += std::move(s);
            }

            for (auto && [kind, body] : topics)
                if (kind == "English")
                    result.accept (entry {title, std::move(body)}); else
                    result.reject (entry {title, std::move(body)}, kind);
        }

        for (auto [s, n] : languages  ) result.report (s + " " + std::to_string(n), "languages");
        for (auto [s, n] : equalequals) result.report (s + " " + std::to_string(n), "contains ==");
        for (auto [s, n] : headers    ) result.report (s + " " + std::to_string(n), "headers");
    };
}


