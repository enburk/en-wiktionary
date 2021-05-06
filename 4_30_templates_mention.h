#pragma once
#include "4.h"
namespace pass4
{
    str templates_mention_(str title, str header, str body, Result<entry> & result)
    {
        if (body == "QUOTE" || body == "rfdate" || body == "RQ") return "{{" + body + "}}";

        args args (body); str name = args.name; str arg = args.body;

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        if (name == "mention")
        {
            str a1, a2, a3;
            args.ignore("t"); args.ignore("tr"); args.ignore("gloss"); args.ignore("pos");
            if (args.lang != "") { args.unnamed.insert(0, args.lang); args.lang = ""; args.complexity++; }
            if (args.unnamed.size() > 1 && args[1] != "") a1 = "[[''"+args[1]+"'']]"; 
            if (args.unnamed.size() > 2 && args[2] != "") a2 = "[[''"+args[2]+"'']]"; 
            if (args.unnamed.size() > 3 && args[3] != "") a3 = "("+oquot+args[3]+cquot+")"; a3.replace_all("\n", " ");
            if (args.complexity == 1) { output = ""; kind += " 0"; report += " ==== " + title; } else
            if (args.complexity == 2) { output = a1; kind += " 1"; } else
            if (args.complexity == 3 && a1 == "" && a2 != "") { output = a2; kind += " 2-x"; } else
            if (args.complexity == 3 && a1 != "" && a2 == "") { output = a1; kind += " 2x-"; } else
            if (args.complexity == 3 && a1 != "" && a2 != "") { output = a2; kind += " 2xx"; } else
            if (args.complexity == 4 && a1 != "" && a2 == "" && a3 != "") { output = a1 + a3; kind += " 3x-x"; } else
            if (args.complexity == 4 && a1 != "" && a2 != "" && a3 != "") { output = a2 + a3; kind += " 3xxx"; } else
            if (args.complexity == 4 && a1 == "" && a2 != "" && a3 != "") { output = a2 + a3; kind += " 3-xx"; } else
            if (args.acquire("sc") != "") { kind += " sc"; report += " ==== " + title; }
            { kind += " quest"; report += " ==== " + title; }
        }
        else
        {
            kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }

        if (output.contains("\n")) kind +=  " #br#";
        if (output.contains("\n")) report = "==== " + title + " ==== " + header + " ==== " + "\n\n" + report;
        if (output.contains("\n")) output.replace_all("\n", " ");
        result.report (report, kind);
        return output;
    }

    Pass <entry, entry> templates_mention = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
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

        dump_templates_statistics(result);
    };
}
