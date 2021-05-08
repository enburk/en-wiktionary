#pragma once
#include "6.h"
namespace pass6
{
    Pass<pass5::entry, entry> load = [](auto & input, auto & output)
    {
        for (auto && [title, topic] : input)
        {
            entry e; e.title = title;

            e.topic.reserve(topic.size());

            for (auto && [header, forms, content] : topic)
            {
                e.topic += paragraph{};
                e.topic.back().header  = std::move(header);
                e.topic.back().forms   = std::move(forms);
                e.topic.back().content = std::move(content);
            }

            output.push(std::move(e));
        }
    };
}