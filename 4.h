#pragma once
#include "2.h"
namespace pass2
{
    auto path = "enwiktionary-pages-articles.txt";

    Pass <entry, entry> save = [](auto & input, auto & output)
    {
        std::ostringstream sstream; 

        for (auto && entry : input) sstream << std::move(entry);

        str s = sstream.str(); if (s != "")
        {
            print("=== save ===");
            std::ofstream fstream (path);
            fstream << s << esc;
        }
        {
            print("=== read ===");
            std::ifstream fstream (path);
            str line; entry e;

            while (std::getline(fstream, line))
            {
                if (line != "" &&
                    line.back() == '\r')
                    line.truncate();

                if (line == esc)
                {
                    if (!e.topic.empty())
                    {
                        output.push(std::move(e));
                        e = entry{};
                    }
                }
                else
                if (line.starts_with("==== "))
                {
                    line = line.from(5);
                    e.topic += paragraph{};
                    line.split_by(" ==== ", e.topic.back().header, e.topic.back().forms);
                    continue;
                }
                else
                {
                    if (e.title == "") e.title = line; else
                    if (e.topic.size() == 0) continue; else
                        e.topic.back().content += line + "\n";
                }
            }
        }        
    };
}
