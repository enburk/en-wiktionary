#pragma once
#include "1.h"
namespace pass1
{
    Pass <entry, entry> save = [](auto & input, auto & output)
    {
        std::ostringstream sstream; 

        for (auto && entry : input)
        {
            sstream << entry; output.push(std::move(entry));
        }

        str articles = sstream.str();
        if (articles == "") return;

        print("=== save 1... ===");

        std::ofstream fstream (path_out);

        for (auto [from, to] : redirect          ) fstream << from << " ====> " << to << "\n"; fstream << esc << "\n";
        for (auto [from, to] : redirect_templates) fstream << from << " ====> " << to << "\n"; fstream << esc << "\n";
        for (auto [from, to] : redirect_modules  ) fstream << from << " ====> " << to << "\n"; fstream << esc << "\n";

        for (auto [name, text] : Templates) fstream
            << esc << "\n" << ("Template:" + name) << "\n"
            << esc << "\n" << "==== header ==== " << "\n"
            << text << "\n";

        for (auto [name, text] : Modules  ) fstream
            << esc << "\n" << ("Module:" + name) << "\n"
            << esc << "\n" << "==== header ==== " << "\n"
            << text << "\n";

        fstream << articles;
        fstream << esc;
        fstream.close();

        print("=== save 1 ok ===");
    };
}


