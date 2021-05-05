#pragma once
#include "3.h"
namespace pass3
{
    Pass <entry, entry> multiline1 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        auto erase_all = [&result](str & title, str & topic, str from, str to, str report)
        {
            for (int e, b = 0; ; )
            {
                auto rb = topic.find(from, str::start_from(b)); b = rb.offset; if (!rb) break;
                auto re = topic.find(to  , str::start_from(b)); e = re.offset; if (!re)
                {
                    report += " final";
                    e = topic.size();
                }
                auto range = topic.from(b).size(e-b);//+to.size());
                result.reject(range, report);
                range.erase();
            }
        };

        auto erase_tag = [&result](str & title, str & topic, str tag)
        {
            str report = "- tag "+ tag;
            for (int z, c, e, a, b = 0; ; )
            {
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
                        + topic + "\n=================================================================\n"
                        + topic.from (b), report);
                    break;
                }
                if (e > c && c == z-1) { e = c; re = rc; }
                auto range = topic.from(b).size(e-b+re.length);
                result.reject(range, report);
                range.erase();
            }
        };

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("multiline1", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                str paragraph = "==== " + title + " ==== "
                    + header + " ==== " + "\n\n" + content;

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
                    result.reject(paragraph, "- " + kind);
                    content = "";
                    continue;
                }

                bracketer b;
                b.proceed(content);
                if (b.report.empty()) continue;
                result.report("==== " + title + " ==== "
                  + header + " ==== " + "\n\n" + content + "\n" +
                  "!!!!!!!!!!!!!!!!!!!!" + str(b.report) + "\n",
                  "broken brackets 0");

                if (related_items.contains(header)) {
                    result.reject(paragraph, "- related items broken");
                    content = "";
                    continue;
                }
                if (lexical_notes.contains(header)) {
                    result.reject(paragraph, "- lexical notes broken");
                    content = "";
                    continue;
                }

                erase_tag (title, content, "REF");
                erase_tag (title, content, "ref");
                erase_tag (title, content, "references");
                erase_tag (title, content, "gallery");

                erase_all (title, content, "{{quote-", "\n#", "quote");
                erase_all (title, content, "{{cite-",  "\n#", "quote");

                b.proceed(content);
                if (b.report.empty()) continue;
                result.report("==== " + title + " ==== "
                  + header + " ==== " + "\n\n" + content +
                  "!!!!!!!!!!!!!!!!!!!!" + str(b.report) + "\n",
                  "broken brackets 1");


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
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
