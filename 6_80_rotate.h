#pragma once
#include "6.h"
namespace pass6
{
    Pass<entry, entry> rotate = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            auto it = topic.end();

            for (auto i = topic.begin(); i != topic.end(); ++i)
            {
                if (i->header == "etymology") {
                    if (it != topic.end()) {
                        it  = topic.end();
                            break; }
                    else it = i;
                }
            }

            if (it != topic.end())
                std::rotate(it, it+1,
                    topic.end());

            it = topic.end();

            for (auto i = topic.begin(); i != topic.end(); ++i)
            {
                if (i->header == "pronunciation") {
                    if (it != topic.end()) {
                        it  = topic.end();
                            break; }
                    else it = i;
                }
            }

            if (it != topic.end())
                std::rotate(it, it+1,
                    topic.end());

            it = topic.end();

            for (auto i = topic.begin(); i != topic.end(); ++i)
            {
                if (i->header == "hyphenation") {
                    if (it != topic.end()) {
                        it  = topic.end();
                            break; }
                    else it = i;
                }
            }

            if (it != topic.end())
                std::rotate(it, it+1,
                    topic.end());

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}