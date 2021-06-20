#pragma once
#include "4.h"
namespace pass4
{
    str templates_xfix_(str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; auto & a = args;
        args.languaged();

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        args.kind  = "{{" + name + "}}";

        if (name == "prefixusex")
        {
            a.ignore_all();
            if (a.complexity == 2) { output = "''"+title+"'' + ''"+a[0]+"'' → ''"+a[1]+"''"; } else
            a.kind += " quest";
        }
        else
        if (name == "suffixusex")
        {
            a.ignore_all();
            if (a.complexity == 2) { output = "''"+a[0]+"'' + ''"+title+"'' → ''"+a[1]+"''"; } else
            if (a.complexity == 4) { output = "''"+a[0]+"'' + ''"+a[1] +"'' + ''"+a[2]+"'' → ''"+a[3]+"''"; } else
            a.kind += " quest";
        }
        else
        if (name == "blend"  or
            name == "affix"  or
            name == "prefix" or
            name == "suffix" or
            name == "confix" or
            name == "compound")
        {
            str
            q = a.acquire("alt1"); if (q != "" and a.unnamed.size() >= 1) { a[0] = q; }//kind = "{{suffix}} alt"; }
            q = a.acquire("alt2"); if (q != "" and a.unnamed.size() >= 2) { a[1] = q; }//kind = "{{suffix}} alt"; }
            q = a.acquire("alt3"); if (q != "" and a.unnamed.size() >= 3) { a[2] = q; }//kind = "{{suffix}} alt"; }
            if (name == "prefix" or name == "confix") if (a.unnamed.size() >= 1 and a[0] != "" and not a[0].ends_with  ("-")) a[0] = a[0] + "-";
            if (name == "suffix" or name == "confix") if (a.unnamed.size() >= 2 and a[1] != "" and not a[1].starts_with("-")) a[1] = "-" + a[1];
            if (name == "suffix" or name == "confix") if (a.unnamed.size() >= 3 and a[2] != "" and not a[2].starts_with("-")) a[2] = "-" + a[2];
            for (str & s : a.unnamed) if (s != "") s = "''" + s + "''";
            q = a.acquire("t1" ); if (q != "" and a.unnamed.size() >= 1) { a[0] += " (“"+q+"”)"; }//kind = "{{suffix}} trans"; }
            q = a.acquire("t2" ); if (q != "" and a.unnamed.size() >= 2) { a[1] += " (“"+q+"”)"; }//kind = "{{suffix}} trans"; }
            q = a.acquire("t3" ); if (q != "" and a.unnamed.size() >= 3) { a[2] += " (“"+q+"”)"; }//kind = "{{suffix}} trans"; }
            q = a.acquire("lit"); if (q != "" and a.unnamed.size() >= 1) { a[0] += " (literally “"+q+"”)"; }
            str lang1 = a.lang1 != "" and Languages.contains(a.lang1) ? Languages[a.lang1] : "";
            str lang2 = a.lang2 != "" and Languages.contains(a.lang2) ? Languages[a.lang2] : "";
            if (lang1 != "" and a.unnamed.size() >= 1 and a[0] != "") { a [0] = lang1 + " " + a[0]; a.kind += " lang"; }
            if (lang2 != "" and a.unnamed.size() >= 2 and a[1] != "") { a [1] = lang2 + " " + a[1]; a.kind += " lang"; }
            a.ignore_all();
            array<str> aa;
            for (str s : a.unnamed) if (s != "") aa += s;
            if (not a.kind.ends_with("lang")) a.kind += " " + std::to_string(min(3,aa.size()));
            if (not aa.empty())
            {
                output = str(aa, " + ");
                if (name == "blend") output = "Blend of " + output;
            }
        }
        else
        {
            a.kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }

        if (a.kind != "{{}}") 
        if (not a.opt.empty()) a.kind += " opt";

        if (a.kind.contains(" quest")) a.kind += " !!!!!";
        if (output.contains("\n")) a.kind +=  " #br#";
        if (output.contains("\n")) report = "==== "+title+" ==== "+header+" ==== "+"\n\n" + report;
        if (output.contains("\n")) output.replace_all("\n", " ");
        result.report (report + " => " + output + " == " + title, a.kind);
        return output;
    }

    Pass <entry, entry> templates_xfix = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("templates5", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                for (auto & line : content)
                {
                    bracketer b;
                    b.proceed_template = [&](str s){ return templates_xfix_(t, h, s, result); };
                    b.proceed(line);
                    line = b.output;
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }

        dump_templates_statistics(result);
    };
}
