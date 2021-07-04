#pragma once
#include "5.h"
namespace pass5
{
    str unlink1_(str title, str header, str body, Result<entry> & result, str line)
    {
        str kind   = "";
        str output = "[[" + body + "]]";
        str report = "[[" + body + "]]";

        auto ll = body.split_by("|");

        for (str & l : ll)
        {
            str anchor;
            l.split_by("#A", l, anchor);
            l.split_by("#E", l, anchor);
            l.split_by("#D", l, anchor);
            l.split_by("#I", l, anchor);
            l.split_by("#M", l, anchor);
            l.split_by("#N", l, anchor);
            l.split_by("#O", l, anchor);
            l.split_by("#P", l, anchor);
            l.split_by("#S", l, anchor);
            l.split_by("#U", l, anchor);
            l.split_by("#V", l, anchor);
            l.split_by("##", l, anchor);
        }

        if (ll.size() == 1 and
           (ll[0].starts_with("https:") or
            ll[0].starts_with("http:"))) {
            kind = "http";
            output = "[http:]";
        }
        else
        if (ll.size() == 1
        and ll[0].contains_only(str::one_of(alnum))) {
            kind = "[[ascii]]";
            output = ll[0];
        }
        else
        if (ll.size() == 1
        and ll[0].contains_only(str::one_of(ALNUM))) {
            kind = "[[ascii+]]";
            output = ll[0];
        }
        else
        if (ll.size() == 2 
        and ll[0].contains_only(str::one_of(ALNUM))
        and ll[1].contains_only(str::one_of(ALNUM))) {
            kind = "[[ascii+pipe]]";
            output = ll[1];
        }
        else kind = "quest";

        if (report.contains("#")) kind += " anchor";

        result.report (report + " => " + output, kind);
        return output;
    }

    Pass <entry, entry> unlink1 = [](auto & input, auto & output)
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
                    if (line == "") continue;

                    bracketer b;
                    b.proceed_link = [&] (str s) { return unlink1_(t, h, s, result, line); };
                    b.proceed(line);
            
                    line = std::move(b.output);
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
