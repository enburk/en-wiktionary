#pragma once
#include "4.h"
namespace pass4
{
    Pass<pass3::entry, entry> load = [](auto & input, auto & output)
    {
        bool live = false;

        for (auto && [title, topic] : input)
        {
            live = true;

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

        if (live or std::filesystem::exists(path_out)) return;

        print("=== load 4... ===");

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
                    if (not e.topic.empty()) {
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
                        e.topic.back().content += line;
                }
            }
        }
        catch(std::exception & e) {
            print(str("exception: ")
                + e.what()); }

        print("=== load 4 ok ===");
     };
}