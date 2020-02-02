#pragma once
#include "2.h"
namespace pass2
{
    str proceed_unlink (str title, str header, str body, Result<entry> & result, str o, str c)
    {
        str kind   = o;
        str output = o + body + c;
        str report = o + body + c;

        if (o == "[[")
        {
            if (body.starts_with("File:" )) { output = ""; kind = "[[File]]"; } else
            if (body.starts_with("Image:")) { output = ""; kind = "[[Image]]"; } else
            {
                auto ll = body.split_by("|");

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

                    if(x0 == y0) kind = "[[==]] =="; else
                    if(x1 == y1) kind = "[[==]] ==="; else
                    if(xl == yl) kind = "[[==]] ===="; else
                    if(x2 == y2) kind = "[[==]] ====="; else
                        kind =  "[[==]] quest";
                    if (kind != "[[==]] quest") output = "[["+y1+"]]";
                }
            }
        }
        if (o == "{{")
        {
            args args (body);

            if (args.name == "link" && args.complexity == 1) { output = "[["+args[0]+"]]"; kind = "{{link}}"; } else
            if (args.name == "w"    && args.complexity == 1) { output = "[["+args[0]+"]]"; kind = "{{w}}"; } else
            {}
        }

        result.report (report, kind);
        return output;
    }

    Pass <entry, entry> unlink = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, UPDATING_REPORTS};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("unlink   ", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                bracketer b;
                b.proceed_sbrakets  = [&] (str s) { return   "[" + s + "]"  ; };
                b.proceed_qbrakets  = [&] (str s) { return   "{" + s + "}"  ; };
                b.proceed_parameter = [&] (str s) { return "{{{" + s + "}}}"; };
                b.proceed_link      = [&] (str s) { return proceed_unlink (title, header, s, result,  "[[", "]]" ); };
                b.proceed_template  = [&] (str s) { return proceed_unlink (title, header, s, result,  "{{", "}}" ); };
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
    };
}
