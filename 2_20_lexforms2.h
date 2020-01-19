#pragma once
#include "2.h"
namespace pass2
{
    namespace lexforms2_internal
    {
        static std::unordered_map<str, str> list_of_forms =
        {
        };
    }

    str proceed_lexforms1 (str title, str header, str & forms, str body, Result<entry> & result)
    {
        str name; if (auto r = body.find('|'); r)
            name = body.upto(r.offset); else
            name = body; name.strip(" \t\n");

        auto it = lexforms2_internal::list_of_forms.find(name);
        if (it == lexforms2_internal::list_of_forms.end()) return "{{" + body + "}}";
        str lexical_item = it->second;
        str output = "{{" + body + "}}";
        str report = header;

        array<str> args = body.split_by("|"); for (auto & arg : args) arg.strip(" \t\n"); args.erase(args.begin());
        array<str> unnamed; std::map<str,str> opt;
        for (str arg : args) { str key, value;
            if (arg.split_by ("=", key, value)) {
                if (key == "head") continue;
                else opt [key] = value; }
            else unnamed += arg;
        }
        args = unnamed;

        if (header != lexical_item) report += " wrong"; else
        {}

        result.report (title + " ==== " + header + " ==== {{" + body + "}}", report);
        return output;
    }

    Pass<entry, entry> lexforms2 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("lexforms2", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                bracketer b;
                b.proceed_sbrakets  = [&] (str s) { return   "[" + s + "]"  ; };
                b.proceed_qbrakets  = [&] (str s) { return   "{" + s + "}"  ; };
                b.proceed_link      = [&] (str s) { return  "[[" + s + "]]" ; };
                b.proceed_parameter = [&] (str s) { return "{{{" + s + "}}}"; };
                b.proceed_template  = [&] (str s) { return proceed_lexforms2 (title, header, forms, s, result); };
                b.proceed(content);
            
                if (b.report.size() > 0)
                {
                    entry report;
                    report.title = title;
                    report.topic += paragraph {header, forms, content};
                    report.topic.back().content += "\n=================================================================\n";
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
