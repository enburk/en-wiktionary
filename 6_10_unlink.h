#pragma once
#include "6.h"
namespace pass6
{
    str unlink_(str title, str header, str body, Result<entry> & result, str line)
    {
        str kind   = "";
        str output = "[[" + body + "]]";
        str report = "[[" + body + "]]";

        auto ll = body.split_by("|");

        if (ll.size() == 1
        &&  ll[0].contains_only(str::one_of(alnum))) {
            kind = "[[ascii]]";
            output = ll[0];
        }
        else
        if (ll.size() == 1
        &&  ll[0].contains_only(str::one_of(ALNUM))) {
            kind = "[[ascii+]]";
            output = ll[0];
        }
        else
        if (ll.size() == 2 
        &&  ll[0].contains_only(str::one_of(ALNUM))
        &&  ll[1].contains_only(str::one_of(ALNUM))) {
            kind = "[[ascii+ pipe]]";
            output = ll[1];
        }
        else
        if (ll.size() == 2)
        {
            if (ll[0].starts_with("wikipedia:")) ll[0].upto(10).erase();
            if (ll[0].starts_with("W:"        )) ll[0].upto( 2).erase();
            if (ll[0].starts_with("w:"        )) ll[0].upto( 2).erase();
            if (ll[0].starts_with("s:"        )) ll[0].upto( 2).erase();

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

        result.report (report, kind);
        return output;
    }

    Pass <entry, entry> unlink = [](auto & input, auto & output)
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
                    b.proceed_link = [&] (str s) { return unlink_(t, h, s, result, line); };
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
