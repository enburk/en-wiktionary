#pragma once
#include "5.h"
namespace pass5
{
    Pass <entry, entry> rel_items = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0) logout("rel_items", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                if (!related_items.contains(header)) continue;

                str cap =
                    esc   + "\n" +
                    title + "\n" +
                    esc   + "\n" + "\n" +
                    "==== " + header + " ==== " + forms + "\n";

                array<str> list; int complexity = 0;

                for (str lines : content) if (lines != "") // {{col}}
                {
                    for (str line : lines.split()) if (line != "")
                    {
                        line.strip();
                        if (line.starts_with("{{") and
                            line.  ends_with("}}")) {
                            complexity = max(20, complexity);
                            continue; }

                        if (line.starts_with("''(") and
                            line.  ends_with("):''")) {
                            complexity = max(30, complexity);
                            list += line;
                            continue; }

                        if (line.starts_with("(''") and
                            line.  ends_with("''):")) {
                            complexity = max(30, complexity);
                            list += line;
                            continue; }

                        if (!line.starts_with ("*")) complexity = 99;
                        else line.erase(0);
                        line.strip();

                        while (line.starts_with ("*")) {
                            complexity = max(40, complexity);
                            line.erase(0);
                            line.strip();
                        }

                        if (line.starts_with("See also") or
                            line.starts_with("''See also") or
                            line.starts_with("[[Appendix:") or
                            line.starts_with("[[Category:") or
                            line.starts_with("[[:Appendix:") or
                            line.starts_with("[[:Category:")) {
                            complexity = max(50, complexity);
                            continue; }

                        str pre, post;
                        if (line.starts_with("(")) {
                            complexity = max(60, complexity);
                            line.split_by(")", pre, line, str::delimiter::to_the_left);
                            line.strip();
                            if (line.starts_with(":")) {
                                line.upto(1).erase();
                                line.strip();
                                pre += ":";
                            }
                        }
                        if (line.ends_with(")")) {
                            complexity = max(70, complexity);
                            line.split_by("(", str::start_from_end(),
                            line, post, str::delimiter::to_the_right);
                        }

                        str s1, s2;
                        if (line.split_by("; See also", s1, s2)) { line = s1; complexity = max(80, complexity); }
                        if (line.split_by("; see also", s1, s2)) { line = s1; complexity = max(80, complexity); }

                        if (!line.contains_only(str::one_of(Alnum))) complexity = max(1+1, complexity);
                        if (!line.contains_only(str::one_of(ALnum))) complexity = max(2+1, complexity);
                        if (!line.contains_only(str::one_of(ALNum))) complexity = max(3+1, complexity);
                        if (!line.contains_only(str::one_of(ALNUm))) complexity = max(4+1, complexity);
                        if (!line.contains_only(str::one_of(ALNUM))) complexity = max(5+1, complexity);

                        if (line.contains("wiki") or line.contains("wikt") or
                            line.contains("Wiki") or line.contains("Wikt") or
                            line.contains("Category") or
                            line.contains("Appendix"))
                            complexity = max(96, complexity);
                    
                        if (line.contains(str::one_of("[]:#|")))
                            complexity = max(97, complexity);
                    
                        if (line.contains(str::one_of("{}")))
                            complexity = max(98, complexity);

                        line.strip();
                        if (line != "")
                        {
                            if (pre  != "") line = pre + " " + line;
                            if (post != "") line += " " + post;
                            list += line;
                        }
                    }
                }

                if (complexity >= 90) list.clear();

                str complexity_ = std::to_string(complexity);
                if (complexity_.size() == 1) complexity_ = "0" + complexity_;
                result.report (cap + str(content) +
                    "\n=============\n" + str(list) + "\n",
                    (header == "see also" ? header+" " : str()) +
                    "complexity "+ complexity_);

                content = list;
            }

            result.accept(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
