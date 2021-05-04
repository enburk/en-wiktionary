#pragma once
#include "3.h"
namespace pass3
{
    Pass<entry, entry> load = [](auto & input, auto & output)
    {
        bool started = false;

        for (auto && e : input)
        {
            started = true;

            output.push(std::move(e));
        }

        if (started) return;
            
        if (std::filesystem::exists(path_out)) return;

        print("=== load 3... ===");

        try
        {
            str line; entry e;

            std::filesystem::path path = path_in;
            path.replace_extension(".meta.txt");
            print("pass3 load ", path.string());
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

        print("=== load 3 ok ===");
    };
}