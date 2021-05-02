#pragma once
#include "4.h"
namespace pass4
{
    array<str> vocabulary;

    str suffixed (str word, str suffix)
    {
        if (suffix.starts_with("e"))
        {
            if (word.ends_with("e")
            ||  word.ends_with("y")) {
                word.truncate(); if (word.ends_with("e"))
                word.truncate();
                word += "i";
            }
        }
        if (suffix.starts_with("s"))
        {
            if (word.ends_with("x")
            ||  word.ends_with("s")) word += "e"; else
            if (word.ends_with("y")) {
                word.truncate(); if (word.ends_with("e"))
                word.truncate();
                word += "ie";
            }
        }
        return word + suffix;
    }
    str comparative (str word, str form)
    {
        if (form == "") form = suffixed(word, "er");
        lexforms[word] += lexform{"comparative", "?", form};
        return form;
    }
    str superlative (str word, str form)
    {
        if (form == "") form = suffixed(word, "est");
        lexforms[word] += lexform{"superlative", "?", form};
        return form;
    }
    str plural (str word, str form = "")
    {
        if (form == "") form = suffixed(word, "s");
        lexforms[word] += lexform{"plural", "?", form};
        return form;
    }
    str third_person (str word, str form = "")
    {
        if (form == "") form = suffixed(word, "s");
        lexforms[word] += lexform{"third-person", "?", form};
        return form;
    }
    str present_participle (str word, str form = "")
    {
        if (form == "") form = suffixed(word, "ing");
        lexforms[word] += lexform{"present participle", "?", form};
        return form;
    }
    str simple_past (str word, str form = "")
    {
        if (form == "") form = suffixed(word, "ed");
        lexforms[word] += lexform{"simple past", "?", form};
        return form;
    }
    str past_participle (str word, str form = "")
    {
        if (form == "") form = suffixed(word, "ed");
        lexforms[word] += lexform{"past participle", "?", form};
        return form;
    }

