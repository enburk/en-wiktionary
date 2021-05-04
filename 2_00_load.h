#pragma once
#include "2.h"
namespace pass2
{
    Pass<pass1::entry, entry> load = [](auto & input, auto & output)
    {
        bool started = false;

        for (auto && [title, topic] : input)
        {
            started = true;

            entry e; e.title = title; paragraph current;

            topic += "==== "; // ending

            for (str & s : topic)
            {
                if (s.starts_with ("==== "))
                {
                    current.strip(); if (!current.empty()) e.topic += std::move(current);
                    current = paragraph{};
                    current.header = s.from(5);
                    continue;
                }

                current.content += std::move(s);
                current.content += "\n";
            }

            output.push(std::move(e));
        }

        if (started) return;
            
        if (std::filesystem::exists(path_out)) return;

        print("=== load 2... ===");

        try
        {
            str line; entry e;

            std::filesystem::path path = path_in;
            path.replace_extension(".meta.txt");
            print("pass2 load ", path.string());
            load_meta(std::ifstream(path));

            std::ifstream fstream(path_in);

            while (std::getline(fstream, line))
            {
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
                    line.split_by(" ==== ",
                        e.topic.back().header,
                        e.topic.back().forms);
                    continue;
                }
                else
                {
                    if (e.title == "") e.title = line; else
                    if (e.topic.size() == 0) continue; else
                        e.topic.back().content +=
                        line + "\n";
                }
            }
        }
        catch(std::exception & e) {
            print(str("exception: ")
                + e.what()); }

        print("=== load 2 ok ===");
    };
}