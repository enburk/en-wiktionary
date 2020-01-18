#pragma once
#include "0.h"
namespace pass0
{
    Pass<entry, entry> sort = [](auto & input, auto & output)
    {
        std::set<entry> entries;

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("sort ", nn, input.cargo);

            if (topic.starts_with("#REDIRECT")
            ||  topic.starts_with("#redirect")
            ||  title.starts_with("Template:")
            ||  title.starts_with("Module:" ))
            {
                title = " " + title;
            }

            entries.emplace(entry{std::move(title), std::move(topic)});
        }

        for (auto && entry : entries)
        {
            output.push(std::move(entry));
        }
    };
}


