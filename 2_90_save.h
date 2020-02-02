#pragma once
#include "2.h"
namespace pass2
{
    auto path = "enwiktionary-pages-articles.txt";

    Pass <entry, entry> save = [](auto & input, auto & output)
    {
        std::ostringstream sstream; 

        for (auto && entry : input) sstream << std::move(entry);

        str articles = sstream.str(); if (articles != "")
        {
            print("=== save ===");
            std::ofstream fstream (path);

            for (auto [from, to] : redirect          ) fstream << from << " ====> " << to << "\n"; fstream << esc << "\n";
            for (auto [from, to] : redirect_templates) fstream << from << " ====> " << to << "\n"; fstream << esc << "\n";
            for (auto [from, to] : redirect_modules  ) fstream << from << " ====> " << to << "\n"; fstream << esc << "\n";

            for (auto [name, text] : Templates) fstream
                << esc << "\n"
                << ("Template:" + name) << "\n"
                << esc << "\n"
                << "==== header ==== " << "\n"
                << text << "\n";

            for (auto [name, text] : Modules  ) fstream
                << esc << "\n"
                << ("Module:"   + name) << "\n"
                << esc << "\n"
                << "==== header ==== " << "\n"
                << text << "\n";

            fstream << articles << esc;
        }
        try
        {
            print("=== read ===");
            std::ifstream fstream (path);
            str line; entry e;

            while (std::getline(fstream, line))
            {
                if (line == esc) break;
                str from, to; line.split_by(" ====> ", from, to);
                redirect.emplace(from, to);
            }
            while (std::getline(fstream, line))
            {
                if (line == esc) break;
                str from, to; line.split_by(" ====> ", from, to);
                redirect_templates.emplace(from, to);
            }
            while (std::getline(fstream, line))
            {
                if (line == esc) break;
                str from, to; line.split_by(" ====> ", from, to);
                redirect_modules.emplace(from, to);
            }
            while (std::getline(fstream, line))
            {
                if (line != "" &&
                    line.back() == '\r')
                    line.truncate();

                if (line == esc)
                {
                    if (!e.topic.empty())
                    {
                        if (e.title.starts_with("Template:")) Templates.emplace(e.title.from(9), e.topic[0].content); else
                        if (e.title.starts_with("Module:"  )) Modules  .emplace(e.title.from(7), e.topic[0].content); else
                        output.push(std::move(e));
                        e = entry{};
                    }
                }
                else
                if (line.starts_with("==== "))
                {
                    line = line.from(5);
                    e.topic += paragraph{};
                    line.split_by(" ==== ", e.topic.back().header, e.topic.back().forms);
                    continue;
                }
                else
                {
                    if (e.title == "") e.title = line; else
                    if (e.topic.size() == 0) continue; else
                        e.topic.back().content += line + "\n";
                }
            }
        }
        catch(std::exception & e) { print(str("exception: ") + e.what()); }
    };
}
