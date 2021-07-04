#pragma once
#include "5.h"
namespace pass5
{
    Pass <entry, entry> lex_notes = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0) logout("lex_notes", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                if (!lexical_notes.contains(header)) continue;

                str cap =
                    esc   + "\n" +
                    title + "\n" +
                    esc   + "\n" + "\n" +
                    "==== " + header + " ==== " + forms + "\n";

                int complexity = 0;

                for (str & line : content)
                {
                    line.replace_all("[[", "");
                    line.replace_all("]]", "");

                    if (header == "pronunciation")
                    {
                        line.replace_all("): , /", "): /"); // {{enPR}}, {{IPA}}
                        line.replace_all( ") , /",  ") /"); // {{enPR}}, {{IPA}}
                        line.replace_all( "* , /",  "* /"); // {{enPR}}, {{IPA}}

                        line.strip();
                        while (line.starts_with("*")) {
                            line.erase(0);
                            line.strip(); }
                    }

                    if (!line.contains_only(str::one_of(alnum))) complexity = max(0+1, complexity);
                    if (!line.contains_only(str::one_of(ALnum))) complexity = max(2+1, complexity);
                    if (!line.contains_only(str::one_of(ALNum))) complexity = max(3+1, complexity);
                    if (!line.contains_only(str::one_of(ALNUM))) complexity = max(5+1, complexity);

                    if (0 < line.replace_all("; see Wikipedia.", ".")
                    or  0 < line.replace_all("<sup>[http:]</sup>", "")
                    or  0 < line.replace_all("(See [http:])", "")
                    or  0 < line.replace_all(" ([http:])", ""))
                        complexity = max(10, complexity);

                    if (line.contains(str::one_of("*" ))) { complexity = max(11, complexity); }
                    if (line.contains(str::one_of(":" ))) { complexity = max(12, complexity); }
                    if (line.contains(str::one_of("[]"))) { complexity = max(13, complexity); }
                    if (line.contains(str::one_of("<>"))) { complexity = max(91, complexity); }
                    if (line.contains(str::one_of("{}"))) { complexity = max(92, complexity); break; }
                    if (line.contains(str::one_of("#|"))) { complexity = max(93, complexity); break; }
                    if (line.contains("[http:]"        )) { complexity = max(94, complexity); break; }
                    if (line.contains("wiki") or line.contains("wikt") or
                        line.contains("Wiki") or line.contains("Wikt") or
                        line.contains("Category") or
                        line.contains("Appendix"))
                        complexity = max(96, complexity);

                }

                if (header == "pronunciation") content.erase_all("");

                if (complexity == 0 && header == "etymology"    ) complexity = 90;
                if (complexity <= 2 && header == "pronunciation") complexity = 90;

                str complexity_ = std::to_string(complexity);
                if (complexity_.size() == 1) complexity_ = "0" + complexity_;
                result.report (cap + str(content) + "\n",
                    header + " - complexity "+ complexity_);

                if (complexity >= 90) content.clear();
            }

            result.accept(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
