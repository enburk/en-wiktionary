#pragma once
#include "4.h"
namespace pass4
{
    Pass<entry, entry> out = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, false};

        {
            std::ostringstream sstream; 

            for (auto && entry : input)
            {
                sstream << entry;
                result.accept(entry);
            }

            std::ofstream fstream ("enwiktionary-data.txt");

            fstream << sstream.str() << "END" << "\n";
        }
        {
            std::ostringstream sstream; 

            for (auto [title, forms] : lexforms)
            for (auto [form, ack, word] : forms)
            sstream << title << " == " << form << " == " << ack << " == " << word << "\n";

            std::ofstream fstream ("enwiktionary-forms.txt");

            fstream << sstream.str() << "END" << "\n";
        }
    };
}