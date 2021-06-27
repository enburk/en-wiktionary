#pragma once
#include "5.h"
namespace pass5
{
    Pass <entry, entry> lex_items = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("lex_items", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                if (not lexical_items.contains(header)) continue;

                content.erase_all("");

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
                    result.report (cap + str(content) + "\n", "complexity 0 ");
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
                    result.report (cap + str(content) + "\n", "complexity 1");
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
                    result.report (cap + str(content) + "\n", "complexity 2");
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
                        complexity = 8;
                        while (s.starts_with("#") or
                               s.starts_with(":") or
                               s.starts_with("*")) { starss += "*"; s.upto(1).erase(); s.triml(); }

                        starss.resize(min(starss.size(), 4 - sharps.size() - colons.size()));
                        starss.replace_all("*", ":");
                        line = sharps + colons + starss + " " + s;
                    }

                    accepted += line;
                }

                if (accepted.empty()) complexity = 9;

                result.report(cap +
                    str(content)  + "\n" +
                    "================\n" +
                    str(accepted) + "\n",
                    "complexity " + std::to_string(
                     complexity));

                content = std::move(accepted);
            }

            for (auto & [header, forms, content] : topic)
            {
                if (not lexical_items.contains(header)) continue;

                str cap =
                    esc   + "\n" +
                    title + "\n" +
                    esc   + "\n" + "\n" +
                    "==== " + header + " ==== " + forms + "\n";

                int problem = 0; bool sensed = false;

                array<str> accepted;

                while (content.size() > 0 and
                      (content.front() == "# " or
                       content.front() == "#: "))
                       content.erase(0);

                while (content.size() > 0 and
                      (content.back() == "# " or
                       content.back() == "#: "))
                       content.truncate();

                for (auto line : content)
                {
                    str prefix, s; line.split_by(" ", prefix, s);

                    if (s.starts_with("See ") and
                        prefix.ends_with(":")) {
                        problem = max(2, problem);
                        continue; }

                    if (s.contains("{{") or
                        s.contains("}}")) {
                        problem = max(3, problem);
                        continue; }

                    if (0 < s.replace_all("; see Wikipedia.", ".")
                    or  0 < s.replace_all("330AD[http:].", "330AD.")
                    or  0 < s.replace_all("imperial unit [http:] or", "imperial unit or")
                    or  0 < s.replace_all("<sup>[http:]</sup>", "")
                    or  0 < s.replace_all("(Formal definitions can be found at [http:]).", "")
                    or  0 < s.replace_all("(See [http:])", "")
                    or  0 < s.replace_all(" ([http:])", "")
                    or  0 < s.replace_all(" ({{QUOTE}})", ""))
                        problem = max(4, problem);

                    if (s.ends_with(".[http:]")) {
                        s.resize(s.size() - 7);
                        problem = max(5, problem); }

                    if (s.ends_with(". [http:]") or
                        s.ends_with(") [http:]")) {
                        s.resize(s.size() - 8);
                        problem = max(5, problem); }

                    if (s.ends_with(". [http:].") or
                        s.ends_with("; [http:].") or
                        s.ends_with(") [http:].")) {
                        s.resize(s.size() - 9);
                        problem = max(5, problem); }

                    if (s.contains("[http:]")) {
                        if (prefix.ends_with("#"))
                        problem = max(7, problem); else
                        problem = max(6, problem);
                        continue; }

                    if (s == "") {
                        problem = max(1, problem);
                        continue; }

                    if (prefix.ends_with("#"))
                        sensed = true;

                    accepted += prefix + " " + s;
                }

                if (not sensed or
                    accepted.empty())
                    problem =  9;
                if (problem == 0) continue;
                if (problem >= 9) accepted.clear();

                result.report(cap +
                    str(content)  + "\n" +
                    "================\n" +
                    str(accepted) + "\n",
                    "problem " + std::to_string(
                     problem));

                content = std::move(accepted);
            }

            result.accept(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
