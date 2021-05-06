#pragma once
#include "3.h"
namespace pass3
{
    str multiline_(str title, str header, str body, Result<entry> & result, str o, str c)
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

    Pass <entry, entry> multiline = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("multiline", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                bracketer b;
                b.proceed_sbrakets  = [&] (str s) { return multiline_(t, h, s, result,   "[", "]"  ); };
                b.proceed_qbrakets  = [&] (str s) { return multiline_(t, h, s, result,   "{", "}"  ); };
                b.proceed_link      = [&] (str s) { return multiline_(t, h, s, result,  "[[", "]]" ); };
                b.proceed_template  = [&] (str s) { return multiline_(t, h, s, result,  "{{", "}}" ); };
                b.proceed_parameter = [&] (str s) { return multiline_(t, h, s, result, "{{{", "}}}"); };
                b.proceed(content);
        
                content = std::move(b.output);
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
