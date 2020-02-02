#pragma once
#include "3.h"
namespace pass3
{
    Pass <entry, entry> lex_items = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, UPDATING_REPORTS};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0) logout("lex_items", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                if (!lexical_items.contains(header)) continue;

                str cap =
                    esc   + "\n" +
                    title + "\n" +
                    esc   + "\n" + "\n" +
                    "==== " + header + " ==== " + forms + "\n";

                int complexity = 0;

                for (auto & line : content)
                    if (!line.starts_with ("# ")
                    ||   line.contains(str::one_of("<>")))
                        { complexity++; break; }

                if (complexity == 0) { result.report (cap + str(content) + "\n", "complexity 00"); continue; }

                for (auto & line : content)
                    if (!line.starts_with ("# ")
                    &&  !line.starts_with ("#: ")
                    ||   line.starts_with ("#: {")
                    ||   line.starts_with ("#: '''")
                    ||   line.contains(str::one_of("<>")))
                        { complexity++; break; }

                if (complexity == 1) { result.report (cap + str(content) + "\n", "complexity 01"); continue; }

                array<str> accepted; str state = "";

                for (auto & line : content)
                {
                    if (line.contains(str::one_of("<>"))) complexity = max(20, complexity);

                    if (line.starts_with ("# ")) { accepted += line; state = "def"; } else
                    if (line.starts_with ("#"))
                    {
                        str prefix = line.upto(15);
                        if (prefix.starts_with ("##")) { prefix.erase(0); complexity = max(40, complexity); }
                        if (prefix.starts_with ("# ")) { accepted += line; state = "def"; continue; }
                        prefix.replace_all("  ", "");
                        prefix.replace_all(" ", "");
                    
                        if (prefix.starts_with ("#:"    )) { if (state == "def") accepted += line; } else
                        if (prefix.starts_with ("#*:"   )) { if (state == "def") complexity = max(10, complexity); } else

                        if (prefix.starts_with ("#*'''"     )) { state = "date"; complexity = max(11, complexity); } else
                        if (prefix.starts_with ("#*ca.'''"  )) { state = "date"; complexity = max(12, complexity); } else
                        if (prefix.starts_with ("#*{{rfdate")) { state = "date"; complexity = max(13, complexity); } else

                        if (prefix == "#*{{QUOTE}}") { state = "quote"; complexity = max(30, complexity); } else
                        if (prefix == "#*{{RQ}}"   ) { state = "quote"; complexity = max(31, complexity); } else


                        complexity = 80;
                    }
                    else
                    {
                        complexity = 90;
                    }
                }

                str complexity_ = std::to_string(complexity);
                if (complexity_.size() == 1) complexity_ = "0" + complexity_;
                result.report (cap + str(content) + "\n=============\n" + str(accepted) + "\n",
                    "complexity "+ complexity_);

                content = accepted;
            }

            result.accept (entry {std::move(title), std::move(topic)});
        }
    };
}
