#pragma once
#include "2.h"
namespace pass2
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

                print("=== save 2... ===");

                fstream = std::ofstream(path_out);
            }

            if (GENERATE_REPORTS) fstream << entry;

            output.push(std::move(entry));
        }

        if (started)
        {
            fstream << esc;
            fstream.close();

            std::filesystem::path path = path_out;
            path.replace_extension(".meta.txt");
            save_meta(std::ofstream(path));

            print("=== save 2 ok ===");
        }
    };
}
