#pragma once
#include "0.h"
namespace pass0
{
    Pass <entry, entry> save = [](auto & input, auto & output)
    {
        std::ofstream fstream;

        bool started = false;

        for (auto && entry : input)
        {
            if (not started)
            {
                started = true;

                print("=== save 0... ===");

                fstream = std::ofstream(path_out);
            }

            fstream << entry;

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


