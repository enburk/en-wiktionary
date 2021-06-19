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
        args args (body); str name = args.name; auto & a = args;
        args.languaged();

        name.replace_all(":", "..");
        name.replace_all("/", "~");

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        if (skipped_templates.contains(name)) return output;

        if (name == "topics")
        {
        }
        else
        if (name == "uxi")
        {
        }
        else
        if (name == "SI-unit-abb")
        {
        }
        else
        if (name == "name translit")
        {
        }
        else
        if (name == "rootsee")
        {
        }
        else
        if (name == "catlangname")
        {
        }
        else
        if (name == "suffixusex")
        {
        }
        else
        if (name == "attention")
        {
        }
        else
        if (name == "langname-mention")
        {
        }
        else
        if (name == "")
        {
        }
        else
        {
            kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }
        if (kind != "{{}}" and not
            templates_usage[__FILE__].contains(a.name)) {
            templates_usage[__FILE__].insert  (a.name);
            result.report(esc + "\n" + Templates[name]
                 + "\n" + esc + "\n", "{{"+name+"}}"); }

        if (kind.contains(" quest")) kind += " !!!!!";
        if (output.contains("\n")) kind +=  " #br#";
        if (output.contains("\n")) report = "==== "+title+" ==== "+header+" ==== "+"\n\n" + report;
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
