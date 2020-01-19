#pragma once
#include "0.h"
namespace pass0
{
    Pass<str, entry> unxml = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, UPDATING_REPORTS};

        const std::vector<str> tagmap = 
        {
            "<page>",
                "<title>",
                // title here
                "</title>",
                "<ns>0</ns>",
                "<revision>",
                    "<contributor>",
                        "<username>MewBot</username>", "<id>345682</id>",
                        "<username>NadandoBot</username>", "<id>394541</id>",
                        "<username>TheDaveBot</username>", "<id>20393</id>",
                    "</contributor>",
                    "<minor />",
                    "<model>wikitext</model>",
                    "<format>text/x-wiki</format>",
                    "<text>xml:space=\"preserve\"</text>",
                    "<text xml:space=\"preserve\">",
                    // topic there
                    "</text>",
                "</revision>",
            "</page>"
        };

        const str ss [] = { "<page>", "<title>", "</title>", "<text", "preserve", ">", "<"};

        const auto searcher1 = std::boyer_moore_horspool_searcher(ss[1].begin(), ss[1].end());
        const auto searcher2 = std::boyer_moore_horspool_searcher(ss[2].begin(), ss[2].end());
        const auto searcher3 = std::boyer_moore_horspool_searcher(ss[3].begin(), ss[3].end());
        const auto searcher4 = std::boyer_moore_horspool_searcher(ss[4].begin(), ss[4].end());

        using I = str::const_iterator;

        auto find1 = [searcher1](I f, I l) { return std::search (f, l, searcher1); };
        auto find2 = [searcher2](I f, I l) { return std::search (f, l, searcher2); };
        auto find3 = [searcher3](I f, I l) { return std::search (f, l, searcher3); };
        auto find4 = [searcher4](I f, I l) { return std::search (f, l, searcher4); };
        auto find5 = [         ](I f, I l) { return std::find   (f, l, '>'); };
        auto find6 = [         ](I f, I l) { return std::find   (f, l, '<'); };

        str title;
        str topic;
        int mode = 1;

        static int64_t chunks = 0;

        str s; s.reserve(16*1024*1024);
        I i = s.begin();
        I j = s.begin(); 

        while (true)
        {
            I found = 
            mode == 1 ? find1(j, s.end()):
            mode == 2 ? find2(j, s.end()):
            mode == 3 ? find3(j, s.end()):
            mode == 4 ? find4(j, s.end()):
            mode == 5 ? find5(j, s.end()):
            mode == 6 ? find6(j, s.end()): s.end();

            if (found == s.end())
            {
                j = s.end() - (ss[mode].size() - 1);

                auto offset = i - s.begin();
            
                if (offset > (int) s.capacity() / 2)
                {
                    s.upto((int) offset).erase(); offset = 0;
                    j = s.begin() + offset + (j - i);
                    i = s.begin() + offset;
                }

                str next; if (!input.pop(next)) break; s += next;
                j = s.begin() + offset + (j - i);
                i = s.begin() + offset;
                chunks++;
                continue;
            }

            if (mode != 2 && mode != 6) found += ss[mode].size();

            switch (mode) {
            case 1: result.reject(i, found); i = j = found; mode = 2; break; // "<title>"
            case 2: title = str  (i, found); i = j = found; mode = 3; break; // "</title>"
            case 3: result.reject(i, found); i = j = found; mode = 4; break; // "<text"
            case 4: result.reject(i, found); i = j = found; mode = 5; break; // "preserve"
            case 5: result.reject(i, found); i = j = found; mode = 6; break; // ">"
            case 6: topic = str  (i, found); i = j = found; mode = 1; break; // "<"
            }

            if (mode == 1 && title.size () > 0)
            {
                static int64_t nn = 0; if (++nn % 500'000 == 0) logout("unxml", nn, input.cargo);

                result.accept (entry {std::move(title), std::move(topic)}, "", true);
                title = "";
                topic = "";
            }
        }

        result.reject(i, s.end());

        print("unxml ", chunks, " chunks ");
    };
}


