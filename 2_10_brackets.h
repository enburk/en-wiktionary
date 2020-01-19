#pragma once
#include "2.h"
namespace pass2
{
    std::map<str, std::map<str, int>> templates_statistics;

    str proceed_brackets (str title, str header, str body, Result<entry> & result, str o, str c)
    {
        str kind   = o;
        str output = o + body + c;
        str report = o + body + c;

        if (o == "[[")
        {
            if (lexical_items.contains(header) && body.ascii_isalnum())
            {
                kind = "[[ ascii ]]"; output = body;
            }
        }
        if (o == "{{")
        {
            str name; if (auto r = body.find('|'); r)
                name = body.upto(r.offset); else
                name = body; name.strip(" \t\n");

            if (name == "head") { kind = "{{ head }}"; output = ""; } else

            if (name.starts_with("quote-")) { kind = "{{ quote quote- }}"; output = "{{QUOTE}}"; } else
            if (name.starts_with("cite-" )) { kind = "{{ quote cite-  }}"; output = "{{QUOTE}}"; } else
            {
                templates_statistics [__FILE__][name]++;
            }
        }

        if (output.contains("\n")) report = "==== " + title + " ==== " + header + " ==== " + "\n\n" + report;
        if (output.contains("\n")) output.replace_all("\n", " ");
        result.report (report, kind);
        return output;
    }

    Pass <entry, entry> brackets = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, UPDATING_REPORTS};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("brackets ", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                bracketer b;
                b.proceed_sbrakets  = [&] (str s) { return proceed_brackets (title, header, s, result,   "[", "]"  ); };
                b.proceed_qbrakets  = [&] (str s) { return proceed_brackets (title, header, s, result,   "{", "}"  ); };
                b.proceed_link      = [&] (str s) { return proceed_brackets (title, header, s, result,  "[[", "]]" ); };
                b.proceed_template  = [&] (str s) { return proceed_brackets (title, header, s, result,  "{{", "}}" ); };
                b.proceed_parameter = [&] (str s) { return proceed_brackets (title, header, s, result, "{{{", "}}}"); };
                b.proceed(content);
            
                if (b.report.size() > 0)
                {
                    entry report;
                    report.title = title;
                    report.topic += paragraph {header, forms, content};
                    report.topic.back().content += "\n==============================================================\n";
                    report.topic.back().content += str(b.report);
                    result.report(report, "- broken brackets");
                }
        
                content = std::move(b.output);
            }

            result.accept (entry {std::move(title), std::move(topic)});
        }

        std::multimap<int, str, std::greater<int>> templates; int total = 0;
        for (auto [name, n] : templates_statistics [__FILE__]) { templates.emplace(n, name); total += n; }
        for (auto [n, name] : templates) result.report (std::to_string(n) + " " + name, "templates");
        result.report ("====================================", "templates");
        result.report (std::to_string(total >> 00) + " total", "templates");
        result.report (std::to_string(total >> 10) + " K    ", "templates");
        result.report (std::to_string(total >> 20) + " M    ", "templates");
    };
}
