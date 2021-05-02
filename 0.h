#pragma once
#include "main.hpp"
namespace pass0
{
    auto path_in  = "data/enwiktionary-pages-articles.xml";
    auto path_out = "data/enwiktionary-pages-articles.txt";

    struct entry { str title; str topic;
    
        auto size () { return title.size() + topic.size(); }

        auto empty () { return title.empty() && topic.empty(); }

        void canonicalize () {}

        friend std::ostream & operator << (std::ostream & out, const entry & entry)
        {
            out << esc         << "\n"
                << entry.title << "\n"
                << esc         << "\n"
                << entry.topic << "\n";
            return out;
        }

		bool operator < (const entry & e) const { return title < e.title; }
    };
}
