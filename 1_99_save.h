#pragma once
#include "1.h"
namespace pass1
{
    Pass <entry, entry> save = [](auto & input, auto & output)
    {
        bool started = false;

        std::ofstream fstream;

        for (auto && entry : input)
        {
            if (GENERATE_REPORTS and not started)
            {
                started = true;
                print("=== save 1... ===");
                std::filesystem::path path = path_out;
                path.replace_extension(".meta.txt");
                print("pass1 save ", path.string());
                save_meta(std::ofstream(path));
                fstream = std::ofstream(path_out);
            }

            if (GENERATE_REPORTS) fstream << entry;

            output.push(std::move(entry));
        }

        if (started)
        {
            fstream << esc;
            fstream.close();
            print("=== save 1 ok ===");
        }
    };
}


