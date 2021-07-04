#pragma once
#include "5.h"
namespace pass5
{
    str unlink2_(str title, str header, str body, Result<entry> & result, str line)
    {
        str kind   = "";
        str output = "[[" + body + "]]";
        str report = "[[" + body + "]]";

        auto ll = body.split_by("|");

        for (str & l : ll)
        {
            if (l.starts_with(":" )) l.upto(1).erase();
            if (l.starts_with("W:")) l.upto(2).erase();
            if (l.starts_with("w:")) l.upto(2).erase();
            if (l.starts_with("s:")) l.upto(2).erase();
            if (l.starts_with("Wikipedia:") or
                l.starts_with("wikipedia:"))
                l.upto(10).erase();
            if (l.starts_with("Citations:") or
                l.starts_with("citations:"))
                l.upto(10).erase();
            if (l.starts_with("Thesaurus:") or
                l.starts_with("thesaurus:"))
                l.upto(10).erase();
            if (l.starts_with("Appendix:") or
                l.starts_with("appendix:"))
                l.upto(9).erase();

            str anchor;
            l.split_by("#A", l, anchor);
            l.split_by("#E", l, anchor);
            l.split_by("#I", l, anchor);
            l.split_by("#O", l, anchor);
            l.split_by("#P", l, anchor);
            l.split_by("#U", l, anchor);
            l.split_by("##", l, anchor);
        }

        if (ll.size() == 1 and
           (ll[0].starts_with("https:") or
            ll[0].starts_with("http:"))) {
            kind = "http";
            output = "";
        }
        else
        if (ll.size() == 1 and
           (ll[0].starts_with("Category:") or
            ll[0].starts_with("category:"))) {
            kind = "category";
            output = "";
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
            kind = "[[ascii+ pipe]]";
            output = ll[1];
        }
        else
        if (ll.size() == 2)
        {
            str x0 = ll[0];
            str y0 = ll[1];
            str x1 = x0; x1.replace_all("\n", " "); x1.canonicalize();
            str y1 = y0; y1.replace_all("\n", " "); y1.canonicalize();
            str xl = x1; xl = x1.ascii_lowercased();
            str yl = y1; yl = y1.ascii_lowercased();
            str x2 = xl; x2.replace_all("'", ""); x2.replace_all("_", " ");
            str y2 = yl; y2.replace_all("'", ""); y2.replace_all(apostrophe, "");

            if(x0 == y0) kind = "[[ascii+pipe]] complexity 0"; else
            if(x1 == y1) kind = "[[ascii+pipe]] complexity 1"; else
            if(xl == yl) kind = "[[ascii+pipe]] complexity 2"; else
            if(x2 == y2) kind = "[[ascii+pipe]] complexity 3"; else
                         kind = "[[ascii+pipe]] complexity 4";

            output = "[["+y1+"]]";
        }
        else
            output = body;

        if (output.contains(":")) kind = "quest"; else
        if (report.contains("#")) kind += " complex2"; else
        if (report.contains(":")) kind += " complex1";

        result.report (report + " => " + output, kind);
        return output;
    }

    Pass <entry, entry> unlink2 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0)
                logout("unlink", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                for (auto & line : content)
                {
                    if (line == "") continue;

                    bracketer b;
                    b.proceed_link = [&] (str s) { return unlink2_(t, h, s, result, line); };
                    b.proceed(line);
            
                    line = std::move(b.output);

                    line.replace_all("_COLON_", ":");
                    line.replace_all("_COLONEQ_", ":=");
                    line.replace_all("_SMILE1_", ":)");
                    line.replace_all("_SMILE2_", ":-)");
                    line.replace_all("_PIPE_", "|");
                    line.replace_all("_EQUAL_", "=");
                    line.replace_all("_LCURLY_", "{");
                    line.replace_all("_RCURLY_", "}");
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
