#pragma once
#include "4.h"
namespace pass4
{
    const array<str> skipped_templates = {
        "QUOTE", "rfdate", "RQ", // lex items
        "projectlink~Wikipedia", // see also
        "PL..species", // see also
        "seeSynonyms", // see also
        "etystub", // etymology
        "unknown", // etymology
    };

    str templates_z_(str title, str header, str body, Result<entry> & result)
    {
        args args (body, false); str name = args.name; str arg = args.body; auto & a = args;

        name.replace_all(":", "..");
        name.replace_all("/", "~");

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        if (body == "QUOTE" or
            body == "rfdate" or
            body == "RQ") return output;

        if (a.unnamed.size() > 0) { if (not Languages.contains(a[0])) kind += " quest lang";
            a.unnamed.erase(0);
            a.complexity--;
        }

        if (name == "zh-l")
        {
        }
        else
        if (name == "inflection of")
        {
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
        result.report(report + " => " + output + " == " + title, kind);
        return output;
    }

    Pass <entry, entry> templates_z = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                for (auto & line : content)
                {
                    bracketer b;
                    b.proceed_template = [&](str s){ return templates_z_(t, h, s, result); };
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
