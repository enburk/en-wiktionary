#pragma once
#include "6.h"
namespace pass6
{
    Pass<entry, entry> out = [](auto & input, auto & output)
    {
        {
            std::ofstream fstream ("enwiktionary-data.txt");

            for (auto && entry : input)
            {
                fstream << entry;
            }

            fstream << "END" << "\n";
        }
        {
            std::ofstream fstream ("enwiktionary-forms.txt");

            for (auto [title, forms] : ::lexforms)
            for (auto [form, ack, word] : forms)
            fstream << title << " == " << form << " == " << ack << " == " << word << "\n";

            fstream << "END" << "\n";
        }
    };
}