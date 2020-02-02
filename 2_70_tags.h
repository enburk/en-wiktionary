#pragma once
#include "2.h"
namespace pass2
{
    void report_broken_tag (str title, str topic, int p, Result<entry> & result)
    {
        result.report("\n#################################################################\n"
            + title + "\n#################################################################\n"
            + topic + "\n=================================================================\n"
            + topic.from (p), "# broken #");
    }

    Pass <entry, entry> tags = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, UPDATING_REPORTS};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("tags", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                for (int p = 0; ; )
                {
                    auto ra = content.find("<", str::start_from(p)); int a = ra.offset;
                    auto rb = content.find(">", str::start_from(p)); int b = rb.offset;
                    if (!ra &&!rb) { break; }
                    if (!ra && rb ||
                         ra &&!rb || a > b) { report_broken_tag(title, content, p, result); break; }

                    p = b+1;
                    str tag = content.from(a+1).upto(b);
                    str name, tail; tag.split_by(str::one_of(" /\n"), name, tail, str::delimiter::to_the_right);
                    name = name.ascii_lowercased();
                    str report =
                        name == "b" || name == "i" || name == "u" || name == "s" ? "b, i, u, s" : 
                        name == "sub" || name == "sup" || name == "big" || name == "small" ? "sub, sup, big, small" :
                        name == "kbd" || name == "hiero" || name == "tt" || name == "em" ? "tt, em, kbd, hiero" :
                        name;

                    if (tail.ends_with("/"))
                    {
                        str text = content.from(a).upto(p);
                        if (text.contains("\n")) report += " #br#";
                        result.report (text, report); continue;
                    }

                    auto rc = content.find("</"+name+">", str::start_from(b+1));
                    if (!rc) { report_broken_tag(title, content, a, result); break; }
                    p = rc.offset + rc.length;

                    str text = content.from(a).upto(p);
                    if (text.contains("\n"))
                    {
                        str combine; 
                        auto lines = text.split();
                        for (str line : lines) 
                        {
                            if (combine != ""
                            && !combine.ends_with("<br>")
                            && !combine.ends_with("<br/>")
                            && !combine.ends_with("<br />"))
                                combine += "<br>";
                                combine += line;
                        }

                        content.from(a).upto(p).replace_by(combine); p = a + combine.size();
                        result.report ("========== " + title + "\n" + text + "\n=====\n" + combine + "\n\n", report + " #br#");
                        continue;
                    }

                    result.report (text, report); continue;
                }
            }

            result.accept (entry {std::move(title), std::move(topic)});
        }
    };
}
