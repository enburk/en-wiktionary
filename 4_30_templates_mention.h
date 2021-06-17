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

        if (name == "zh-l"
        or  name == "zh-m"
        or  name == "ja-r"
        or  name == "ja-l"
        or  name == "ko-l"
        or  name == "he-m"
        or  name == "he-l"
        or  name == "ne-l"
        or  name == "vi-l"
        or  false)
        {
            output = a.link("", "");
            a.kind = "lang " + a.kind;
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
            name == "back-formation" or
            false)
        {
            bool duo =
                name == "borrowed"   or
                name == "derived"    or
                name == "inherited"  or
                name == "calque"     or
                false;

            if (a.unnamed.size() > 0 and duo) {
                a.unnamed.erase(0);
                a.complexity--;
            }

            a.dotcapped();
            a.ignore("g"); a.ignore("g1"); a.ignore("g2"); // gender

            if (a.unnamed.size() > 0
            and Languages.contains(a[0])) {
                output = Languages[a[0]];
                a.unnamed.erase(0);
                a.complexity--;
            }
            if (output != "")
                output += " ";

            if (a.unnamed.size() > 0 and
                a.unnamed.back() == "-")
                a.unnamed.truncate();
            
            output += a.link("''", "''");

            a.kind.replace_all("}} 2", "}} 1");

            if (name == "calque"        ) output = a.capitalized("Calque of "          ) + output;
            if (name == "back-formation") output = a.capitalized("Back-formation from ") + output;
        }
        else
        {
            a.kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }

        if (output.contains("\n")) a.kind +=  " #br#";
        if (output.contains("\n")) report = "==== "+title+" ==== "+header+" ==== "+"\n\n" + report;
        if (output.contains("\n")) output.replace_all("\n", " ");
        if (a.kind != "{{}}") result.report (report + " => " + output + " == " + title, a.kind);
        return output;
    }

    Pass <entry, entry> templates_mention = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

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
