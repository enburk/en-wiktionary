#pragma once
#include "0.h"
namespace pass0
{
    Pass<entry, entry> html = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

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
                    symbols[report + " changed"][it->first]++;
                    b += range.length;
                    continue;
                }

                if (e-b > 2 and s[b+1] == '#') try
                {
                    str so = s.from(b+1).size(e-b-1);
                    str sn = so.from(1);
                    std::size_t pos = 0; int base = 10;
                    if (sn.starts_with("X") or
                        sn.starts_with("x")) {
                        sn = sn.from(1);
                        base = 16;
                    }
                    auto n = std::stoul(sn, &pos, base);
                    if (pos != sn.size()) throw "bad symbols";
                    std::wstring_convert<
                    std::codecvt_utf8<char32_t>, char32_t> converter;
                    sn = converter.to_bytes(n);

                    auto range = s.from(b).size(e-b+1); range.replace_by(sn);
                    symbols[report + " changed+"][so + " => " + sn]++;
                    b += range.length;
                    continue;
                }
                catch(...) { symbols[report + " skipped+"][s.from(b).size(e-b)]++; }

                re = s.find(" " , str::start_from(b)); if (re) if (e > re.offset) e = re.offset;
                re = s.find("=" , str::start_from(b)); if (re) if (e > re.offset) e = re.offset;
                re = s.find("\n", str::start_from(b)); if (re) if (e > re.offset) e = re.offset;
                symbols[report + " skipped"][s.from(b).size(e-b)]++;
                b++;
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
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("html", nn, input.cargo);

            unsymbol (title, "symbols 1st pass title");
            unsymbol (topic, "symbols 1st pass topic");
            unsymbol (title, "symbols 2nd pass title");
            unsymbol (topic, "symbols 2nd pass topic");

            erase_all (title, topic, "<!--", "-->", "comments 0");
            erase_all (title, topic, "<!--", "->",  "comments 1");

            output.push(pass0::entry{
                std::move(title),
                std::move(topic)});
        }

        for (auto & [report, map] : symbols)
            for (auto & [symbol, nn] : map)
                result.report(symbol + " (" + std::to_string(nn) + ")",
                    report);
    };
}


