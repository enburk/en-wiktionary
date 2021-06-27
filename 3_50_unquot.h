#pragma once
#include "3.h"
namespace pass3
{
    Pass <entry, entry> unquot = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("unquot", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                str cap =
                    esc   + "\n" +
                    title + "\n" +
                    esc   + "\n" + "\n" +
                    "==== " + header + " ==== " + forms + "\n";

                array<str> accepted;
                array<str> lines = content.split_by("\n");

                for (str & s : lines)
                {
                    s.strip();

                    if (s == "----") continue;

                    if (!lexical_notes.contains(header) and
                        s == "") continue;

                    if (s.starts_with("[[Category:") and
                        s.ends_with  ("]]") and not
                        s.from(2).contains('[')) {
                        result.reject(std::move(s),
                            "category"); continue; }

                    if (s.starts_with("{{") && s.ends_with("}}"))
                    {
                        str ss = s.from(2); if (!ss.contains("{{"))
                        {
                            if (ss.starts_with("C|"              )) { result.reject (s, "{{c}}"); continue; }
                            if (ss.starts_with("c|"              )) { result.reject (s, "{{c}}"); continue; }
                            if (ss.starts_with("taxoninfl"       )) { result.reject (s, "{{taxoninfl}}"); continue; }
                            if (ss.starts_with("Webster 1913"    )) { result.reject (s, "{{webster}}"); continue; }
                            if (ss.starts_with("Wikipedia"       )) { result.reject (s, "{{wikipedia}}"); continue; }
                            if (ss.starts_with("wikipedia"       )) { result.reject (s, "{{wikipedia}}"); continue; }
                            if (ss.starts_with("morse code audio")) { result.reject (s, "{{z}}"); continue; }
                            if (ss.starts_with("mul-morse "      )) { result.reject (s, "{{z}}"); continue; }
                            if (ss.starts_with("suffixsee"       )) { result.reject (s, "{{z}}"); continue; }
                            if (ss.starts_with("topics|"         )) { result.reject (s, "{{z}}"); continue; }
                            if (ss.starts_with("ccTLD"           )) { result.reject (s, "{{z}}"); continue; }
                        }
                    }

                    accepted += s;
                }

                content = str(accepted); // continue follows

                if (!lexical_items.contains(header)) continue;

                lines = std::move(accepted);

                int complexity = 0;

                for (auto & line : lines)
                    if (not line.starts_with ("# ")
                        or  line.contains(str::one_of("<>")))
                        { complexity++; break; }

                if (complexity == 0) {
                    result.report (cap + content + "\n", "complexity 00 ");
                    continue;
                }

                for (auto & line : lines)
                    if((!line.starts_with ("# ")
                    &&  !line.starts_with ("#: "))
                    ||   line.starts_with ("#: {")
                    ||   line.starts_with ("#: '''")
                    ||   line.contains(str::one_of("<>")))
                        { complexity++; break; }

                if (complexity == 1) {
                    result.report (cap + content + "\n", "complexity 01");
                    continue;
                }

                auto ascii = [](char c){ return
                    (c >= 'A' and c <= 'Z') or
                    (c >= 'a' and c <= 'z'); };

                for (auto & line : lines)
                    if (line.starts_with("#{{") or
                       (line.starts_with("#[[") and line.size() >= 4 and ascii(line[3])) or
                       (line.starts_with("#''") and line.size() >= 4 and ascii(line[3])) or
                       (line.starts_with("#"  ) and line.size() >= 2 and ascii(line[1]))) {
                        line.insert(1," ");
                        complexity = 70; }

                str state = "";

                auto complex = [&complexity](int n){ complexity = max(n, complexity); };

                for (auto & line : lines)
                {
                    if (line.contains(str::one_of("<>"))) complex(20);

                    if (line.starts_with ("# ")) { accepted += line; state = "def"; } else
                    if (line.starts_with ("#"))
                    {
                        str l = line.upto(15);
                        if (l.starts_with ("##")) { l.erase(0); complex(40); }
                        if (l.starts_with ("# ")) { accepted += line; state = "def"; continue; }

                        l.replace_all("  ", "");
                        l.replace_all(" ", "");
                    
                        if (l.starts_with ("#:" )) { if (state == "def") accepted += line; } else
                        if (l.starts_with ("#*:")) { if (state == "def") complex(10); } else

                        if (l.starts_with ("#*'''"      )) { state = "date"; complex(11); } else
                        if (l.starts_with ("#*ca.'''"   )) { state = "date"; complex(12); } else
                        if (l.starts_with ("#*{{rfdate" )) { state = "date"; complex(13); } else
                        if (l.starts_with ("#*''[http:]")) { state = "date"; complex(14); } else
                        if (l.starts_with ("#:''[http:]")) { state = "date"; complex(14); } else

                        if (l == "#*{{QUOTE}}") { state = "quote"; complex(30); } else
                        if (l == "#*{{RQ}}"   ) { state = "quote"; complex(31); } else

                        complexity = 80;
                    }
                    else
                    {
                        complexity = 90; result.report(line, "complexity 90 (lines)");
                    }
                }

                if (accepted.empty()) complexity = 99;

                str complexity_ = std::to_string(complexity);
                if (complexity_.size() == 1)
                    complexity_ = "0" +
                    complexity_;

                result.report(
                    cap + content + "\n" +
                    "================\n" +
                    str(accepted) + "\n",
                    "complexity " +
                     complexity_);

                content = str(accepted);
            }

            result.accept(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
