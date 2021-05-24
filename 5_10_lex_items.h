#pragma once
#include "5.h"
namespace pass5
{
    Pass <entry, entry> lex_items = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("lex_items", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                if (not lexical_items.contains(header)) continue;

                str cap =
                    esc   + "\n" +
                    title + "\n" +
                    esc   + "\n" + "\n" +
                    "==== " + header + " ==== " + forms + "\n";

                int complexity = 0;

                for (auto & line : content)
                    if (not line.starts_with ("# ") or
                        str(line.from(2)).contains(str::one_of("#:*")))
                        { complexity++; break; }

                if (complexity == 0) {
                    result.report (cap + str(content) + "\n", "complexity 00 ");
                    continue;
                }

                for (auto & line : content)
                    if((not line.starts_with ("# ")
                    and not line.starts_with ("## ")
                    and not line.starts_with ("##: ")
                    and not line.starts_with ("#: ")) or
                        str(line.from(3).upto(10)).contains(str::one_of("#:*")))
                        { complexity++; break; }

                if (complexity == 1) {
                    result.report (cap + str(content) + "\n", "complexity 01");
                    continue;
                }

                for (auto & line : content)
                    if((not line.starts_with ("# ")
                    and not line.starts_with ("## ")
                    and not line.starts_with ("##: ")
                    and not line.starts_with ("#:: ")
                    and not line.starts_with ("#: ")) or
                        str(line.from(3).upto(10)).contains(str::one_of("#:*")))
                        { complexity++; break; }

                if (complexity == 2) {
                    result.report (cap + str(content) + "\n", "complexity 02");
                    continue;
                }

                array<str> accepted;

                for (auto line : content)
                {
                    str s = line;
                    str sharps; while (s.starts_with("#")) { sharps += "#"; s.upto(1).erase(); s.triml(); }
                    str colons; while (s.starts_with(":")) { colons += ":"; s.upto(1).erase(); s.triml(); }
                    str starss; while (s.starts_with("*")) { starss += "*"; s.upto(1).erase(); s.triml(); }

                    if (sharps.size() >= 1 and
                        sharps.size() <= 2 and
                        colons.size() <= 2 and
                        sharps.size() + colons.size() <= 4 and
                        starss.size() == 0 and
                        not s.starts_with("#") and
                        not s.starts_with(":") and
                        not s.starts_with("*"))
                        line = sharps + colons + " " + s;

                    else
                    {
                        complexity = 10;
                        while (s.starts_with("#") or
                               s.starts_with(":") or
                               s.starts_with("*")) { starss += "*"; s.upto(1).erase(); s.triml(); }

                        starss.resize(min(starss.size(), 4 - sharps.size() - colons.size()));
                        starss.replace_all("*", ":");
                        line = sharps + colons + starss + " " + s;
                    }

                    accepted += line;
                }

                if (accepted.empty()) complexity = 99;

                str complexity_ = std::to_string(complexity);
                if (complexity_.size() == 1)
                    complexity_ = "0" +
                    complexity_;

                result.report(cap +
                    str(content)  + "\n" +
                    "================\n" +
                    str(accepted) + "\n",
                    "complexity " +
                     complexity_);

                content = accepted;
            }

            result.accept(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
