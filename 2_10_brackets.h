#pragma once
#include "2.h"
namespace pass2
{
    str brackets_(str title, str header, str body, Result<entry> & result, str o, str c)
    {
        str kind   = o;
        str output = o + body + c;
        str report = o + body + c;

        if (o == "[" && body.starts_with("http")) {
            kind = "[ http ]";
            output = "[http:]";
        }
        else
        if (o == "[[") // unlinking needed before lexforms
        {
            if (body.starts_with("File:" )) { output = ""; kind = "[[File]]"; } else
            if (body.starts_with("file:" )) { output = ""; kind = "[[File]]"; } else
            if (body.starts_with("Image:")) { output = ""; kind = "[[Image]]"; } else
            if (body.starts_with("image:")) { output = ""; kind = "[[Image]]"; } else
            if (lexical_items.contains(header) or
                lexical_notes.contains(header))
            {
                auto ll = body.split_by("|");

                if (ll.size() == 1
                &&  ll[0].ascii_isalnum()) {
                    kind = "[[ascii]]";
                    output = ll[0];
                }
                else
                if (ll.size() == 2 
                &&  ll[0].ascii_isalnum()
                &&  ll[1].ascii_isalnum()) {
                    kind = "[[ascii pipe]]";
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

                    if(x0 == y0) kind = "[[ascii+ pipe]] complexity 0"; else
                    if(x1 == y1) kind = "[[ascii+ pipe]] complexity 1"; else
                    if(xl == yl) kind = "[[ascii+ pipe]] complexity 2"; else
                    if(x2 == y2) kind = "[[ascii+ pipe]] complexity 3"; else
                                 kind = "[[ascii+ pipe]] complexity 4";

                    output = "[["+y1+"]]";
                }
            }
        }
        else
        if (o == "{{") // unlinking and preprocessing needed before lexforms
        {
            args args (body); str name = args.name;

            if (name == "head") { kind = "{{head}}"; output = ""; } else

            if (name == "PAGENAME") { kind = "{{preprocessor}}"; output = title; } else

            if (args.name == "1"    && args.complexity == 1) { output = "[["+args[0]+"]]"; kind = "{{1}}"; } else
            if (args.name == "link" && args.complexity == 1) { output = "[["+args[0]+"]]"; kind = "{{link}}"; } else
            if (args.name == "link" && args.complexity == 2) { output = "[["+args[1]+"]]"; kind = "{{link pipe}}"; } else
            if (args.name == "w"    && args.complexity == 2) { output = "[["+args[1]+"]]"; kind = "{{link pipe}}"; } else
            if (args.name == "w"    && args.complexity == 1) { output = "[["+args[0]+"]]"; kind = "{{w}}"; } else

            if (name.starts_with("quote-")) { kind = "{{quote quote-}}"; output = "{{QUOTE}}"; } else
            if (name.starts_with("cite-" )) { kind = "{{quote cite-}}";  output = "{{QUOTE}}"; } else
            if (name.starts_with("rfdate")) { kind = "{{quote rfdate}}"; output = "{{rfdate}}"; } else
            if (name.starts_with("RQ:"   )) { kind = "{{quote RQ}}";     output = "{{RQ}}"; } else
            {
                templates_statistics [__FILE__][name]++;
            }
        }

        result.report (report, kind);
        return output;
    }

    Pass <entry, entry> brackets = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0)
                logout("brackets", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                bracketer b;
                b.proceed_sbrakets  = [&] (str s) { return brackets_(t, h, s, result,   "[", "]"  ); };
                b.proceed_qbrakets  = [&] (str s) { return brackets_(t, h, s, result,   "{", "}"  ); };
                b.proceed_link      = [&] (str s) { return brackets_(t, h, s, result,  "[[", "]]" ); };
                b.proceed_template  = [&] (str s) { return brackets_(t, h, s, result,  "{{", "}}" ); };
                b.proceed_parameter = [&] (str s) { return brackets_(t, h, s, result, "{{{", "}}}"); };
                b.proceed(content);
            
                if (b.report.size() > 0)
                {
                    entry report;
                    report.title = title;
                    report.topic += paragraph {header, forms, content};
                    report.topic.back().content += "\n\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
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
