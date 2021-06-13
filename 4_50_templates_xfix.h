#pragma once
#include "4.h"
namespace pass4
{
    str templates_xfix_(str title, str header, str body, Result<entry> & result)
    {
        args args (body, false); str name = args.name; str arg = args.body; auto & a = args;

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        if (a.unnamed.size() > 0 and not Languages.contains(a[0])) kind += " quest lang";
        if (a.unnamed.size() > 0) { a.unnamed.erase(0); a.complexity--; }

        if (name == "prefixusex")
        {
            a.ignore_all();
            if (a.complexity == 2) { output = "''"+title+"'' + ''"+a[0]+"'' → ''"+a[1]+"''"; } else
            kind += " quest";
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
            if (a.lang1 != "" and Languages.contains(a.lang1) and a.unnamed.size() >= 1 and a[0] != "") { a [0] = Languages[a.lang1] + " " + a[0]; kind += " lang"; }
            if (a.lang2 != "" and Languages.contains(a.lang2) and a.unnamed.size() >= 2 and a[1] != "") { a [1] = Languages[a.lang2] + " " + a[1]; kind += " lang"; }
            a.ignore_all();
            array<str> aa;
            for (str s : a.unnamed) if (s != "") aa += s;
            output = str(aa, " + ");
            if (name == "blend") output = "Blend of " + output;
            kind += " " + std::to_string(min(3,aa.size()));
        }
        else
        {
            kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }
        if (kind != "{{}}" and not
            templates_usage[__FILE__].contains(name)) {
            templates_usage[__FILE__].insert  (name);
            result.report(esc + "\n" + Templates[name]
                 + "\n" + esc + "\n", "{{"+name+"}}"); }

        if (kind.contains(" quest")) kind += " !!!!!";
        if (output.contains("\n")) kind +=  " #br#";
        if (output.contains("\n")) report = "==== " + title + " ==== " + header + " ==== " + "\n\n" + report;
        if (output.contains("\n")) output.replace_all("\n", " ");
        result.report (report + " => " + output + " == " + title, kind);
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
