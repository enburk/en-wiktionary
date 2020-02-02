#pragma once
#include "3.h"
namespace pass3
{
    Pass<pass2::entry, entry> paragraphs = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, false};

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

            result.accept (std::move(e));
        }
    };
}