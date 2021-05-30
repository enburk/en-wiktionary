#pragma once
#include "3.h"
namespace pass3
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
        Result result {__FILE__, output};

        auto erase_tag = [&result](const str & title, str & Topic, str tag)
        {
            tag = tag.ascii_lowercased();
            str report = "- tag "+ tag;
            for (int z, c, e, a, b = 0; ; )
            {
                str topic = Topic.ascii_lowercased();

                auto ra = topic.find("<"+tag+">",  str::start_from(b)); a = ra.offset;
                auto rb = topic.find("<"+tag+" ",  str::start_from(b)); b = rb.offset; if (!ra && !rb) break;
                if (b > a) { b = a; rb = ra; }

                auto rz = topic.find(">",          str::start_from(b)); z = rz.offset;
                auto rc = topic.find("/>",         str::start_from(b)); c = rc.offset;
                auto re = topic.find("</"+tag+">", str::start_from(b)); e = re.offset; if (!rc && !re)
                {
                    report += " broken";
                    result.reject("\n#################################################################\n"
                        + title + "\n#################################################################\n"
                        + Topic + "\n=================================================================\n"
                        + Topic.from (b), report);
                    break;
                }
                if (e > c && c == z-1) { e = c; re = rc; }
                auto range = Topic.from(b).size(e-b+re.length);
                result.reject(range, report);
                range.erase();
            }
        };

        bool first_time = true;

        for (auto && [title, topic] : input)
        {
            if (first_time) {
                first_time = false;

                for (auto & [name, value] : Templates) {
                   erase_tag(name, value, "noinclude");
                }
            }

            for (auto & [header, forms, content] : topic)
            {
                erase_tag(title, content, "REF");
                erase_tag(title, content, "ref");
                erase_tag(title, content, "references");
                erase_tag(title, content, "gallery");

                for (int p = 0; ; )
                {
                    auto ra = content.find("<", str::start_from(p)); int a = ra.offset;
                    auto rb = content.find(">", str::start_from(p)); int b = rb.offset;
                    if ((!ra &&!rb)) { break; }
                    if ((!ra && rb) ||
                        ( ra &&!rb) || a > b) {
                        report_broken_tag(title, content, p, result);
                        break; }

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

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
