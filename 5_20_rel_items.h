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

                for (str line : content) if (line != "")
                {
                    str pre, post;

                    if (line.starts_with("* ("))
                    {
                        complexity = 50;
                        line.split_by(")", pre, line, str::delimiter::to_the_left);
                        line.strip();
                        pre.upto(2).erase();
                        if (line.starts_with(":")) {
                            line.upto(1).erase();
                            line.strip();
                            pre += ":";
                        }
                        line = "* " + line;
                    }

                    if (line.ends_with(")"))
                    {
                        complexity = 60;
                        line.split_by("(", str::start_from_end(), line, post, str::delimiter::to_the_right);
                    }

                    if (!line.starts_with ("*" )) { complexity = 99; break; } else line = line.from(1); line.strip();
                    if (!line.starts_with ("[[")) { complexity = 99; break; } else line = line.from(2); line.strip();
                    if (!line.ends_with   ("]]")) { complexity = 99; break; } else line = line.upto(line.size()-2);

                    if (!line.contains_only(str::one_of(alnum))) complexity = max(0+1, complexity);
                    if (!line.contains_only(str::one_of(Alnum))) complexity = max(1+1, complexity);

                    line.replace_all("]] [[", " ");
                    line.replace_all("]]/[[", "/");
                    line.replace_all("]], [[", ", ");
                    line.replace_all("]]; [[", "; ");
                    line.replace_all("]] / [[", " / ");

                    if (!line.contains_only(str::one_of(ALnum))) complexity = max(2+1, complexity);
                    if (!line.contains_only(str::one_of(ALNum))) complexity = max(3+1, complexity);

                    line.replace_all("]], from [[", ", from ");

                    if (!line.contains_only(str::one_of(ALNUm))) complexity = max(4+1, complexity);
                    if (!line.contains_only(str::one_of(ALNUM))) complexity = max(5+1, complexity);

                    if (line.starts_with("Appendix:")
                    ||  line.starts_with(":Category:")
                    ) { complexity = max(10, complexity); continue; }

                    if (line.contains(str::one_of("[]{}:#|"))) { complexity = 99; break; }

                    if (pre  != "") line = pre + " " + line;
                    if (post != "") line += " " + post;

                    list += line;
                }

                if (complexity == 99) list.clear();

                str complexity_ = std::to_string(complexity);
                if (complexity_.size() == 1) complexity_ = "0" + complexity_;
                result.report (cap + str(content) + "\n=============\n" + str(list) + "\n",
                    "complexity "+ complexity_);

                content = list;
            }

            result.accept(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
