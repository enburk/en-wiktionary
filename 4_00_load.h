#pragma once
#include "4.h"
namespace pass4
{
    Pass<pass3::entry, entry> load = [](auto & input, auto & output)
    {
        for (auto && [title, topic] : input)
        {
            entry e; e.title = title;

            for (auto && [header, forms, content] : topic)
            {
                e.topic += paragraph{};
                e.topic.back().header  = header;
                e.topic.back().forms   = forms;
                e.topic.back().content = content.split();
            }

            output.push(std::move(e));
        }
    };
}