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

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("untag", nn, input.cargo);

            unsymbol (title, "symbols 1st pass title");
            unsymbol (topic, "symbols 1st pass topic");
            unsymbol (title, "symbols 2nd pass title");
            unsymbol (topic, "symbols 2nd pass topic");

            erase_all(title, topic, "<!--", "-->", "tag xml comments 0");
            erase_all(title, topic, "<!--", "->", "tag xml comments 1");

            erase_all(title, topic, "<ref>", "</ref>", "tag ref");

        //  erase_all(title, topic, "<noinclude>", "</noinclude>", "tag noinclude");

            result.accept (entry {std::move(title), std::move(topic)}, "", true);
        }

        for (auto & [rep, map] : symbols)
            for (auto & [symbol, nn] : map)
                result.report(symbol + " (" + std::to_string(nn) + ")", rep);
    };
}