    Pass <entry, entry> lexforms1 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, UPDATING_REPORTS};

        vocabulary.reserve (1024*1024);

        for (auto && [title, topic] : input)
        {
            vocabulary += title;

            static int64_t nn = 0; if (++nn % 100'000 == 0) logout("lexforms1", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                str report = header + " ";

                if (header == "alternative forms")
                {
                    int complexity = 0;

                    for (str s : content)
                    {
                        int kind = 10;
                        if (s.contains_only(str::one_of(alnum))) kind = 0; else
                        if (s.contains_only(str::one_of(alnum+" -"))) kind = 1; else
                        if (s.contains_only(str::one_of(alnum+" -.!?"))) kind = 2; else
                        if (s.contains_only(str::one_of(alnum+" -.!?'"))) kind = 3; else
                        if (s.contains_only(str::one_of(ALNUM))
                        && !s.contains(str::one_of("/(){}[]")))
                        {
                            if (!title.contains(","))
                            {
                                kind = s.contains(",") ? 5 : 4;
                                auto list = s.split_by(",");
                                for (str i : list) { i.strip();
                                lexforms[title] += lexform{"alternative", "?", i}; }
                            }
                            else
                            {
                                int n = 0; for (char c : title) if (c == ',') n++;
                                int m = 0; for (char c : s    ) if (c == ',') m++;
                                if (m == n) { kind = 6;
                                lexforms[title] += lexform{"alternative", "?", s}; }
                            }
                        }

                        if (kind < 4) lexforms[title] += lexform{"alternative", "?", s};
                            
                        complexity = max(kind, complexity);
                    }

                    report += complexity < 10 ? std::to_string(complexity) : "!!! quest";

                    result.report ("===== " + title + " ===== \n" + str(content) + "\n", report);
                }

                if (!lexical_items.contains(header)) continue;
                
                if (header == "adjective" || header == "adverb")
                {
                    report = "adj-adv ";

                    auto more = [title](){ return "'''" + comparative(title, "more "+ title) + "'''"; };
                    auto most = [title](){ return "'''" + superlative(title, "most "+ title) + "'''"; };
                    auto er   = [title](){ return "'''" + comparative(title, "") + "'''"; };
                    auto est  = [title](){ return "'''" + superlative(title, "") + "'''"; };

                    auto cmp = [title](str a, str b = "", str c = ""){
                        str s  =  "''comparative'' " + a;
                        if (b != "") s += " ''or'' " + b;
                        if (c != "") s += " ''or'' " + c;
                        return s;
                    };
                    auto sup = [title](str a, str b = "", str c = ""){
                        str s  =  "''superlative'' " + a;
                        if (b != "") s += " ''or'' " + b;
                        if (c != "") s += " ''or'' " + c;
                        return s;
                    };

                    if (forms == ""       ) { report += ""; } else
                    if (forms == "?"      ) { report += "x";       forms = ""; } else
                    if (forms == "-"      ) { report += "-";       forms = "(''not comparable'')"; } else
                    if (forms == "+"      ) { report += "+";       forms = "(" + cmp(more()) + ", " + sup(most()) + ")"; } else
                    if (forms == "more"   ) { report += "more";    forms = "(" + cmp(more()) + ", " + sup(most()) + ")"; } else
                    if (forms == "er"     ) { report += "er";      forms = "(" + cmp(er  ()) + ", " + sup(est ()) + ")"; } else
                    if (forms == "er|more") { report += "er+more"; forms = "(" + cmp(er(),more()) + ", " + sup(est(),most()) + ")"; } else
                    if (forms == "more|er") { report += "more+er"; forms = "(" + cmp(more(),er()) + ", " + sup(most(),est()) + ")"; } else
                
                        report += "- quest";
                }
                else
                if (header == "noun" || header == "proper noun")
                {
                    report = "noun ";

                    if (forms == "+" && header == "proper noun") forms = "";

                    if (forms == ""       ) { report += ""; } else
                    if (forms == "?"      ) { report += "x";  forms = ""; } else
                    if (forms == "-"      ) { report += "-";  forms = "(''uncountable'')"; } else
                    if (forms == "!"      ) { report += "-";  forms = "(''plural not attested'')"; } else
                    if (forms == "+"      ) { report += "+";  forms = "(''plural'' '''" + plural(title) + "''')"; } else
                    if (forms == "s"      ) { report += "s";  forms = "(''plural'' '''" + plural(title, title+"s") + "''')"; } else
                    if (forms == "es"     ) { report += "es"; forms = "(''plural'' '''" + plural(title, title+"es") + "''')"; } else
                    if (forms == "~"      ) { report += "~";  forms = "(''countable and uncountable, plural'' '''" + plural(title) + "''')"; } else
                    if (forms == "~|s"    ) { report += "~s"; forms = "(''countable and uncountable, plural'' '''" + plural(title+"s") + "''')"; } else
                    if (forms == "~|es"   ) { report += "~s"; forms = "(''countable and uncountable, plural'' '''" + plural(title+"es") + "''')"; } else
                    if (forms == "-|s"    ) { report += "-s"; forms = "(''usually uncountable, plural'' '''" + plural(title+"s") + "''')"; } else
                    if (forms == "-|es"   ) { report += "-s"; forms = "(''usually uncountable, plural'' '''" + plural(title+"es") + "''')"; } else
                    {
                        args args ("verb|"+forms);

                        if (args.complexity == 1)
                        {
                            report += "1";
                            if (args[0].upto(2) != title.upto(2)) report += " - quest";
                            else forms = "(''plural'' '''" +
                                plural(title, args[0]) +
                                "''')";
                        }
                        else
                        if (args.complexity == 2)
                        {
                            report += "2";
                            if (args[0] == "~"  && args[1].upto(2) == title.upto(2)) { report += " ~"; forms = "(''countable and uncountable, plural'' '''" + plural(title, args[1]) + "''')"; } else
                            if (args[0] == "-"  && args[1].upto(2) == title.upto(2)) { report += " -"; forms = "(''usually uncountable, plural'' '''" + plural(title, args[1]) + "''')"; } else
                            if (args[0] == "s"  && args[1].upto(2) == title.upto(2)) { report += " s"; forms = "(''plural'' '''" + plural(title, title+"s") + " ''or'' " + plural(title, args[1]) + "''')"; } else
                            if (args[0] == "es" && args[1].upto(2) == title.upto(2)) { report += " s"; forms = "(''plural'' '''" + plural(title, title+"es") + " ''or'' " + plural(title, args[1]) + "''')"; } else
                            if (args[0].upto(2) != title.upto(2)
                            ||  args[1].upto(2) != title.upto(2)) report += " - quest";
                            else forms = "(''plural'' '''" +
                                plural(title, args[0]) + " ''or'' " +
                                plural(title, args[1]) +
                                "''')";
                        }
                        else report += "!!! quest";
                    }
                }
                else
                if (header == "verb")
                {
                    auto es  = [title](str s = ""){ return "'''" + third_person      (title, s) + "'''"; };
                    auto ing = [title](str s = ""){ return "'''" + present_participle(title, s) + "'''"; };
                    auto ed  = [title](str s = ""){ return "'''" + simple_past       (title, s) + "'''"; };
                    auto pp  = [title](str s = ""){ return "'''" + past_participle   (title, s) + "'''"; };

                    if (forms == ""       ) { report += ""; } else
                    if (forms == "+"      ) { report += "+"; forms = "(" + es() + ";" + ing() + ";" + ed() + ";" + pp() + ")"; } else
                    if (forms == "es"     ) { report += "es"; forms = "(" + es(title+"es") + ";" + ing() + ";" + ed() + ";" + pp() + ")"; } else
                    {
                        args args ("verb|"+forms);

                        if (args.complexity == 1)
                        {
                            report += "1";
                            if (args[0].upto(2) != title.upto(2)) report += " - quest"; else
                            forms =  "(" + 
                            es (args[0]+"es" ) + ";" + 
                            ing(args[0]+"ing") + ";" + 
                            ed (args[0]+"ed" ) + ";" + 
                            pp (args[0]+"ed" ) + ")";
                        }
                        else
                        if (args.complexity == 3)
                        {
                            report += "3";
                            if (args[0].upto(2) != title.upto(2)
                            ||  args[1].upto(2) != title.upto(2)
                            ||  args[2].upto(2) != title.upto(2)) report += " - quest"; else
                            forms =  "(" + 
                            es (args[0]) + ";" + 
                            ing(args[1]) + ";" + 
                            ed (args[2]) + ";" + 
                            pp (args[2]) + ")";
                        }
                        else
                        if (args.complexity == 4)
                        {
                            report += "4";
                            if (args[0].upto(2) != title.upto(2)
                            ||  args[1].upto(2) != title.upto(2)
                            ||  args[2].upto(2) != title.upto(2)
                            ||  args[3].upto(2) != title.upto(2)) report += " - quest"; else
                            forms =  "(" + 
                            es (args[0]) + ";" + 
                            ing(args[1]) + ";" + 
                            ed (args[2]) + ";" + 
                            pp (args[3]) + ")";
                        }
                        else report += "!!! quest";
                    }
                }
                else continue;

                result.report (title + " == " + header + " == " + forms, report);
            }

            result.accept (entry {std::move(title), std::move(topic)});
        }
    };
}
