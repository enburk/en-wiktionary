#pragma once
#include "5.h"
namespace pass5
{
    Pass <entry, entry> pronunciation = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            array<str> hyphenations;

            for (auto & [header, forms, content] : topic)
            {
                if (!lexical_notes.contains(header)) continue;
                if (header != "pronunciation") continue;

                str cap =
                    esc   + "\n" +
                    title + "\n" +
                    esc   + "\n" + "\n" +
                    "==== " + header + " ==== " + forms + "\n";

                int complexity = 0;

                array<str> accepted;

                if (str(content) == "" ) complexity = 900; else
                if (str(content) == "*") complexity = 901; else
                {
                    for (str line : content)
                    {
                        line.strip();
                        if (line == "") continue;
                        if (line == "English:") continue;
                        if (line == "* English:") continue;
                        if (line == "* (''English language'')") continue;

                        line.replace_all("): , /", "): /"); // {{enPR}}, {{IPA}}
                        line.replace_all( ") , /",  ") /"); // {{enPR}}, {{IPA}}
                        line.replace_all( "* , /",  "* /"); // {{enPR}}, {{IPA}}
                        line.replace_all( "* , /",  "* /"); // {{enPR}}, {{IPA}}
                        line.replace_all( "/ ''or'' , /",  "/ ''or'' /");
                        line.replace_all( ", ,",  ",");
                        line.replace_all( ", ,",  ",");
                        if (line.starts_with(", /"))
                            line.upto(2).erase();

                        if (line.ends_with("/,")) {
                            line.truncate();
                            complexity = max(70,
                            complexity);
                        }

                        if (line.ends_with("/:")) {
                            complexity = max(75,
                            complexity);
                            //continue;
                        }

                        line.replace_all("[[w:Great Britain|Britain]]", "UK");
                        line.replace_all("[[w:United States|US]]", "US");
                        line.replace_all("[[Appendix:Glossary#plural|plural]]", "plural");
                        line.replace_all("[[Appendix:Glossary#predicative|predicative]]", "predicative");
                        line.replace_all("[[Appendix:English nouns#Attributive|attributive]]", "attributive");
                        line.replace_all("[[Appendix:Glossary#dialectal|dialect]]", "dialect");

                        if (not line.starts_with("/") or
                            not line.  ends_with("/"))
                            complexity = max(10, complexity);

                        if (line.contains(","))
                            complexity = max(15, complexity);

                        if (line.contains("{{rfv-pron"))
                            complexity = max(999, complexity);

                        line.strip();
                        if (line.starts_with("{{") and
                            line.  ends_with("}}")) {
                            complexity = max(20, complexity);
                            continue; }

                        if (line.starts_with("hyphenation:")) {
                            line = line.from(str("hyphenation:").size());
                            line.triml();
                            hyphenations += line;
                            if (hyphenations.size() > 1)
                            complexity = max(37, complexity); else
                            complexity = max(36, complexity);
                            continue;
                        }

                        line.strip();
                        if (line.starts_with("*") or
                            line.starts_with(":")) {
                            if (content.size() == 1)
                            complexity = max(30, complexity); else
                            complexity = max(35, complexity);
                            str prefix; while(
                                line.starts_with("*") or
                                line.starts_with(":")) {
                                line.erase(0);
                                line.strip();
                                prefix += ":";
                            }
                            if (line.starts_with("hyphenation:")) {
                                line = line.from(str("hyphenation:").size());
                                line.triml();
                                hyphenations += line;
                                if (hyphenations.size() > 1)
                                complexity = max(37, complexity); else
                                complexity = max(36, complexity);
                                continue;
                            }
                            prefix.truncate(4);
                            if (line == "") continue; else
                                line = prefix + " " +
                                line;
                        }

                        str s1, s2;
                        if (line.split_by("): ", s1, s2))
                        {
                            if (s2 == "")
                                complexity = max(98,
                                    complexity);

                            if (not s1.starts_with("(") or
                                not s1.  ends_with(")") or
                                not s2.starts_with("/") or
                                not s2.  ends_with("/"))
                                complexity = max(40,
                                    complexity);
                        }

                        if (line.contains(str::one_of(":" ))) { complexity = max(12, complexity); }
                        if (line.contains(str::one_of("<>"))) { complexity = max(74, complexity); }
                        if (line.contains(str::one_of("[]"))) { complexity = max(80, complexity); }
                        if (line.contains(            "[[" )) { complexity = max(81, complexity); }
                        if (line.contains("hyphenation"    )) { complexity = max(82, complexity); }
                        if (line.contains(str::one_of("#|"))) { complexity = max(83, complexity); }
                        if (line.contains(str::one_of("{}"))) { complexity = max(92, complexity); }
                        if (line.contains("[http:]"        )) { complexity = max(94, complexity); }
                        if (line.contains("???"            )) { complexity = max(95, complexity); }
                        if (line.contains("wiki") or line.contains("wikt") or
                            line.contains("Wiki") or line.contains("Wikt") or
                            line.contains("Category") or
                            line.contains("Appendix"))
                            complexity = max(96, complexity);

                        accepted += line;
                    }

                    int colons = 99;
                    for (str s : accepted) {
                        int n = 0;
                        for (char c : s)
                            if (c != ':') break;
                                else n++;
                        colons = min(colons, n);
                    }
                    if (colons > 0)
                        for (str & s : accepted) {
                            s.upto(colons).erase();
                            s.triml(); }

                    if (not str(accepted).contains("/")
                        and str(accepted).ends_with(":"))
                        complexity = max(91,
                            complexity);

                    if (not str(accepted).contains("/"))
                        complexity = max(50,
                            complexity);

                    accepted.erase_all("");
                    if (accepted.empty())
                        complexity = 90;

                    if (complexity > 90)
                        accepted.clear();
                }

                result.report(cap +
                    str(content)  + "\n" +
                    "================\n" +
                    str(accepted) + "\n",
                    "complexity " + std::to_string(
                     complexity));

                content = std::move(accepted);
            }

            if (not hyphenations.empty())
            {
                topic += paragraph{};
                topic.back().header  = "hyphenation";
                topic.back().content =  hyphenations;
            }

            result.accept(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
