#pragma once
#include "6.h"
namespace pass6
{
    Pass <entry, entry> unlink = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0)
                logout("unlink", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                for (auto & line : content)
                {
                    if (line == "") continue;

                    bracketer b;
                    b.proceed_link = [&] (str s) { return pass5::unlink_(t, h, s, result, line); };
                    b.proceed(line);
            
                    line = std::move(b.output);
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
