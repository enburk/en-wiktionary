#pragma once
#include "4.h"
namespace pass4
{
    str templates_mention_(str title, str header, str body, Result<entry> & result)
    {
        if (body == "QUOTE" || body == "rfdate" || body == "RQ") return "{{" + body + "}}";

        args args(body, false); str name = args.name; str arg = args.body; auto & a = args;

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        if (name == "etyl")
        {
            if (a.complexity >= 1)
            {
                if (Languages.contains(a[0])) {
                    output = Languages[a[0]];
                    kind += " 1"; } else
                    kind += " quest 1";
            }
            else kind += " quest 2";
        }
        else
        if (name == "link"       or
            name == "mention"    or
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
        //  if (a.unnamed.size() > 0 and duo and a[0] != "en" and a[0] != "mul") kind += " quest lang";
            if (a.unnamed.size() > 0 and duo) { a.unnamed.erase(0); a.complexity--; }

            str alt = a.acquire("alt");
            if (alt != "" and a.unnamed.size() > 1) {
                a[1] = alt; //kind += " alt";
            }

            if (a.unnamed.size() > 1 and a.unnamed.back() == "-") a.unnamed.truncate();
            if (a.unnamed.size() > 0 and not Languages.contains(a[0])) kind += " quest lang";
            if (a.unnamed.size() > 2 and a[2] != "") { output = "''"+a[2]+"''"; kind += " 1"; } else
            if (a.unnamed.size() > 1 and a[1] != "") { output = "''"+a[1]+"''"; kind += " 1"; } else
                                                     { output = "''(?)''";      kind += " 0"; }

            if (name != "link"
            and name != "mention"
            and a.unnamed.size() > 0
            and Languages.contains(a[0]))
                output = Languages[a[0]]
                    + " " + output;

            a.ignore("g"); a.ignore("g1"); a.ignore("g2"); // gender
            a.ignore("sc"); // script
            a.ignore("id");
            a.ignore("pos");
            
            str q;
            str nocap = a.acquire("nocap");
            str notext = a.acquire("notext");
            str tr = a.acquire("tr"); // transcript
            str tt = a.acquire("t"); // translation
            q = a.acquire("ts"); if (q != "") tr = q;
            if (a.unnamed.size() >= 4 and a[3] != "") tt = a[3];
            if (tr == "-") tr = "";
            if (tt == "-") tt = "";
            if (tr != "") tr = "''"+tr+"''";
            if (tt != "") tt = "“" +tt+ "”";
            q = a.acquire("lit"); if (q != "") tt = "literally “" +q+ "”";
            if (a.complexity >= 5) kind += " quest";
            if (tr == "" and tt != "") { output += " ("+tt+")"; kind += "t"; } else
            if (tr != "" and tt == "") { output += " ("+tr+")"; kind += "t"; } else
            if (tr != "" and tt != "") { output += " ("+tr+", "+tt+")"; kind += "t"; }

            if (name == "calque" and notext == "") output = (nocap == "" ? "Calque of " : "calque of ") + output;
            if (name == "back-formation" and notext == "") output = (nocap == "" ? "Back-formation from " : "back-formation from ") + output;
        }
        else
        {
            kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }

        if (output.contains("\n")) kind +=  " #br#";
        if (output.contains("\n")) report = "==== " + title + " ==== " + header + " ==== " + "\n\n" + report;
        if (output.contains("\n")) output.replace_all("\n", " ");
        if (kind != "{{}}") result.report (report + " => " + output + " == " + title, kind);
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
