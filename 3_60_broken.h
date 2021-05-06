#pragma once
#include "3.h"
namespace pass3
{
    Pass <entry, entry> broken = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
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
                  + header + " ==== " + "\n\n" + content + "\n\n"
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

                b.proceed(content);
                if (b.report.empty()) continue;
                result.report("==== " + title + " ==== "
                  + header + " ==== " + "\n\n" + content + "\n\n"
                  "!!!!!!!!!!!!!!!!!!!!" + str(b.report) + "\n",
                  "broken brackets 1");
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
