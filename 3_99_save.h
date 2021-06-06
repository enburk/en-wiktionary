#pragma once
#include "3.h"
namespace pass3
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
                print("=== save 3... ===");
                fstream = std::ofstream(path_out);
            }

            if (GENERATE_REPORTS) fstream << entry;

            output.push(std::move(entry));
        }

        if (started)
        {
            fstream << esc;
            fstream.close();
            // only now (meta could be modified)
            std::filesystem::path path = path_out;
            path.replace_extension(".meta.txt");
            print("pass3 save ", path.string());
            save_meta(std::ofstream(path));
            print("=== save 3 ok ===");
        }
    };
}
