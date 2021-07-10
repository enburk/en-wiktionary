#pragma once
#include "4.h"
namespace pass4
{
    str templates_mention_(str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; auto & a = args;

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        args.kind  = "{{" + name + "}}";

        if (name == "lang")
        {
            a.languaged();
            a.ignore("sc"); // script
            a.ignore("face");
            if (a.complexity >= 1) output = "''" + a[0] + "''";
            else a.kind += " quest";
        }
        else
        if (name == "etyl")
        {
            if (a.complexity >= 1)
            {
                if (Languages.contains(a[0]))
                    output = Languages[a[0]];
                    else a.kind += " quest 1";
            }
            else a.kind += " quest 2";
        }
        else
        if (name == "mention"    or
            name == "borrowed"   or
            name == "derived"    or
            name == "inherited"  or
            name == "cognate"    or
            name == "noncognate" or
            name == "calque"     or
            name == "ellipsis"   or
            name == "partial calque"    or
            name == "back-formation"    or
            name == "langname-mention"  or
            name == "learned borrowing" or
            name == "unadapted borrowing" or
            name == "semantic loan"     or
            name == "aphetic form"      or
            name == "transliteration"   or
            false)
        {
            bool duo =
                name == "borrowed"   or
                name == "derived"    or
                name == "inherited"  or
                name == "calque"     or
                name == "partial calque"    or
                name == "learned borrowing" or
                name == "unadapted borrowing" or
                name == "semantic loan"     or
                name == "transliteration"   or
                false;

            if (duo)
            a.languaged();
            a.dotcapped();
            a.ignore("g"); a.ignore("g1"); a.ignore("g2"); // gender
            a.ignore("w");

            if (a.size() > 0
            and Languages.contains(a[0])) {
                output =
                    a[0] == "mul" ? "translingual" :
                    a[0] == "en" ? "" :
                    Languages[a[0]];
                a.unnamed.erase(0);
                a.complexity--;
            }

            if (a.size() == 0 or // ''(?)''
                a.size() >= 2 or
                a[0] != "-")
                output += (
                output == "" ? "" : " ")
                    + a.link("''", "''");
            else a.kind += " 0";

            if (name == "calque"        ) output = a.capitalized("Calque of "          ) + output;
            if (name == "ellipsis"      ) output = a.capitalized("Ellipsis of "        ) + output;
            if (name == "back-formation") output = a.capitalized("Back-formation from ") + output;
            if (name == "back-formation") output = a.capitalized("Transliteration of " ) + output;
            if (name == "semantic loan" ) output = a.capitalized("Semantic loan from " ) + output;
            if (name == "aphetic form"  ) output = a.capitalized("Aphetic form of "    ) + output;
        }
        else
        if (name == "clipping")
        {
            a.languaged();
            a.dotcapped();
            if (a.complexity == 0) output = "clipping"; else
            output = a.capitalized("Clipping of ") + a.link("''", "''");
        }
        else
        if (name == "doublet")
        {
            a.languaged();
            a.dotcapped();
            a.ignore_all(); 
            output = a.capitalized("Doublet of ''") + str::list(a.unnamed, ", ", " and ") + "''";
        }
        else
        if (name == "onomatopoeic")
        {
            a.languaged();
            a.dotcapped();
            str s = a.acquire("title");
            output = a.capitalized(s != "" ? s : "Onomatopoeic");
        }
        else
        if (name == "inflection of")
        {
            a.languaged(); if (a.size() < 3) a.kind += " quest size"; else
            {
                a.ignore_all(); if (not a.opt.empty()) a.kind += " opt";
                str word = a.unnamed[0]; a.unnamed.erase(0);
                str sept = a.unnamed[0]; a.unnamed.erase(0);
                if (sept != "") a.kind +=
                    " quest sept";

                output = "";

                for (str s : a.unnamed)
                {
                    if (output != ""  ) output += " ";
                    if (s == "1"      ) output += "first-person"; else
                    if (s == "2"      ) output += "second-person"; else
                    if (s == "3"      ) output += "third-person"; else
                    if (s == "3s"     ) output += "third-person singular"; else
                    if (s == "1//3"   ) output += "first/third-person"; else
                    if (s == "1//2//3") output += "first/second/third-person"; else
                    if (s == "s"      ) output += "singular"; else
                    if (s == "p"      ) output += "plural"; else
                    if (s == "pl"     ) output += "plural"; else
                    if (s == "plural" ) output += "plural"; else
                    if (s == "s//p"   ) output += "singular/plural"; else
                    if (s == "f"      ) output += "feminine"; else
                    if (s == "n"      ) output += "neiter"; else
                    if (s == "sim"    ) output += "simple"; else
                    if (s == "simple" ) output += "simple"; else
                    if (s == "pres"   ) output += "present"; else
                    if (s == "present") output += "present"; else
                    if (s == "past"   ) output += "past"; else
                    if (s == "present//past" ) output += "present/past"; else
                    if (s == "part"   ) output += "participle"; else
                    if (s == "participle" ) output += "participle"; else
                    if (s == "ind"    ) output += "indicative"; else
                    if (s == "indc"   ) output += "indicative"; else
                    if (s == "col"    ) output += "collective"; else
                    if (s == "obs"    ) output += "obsolete"; else
                    if (s == "alternative") output += "alternative"; else
                    if (s == "finite usage") output += "finite usage"; else
                    if (s == "nom"    ) output += "nominative"; else
                    if (s == "gen"    ) output += "genitive"; else
                    if (s == "def"    ) output += "definite"; else
                    if (s == "act"    ) output += "active"; else
                    if (s == "an"     ) output += "animate"; else
                    if (s == "imp"    ) output += "imperative"; else
                    if (s == "sub"    ) output += "subjunctive"; else
                    if (s == "subj"   ) output += "subjunctive"; else
                    if (s == "cond"   ) output += "conditional"; else
                    if (s == "comd"   ) output += "comparative degree"; else
                    if (s == "supd"   ) output += "superlative degree"; else
                    if (s == "form"   ) output += "form"; else
                    if (s == "tense"  ) output += "tense"; else
                    if (s == ";"      ) output += "'' and ''"; else

                    { output += "?"; a.kind += "!"; }
                }

                output = "''" + output + " of'' '''" + word + "'''";
            }
        }
        else
        {
            a.kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }

        if (a.kind != "{{}}") 
        if (not a.opt.empty()) a.kind += " opt";

        a.kind.replace_all("}} 2", "}} 1");
        a.kind.replace_all("}} 1 tr", "}} 1");
        a.kind.replace_all("}} 0 tr", "}} 0");

        if (output.contains("\n")) a.kind += " #br#";
        if (output.contains("\n"))
            report = esc+" "+title+"\n"+report+"\n====\n"+output; else
            report = report + " => " + output + " == " + title;
        if (a.kind != "{{}}") result.report (report, a.kind);
        return output;
    }

