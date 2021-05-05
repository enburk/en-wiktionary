#pragma once
#include "3.h"
namespace pass3
{
    str templates_(str title, str header, str body, Result<entry> & result, str o, str c)
    {
        const str preamble = "==== " + title + " ==== " + header + " ==== " + "\n\n";

        str output = o + body + c;
        str kind   = o + c;

        if (o == "{{")
        {
            args args (body); str name = args.name;
            templates_statistics [__FILE__][name]++;

            if (name == "examples") { // multiline
                result.reject(output,
                "{{examples}}");
                return "";
            }
        }

        result.report(output, kind);

        if (not output.contains("\n")) return output;

        kind +=  " #br#";
        result.report(preamble + output, kind);

        if (o == "{") {
            result.reject(output,
            "{tables}");
            return "";
        }

        output.replace_all("\r", "");
        output.replace_all("\n\n", "\n");
        output.replace_all("\n|", "|");
        output.replace_all("|\n", "|");

        if (not output.contains("\n")) return output;

        kind +=  " #br#";
        result.report(preamble + output, kind);
        output.replace_all("\n{", "{");
        output.replace_all("\n}", "}");
        output.replace_all("\n[", "[");
        output.replace_all("\n]", "]");
        output.replace_all("{\n", "{");
        output.replace_all("}\n", "}");
        output.replace_all("[\n", "[");
        output.replace_all("]\n", "]");

        if (not output.contains("\n")) return output;

        kind +=  " #br#";
        result.report(preamble + output, kind);
        output.replace_all("\n", " ");
        return output;
    }

    Pass <entry, entry> templates = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("templates", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                str kind;
                // many broken brackets
                if (related_items.contains(header)) {
                    int n = 0; for (char c : content) if (c == '\n') n++;
                    if (n >= 50) kind = "related items 50+"; else
                //  if (n >= 40) kind = "related items 40+"; else
                //  if (n >= 30) kind = "related items 30+"; else
                    {}
                }
                if (lexical_notes.contains(header)) {
                    int n = 0; for (char c : content) if (c == '\n') n++;
                    if (n >= 50) kind = "lexical notes 50+"; else
                    if (n >= 40) kind = "lexical notes 40+"; else
                //  if (n >= 30) kind = "lexical notes 30+"; else
                    {}
                }
                if (kind != "") {
                    result.reject("==== " + title + " ==== "
                    + header + " ==== " + "\n\n" + content);
                    content = "";
                    continue;
                }


                //content.replace_all("{{l|mul|]}}", "");
                //content.replace_all("{{l|mul|[}}", "");
                //content.replace_all("(3,4,3}", "{3,4,3}");
                //content.replace_all("", "");
                //content.replace_all("", "");
                //content.replace_all("", "");
                //content.replace_all("", "");
                //content.replace_all("", "");
                //content.replace_all("", "");
                //content.replace_all("", "");
                //content.replace_all("", "");
                //content.replace_all("", "");
                //content.replace_all("", "");
                //content.replace_all("", "");

                bracketer b;
                b.proceed_sbrakets  = [&] (str s) { return templates_(t, h, s, result,   "[", "]"  ); };
                b.proceed_qbrakets  = [&] (str s) { return templates_(t, h, s, result,   "{", "}"  ); };
                b.proceed_link      = [&] (str s) { return templates_(t, h, s, result,  "[[", "]]" ); };
                b.proceed_template  = [&] (str s) { return templates_(t, h, s, result,  "{{", "}}" ); };
                b.proceed_parameter = [&] (str s) { return templates_(t, h, s, result, "{{{", "}}}"); };
                b.proceed(content);
            
                if (b.report.size() > 0)
                {
                    entry report;
                    report.title = title;
                    report.topic += paragraph {header, forms, content};
                    report.topic.back().content += "\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
                    report.topic.back().content += str(b.report);
                    result.report(report, "# broken brackets");
                }
        
                content = std::move(b.output);
            }

            result.accept(entry{
                std::move(title),
                std::move(topic)});
        }

        dump_templates_statistics(result);
    };
}
