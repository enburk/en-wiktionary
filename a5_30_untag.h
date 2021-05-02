#pragma once
#include "0.h"
namespace pass0
{
    Pass<entry, entry> untag = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, UPDATING_REPORTS};

        std::map<str, std::map<str, int>> symbols;

        auto unsymbol = [&symbols](str & s, str report)
        {
            for (int e, b = 0; ; )
            {
                auto rb = s.find("&", str::start_from(b)); b = rb.offset; if (!rb) break;
                auto re = s.find(";", str::start_from(b)); e = re.offset; if (!re) { b++; continue; }

                auto it = Symbols.find(s.from(b+1).size(e-b-1));
                if (it != Symbols.end())
                {
                    auto range = s.from(b).size(e-b+1);
                    range.replace_by(str((const char *)(it->second)));
                    symbols [report + " changed"][it->first]++;
                    b += range.length;
                }
                else
                {
                    re = s.find(" " , str::start_from(b)); if (re) if (e > re.offset) e = re.offset;
                    re = s.find("=" , str::start_from(b)); if (re) if (e > re.offset) e = re.offset;
                    re = s.find("\n", str::start_from(b)); if (re) if (e > re.offset) e = re.offset;
                    symbols [report + " skipped"][s.from(b).size(e-b)]++;
                    b++;
                }
            }
        };

        auto erase_all = [&result](str & title, str & topic, str from, str to, str report)
        {
            for (int e, b = 0; ; )
            {
                auto rb = topic.find(from, str::start_from(b)); b = rb.offset; if (!rb) break;
                auto re = topic.find(to  , str::start_from(b)); e = re.offset; if (!re)
                {
                    report += b + from.size() == topic.size() ? " final" : " broken";
                    result.reject("\n#################################################################\n"
                        + title + "\n#################################################################\n"
                        + topic + "\n=================================================================\n"
                        + topic.from (b), report);
                    break;
                }
                auto range = topic.from(b).size(e-b+to.size());
                result.reject(range, report);
                range.erase();
            }
        };

        auto erase_tag = [&result](str & title, str & topic, str tag)
        {
            str report = "tag "+ tag;
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
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("untag", nn, input.cargo);

            unsymbol (title, "symbols 1st pass title");
            unsymbol (topic, "symbols 1st pass topic");
            unsymbol (title, "symbols 2nd pass title");
            unsymbol (topic, "symbols 2nd pass topic");

            erase_all (title, topic, "<!--", "-->", "xml comments 0");
            erase_all (title, topic, "<!--", "->",  "xml comments 1");

            erase_tag (title, topic, "REF");
            erase_tag (title, topic, "ref");
            erase_tag (title, topic, "references");
            erase_tag (title, topic, "gallery");

        //  erase_tag (title, topic, "noinclude"); // parted by templates

            result.accept (entry {std::move(title), std::move(topic)}, "", true);
        }

        for (auto & [rep, map] : symbols)
            for (auto & [symbol, nn] : map)
                result.report(symbol + " (" + std::to_string(nn) + ")", rep);
    };
}


