#pragma once
#include "1.h"
namespace pass1
{
    Pass <pass0::entry, pass0::entry> load = [](auto & input, auto & output)
    {
        if (std::filesystem::exists(path_out)) return;

        bool started = false;

        for (auto && entry : input)
        {
            started = true;

            output.push(std::move(entry));
        }

        if (started) return;
            
        print("=== load 1... ===");

        try
        {
            str line; pass0::entry e;

            std::ifstream fstream (path_in);

            while (std::getline(fstream, line))
            {
                if (line != "" &&
                    line.back() == '\r')
                    line.truncate();

                if (line == esc)
                {
                    if (!e.topic.empty()) {
                        output.push(std::move(e));
                        e = pass0::entry{}; }
                }
                else
                {
                    if (e.title == "") e.title = line; else
                        e.topic += line + "\n";
                }
            }
        }
        catch(std::exception & e) {
            print(str("exception: ")
                + e.what()); }

        print("=== load 1 ok ===");
    };
}