    Pass <entry, entry> templates_mention = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        bool first_time = true;

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("templates2", nn, input.cargo);

            if (first_time) {
                first_time = false;
            
                for (auto & [name, analysis] : Repo)
                {
                    if (name.starts_with("languages/code"))
                    {
                        for (auto & cluster : analysis.clusters)
                        {
                            auto ee = cluster.elements;
                            auto cc = cluster.clusters;

                            if (ee.size() == 2
                            and ee[0].token
                            and ee[0].token->text == "return")
                            for (auto & e : ee[1].elements)
                            {
                                if (e.elements.size() == 3
                                and e.elements[0].elements.size() == 1
                                and e.elements[0].elements[0].elements.size() == 1
                                and e.elements[0].elements[0].elements[0].token
                                and e.elements[2].token)
                                {
                                    str abbr = e.elements[0].elements[0].elements[0].token->text;
                                    str full = e.elements[2].token->text;
                                    abbr.strip("\"");
                                    full.strip("\"");
                                    result.report(abbr + " = " + full, "languages");
                                    Languages[abbr] = full;
                                }
                            }
                        }
                    }
                    if (name.starts_with("families/data") or
                        name.starts_with("etymology languages/data"))
                    {
                        for (auto & cluster : analysis.clusters)
                        {
                            auto ee = cluster.elements;
                            auto cc = cluster.clusters;

                            if (ee.size() == 5
                            and ee[0].token
                            and ee[0].token->text == "m"
                            and ee[1].elements.size() == 1
                            and ee[1].elements[0].elements.size() == 1
                            and ee[1].elements[0].elements[0].token
                            and ee[3].token
                            and ee[3].token->text == "m"
                            and ee[4].elements.size() == 1
                            and ee[4].elements[0].elements.size() == 1
                            and ee[4].elements[0].elements[0].token
                            ) {
                                str abbr = ee[1].elements[0].elements[0].token->text;
                                str full = ee[4].elements[0].elements[0].token->text;
                                abbr.strip("\"");
                                full.strip("\"");
                                Languages[abbr] = Languages[full];
                                result.report(abbr + " = " + full +
                                    " = " + Languages[full],
                                    "languages etyl");
                            }

                            if (ee.size() == 4
                            and ee[0].token
                            and ee[0].token->text == "m"
                            and ee[1].elements.size() == 1
                            and ee[1].elements[0].elements.size() == 1
                            and ee[1].elements[0].elements[0].token)
                            {
                                str abbr = ee[1].elements[0].elements[0].token->text;
                                abbr.strip("\"");

                                for (auto & pp : ee[3].elements)
                                {
                                    auto & p = pp.elements;
                                    if (p.size() == 3
                                    and p[0].token
                                    and p[0].token->text == "canonicalName"
                                    and p[2].token)
                                    {
                                        str full = p[2].token->text;
                                        full.strip("\"");
                                        result.report(abbr + " = " + full, "languages etyl");
                                        Languages[abbr] = full;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                for (auto & line : content)
                {
                    bracketer b;
                    b.proceed_template = [&](str s){ return templates_mention_(t, h, s, result); };
                    b.proceed(line);
                    line = b.output;
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
