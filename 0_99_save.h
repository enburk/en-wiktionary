#pragma once
#include "0.h"
namespace pass0
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

                print("=== save 0... ===");

                fstream = std::ofstream(path_out);
            }

            if (GENERATE_REPORTS) fstream << entry;

            output.push(std::move(entry));
        }

        if (started)
        {
            fstream << esc;
            fstream.close();

            print("=== save 0 ok ===");
        }
    };
}


