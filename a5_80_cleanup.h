#pragma once
#include "1.h"
namespace pass1
{
    Pass <entry, entry> cleanup = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, UPDATING_REPORTS};

        for (auto [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("cleanup  ", nn, input.cargo);

            std::map<str, array<str>> topics;

            str header; array<str> paragraph;

            topic += "==== "; // ending

            for (str & s : topic)
            {
                if (s == "----") continue;

                if (s.starts_with("[[Category:") && s.ends_with("]]") && !s.from(2).contains('['))
                { result.reject (std::move(s), "category"); continue; }

                if (s.starts_with("{{") && s.ends_with("}}"))
                {
                    str ss = s.from(2); if (!ss.contains("{{"))
                    {
                        if (ss.starts_with("morse code audio")) { result.reject (s, "{{morse}}"); continue; }
                        if (ss.starts_with("mul-morse "      )) { result.reject (s, "{{morse}}"); continue; }
                        if (ss.starts_with("suffixsee"       )) { result.reject (s, "{{suffixsee}}"); continue; }
                        if (ss.starts_with("ccTLD"           )) { result.reject (s, "{{ccTLD}}"); continue; }
                        if (ss.starts_with("taxoninfl"       )) { result.reject (s, "{{taxoninfl}}"); continue; }
                        if (ss.starts_with("Webster 1913"    )) { result.reject (s, "{{Webster}}"); continue; }
                        if (ss.starts_with("Wikipedia"       )) { result.reject (s, "{{wikipedia}}"); continue; }
                        if (ss.starts_with("wikipedia"       )) { result.reject (s, "{{wikipedia}}"); continue; }
                        if (ss.starts_with("topics|"         )) { result.reject (s, "{{topics}}"); continue; }
                        if (ss.starts_with("C|"              )) { result.reject (s, "{{c}}"); continue; }
                        if (ss.starts_with("c|"              )) { result.reject (s, "{{c}}"); continue; }
                    }
                }

                if (s.starts_with ("==== "))
                {
                    str kind = "accepted";

                    if (related_items.find(header) != related_items.end()) {
                        if (paragraph.size () >= 50) kind = "related items 50+"; else
                    //  if (paragraph.size () >= 40) kind = "related items 40+"; else
                    //  if (paragraph.size () >= 30) kind = "related items 30+"; else
                        {}
                    }
                    if (lexical_notes.find(header) != lexical_notes.end()) {
                        if (paragraph.size () >= 50) kind = "lexical notes 50+"; else
                        if (paragraph.size () >= 40) kind = "lexical notes 40+"; else
                    //  if (paragraph.size () >= 30) kind = "lexical notes 30+"; else
                        {}
                    }

                    for (auto & line : paragraph) topics [kind] += std::move(line);

                    paragraph.clear();
                    header = s.from(5);
                }

                paragraph += std::move(s);
            }

            for (auto [kind, body] : topics)
                if (kind == "accepted")
                    result.accept (entry {title, std::move(body)}); else
                    result.reject (entry {title, std::move(body)}, kind);
        }
    };
}