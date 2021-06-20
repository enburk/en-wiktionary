#pragma once
#include "4.h"
namespace pass4
{
    const array<str> skipped_templates = {
        "QUOTE", "rfdate", "RQ", // lex items
        "name translit", // name/surname translit senses
        "projectlink~Wikipedia", // see also
        "PL..species", // see also
        "seeSynonyms", // see also
        "etystub", // etymology
        "unknown", // etymology
        "zh-l", "zh-m",
        "ja-r", "ja-l",
        "ko-l", "he-m",
        "he-l", "ne-l",
        "vi-l",
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

        if (name == "nobr")
        {
        }
        else
        if (name == "B.C.E.")
        {
        }
        else
        if (name == "R..TPL")
        {
        }
        else
        if (name == "checksense")
        {
        }
        else
        if (name == "·")
        {
        }
        else
        if (name == "term-label")
        {
        }
        else
        if (name == "nb...")
        {
        }
        else
        if (name == "learned borrowing")
        {
        }
        else
        if (name == "translation only")
        {
        }
        else
        if (name == "century")
        {
        }
        else
        if (name == "comcatlite")
        {
        }
        else
        if (name == "PL..commons")
        {
        }
        else
        if (name == "ethnologue")
        {
        }
        else
        if (name == "C.E.")
        {
        }
        else
        if (name == "Latn-def")
        {
        }
        else
        if (name == "morse code for")
        {
        }
        else
        if (name == "U..en..plurals of letters")
        {
        }
        else
        if (name == "transliteration")
        {
        }
        else
        if (name == "col")
        {
        }
        else
        if (name == "")
        {
        }
        else
        if (name == "")
        {
        }
        else
        if (name == "")
        {
        }
        else
        if (name == "")
        {
        }
        else
        if (name == "")
        {
        }
        else
        if (name == "")
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
