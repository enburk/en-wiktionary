#pragma once
#include "3.h"
namespace pass3
{
    std::map<str, std::map<str, int>> templates_statistics;

    str proceed_templates (str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; str arg = args.body;

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        if (false)
        {
        }
        else
        {
            kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }

        result.report (report, kind);
        return output;
    }

    Pass <entry, entry> templates = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};//UPDATING_REPORTS};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("tempLates", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                for (auto & line : content)
                {
                    bracketer b;
                    b.proceed_sbrakets  = [&] (str s) { return   "[" + s + "]"  ; };
                    b.proceed_qbrakets  = [&] (str s) { return   "{" + s + "}"  ; };
                    b.proceed_parameter = [&] (str s) { return "{{{" + s + "}}}"; };
                    b.proceed_link      = [&] (str s) { return  "[[" + s + "]]" ; };
                    b.proceed_template  = [&] (str s) { return proceed_templates (title, header, s, result); };
                    b.proceed(line);
            
                    if (b.report.size() > 0)
                    {
                        entry report;
                        report.title = title;
                        report.topic += paragraph {header, forms, content};
                        report.topic.back().content += "\n==============================================================\n";
                        report.topic.back().content += str(b.report);
                        result.report(report, "- broken brackets");
                    }
        
                    line = std::move(b.output);
                }
            }

            result.accept (entry {std::move(title), std::move(topic)});
        }

        std::multimap<int, str, std::greater<int>> templates; int total = 0;
        for (auto [name, n] : templates_statistics [__FILE__]) { templates.emplace(n, name); total += n; }
        for (auto [n, name] : templates) result.report (std::to_string(n) + " " + name, "# templates");
        result.report ("====================================", "# templates");
        result.report (std::to_string(total >> 00) + " total", "# templates");
        result.report (std::to_string(total >> 10) + " K    ", "# templates");
        result.report (std::to_string(total >> 20) + " M    ", "# templates");
    };
}
