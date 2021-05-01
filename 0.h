#pragma once
#include "main.hpp"
namespace pass0
{
    auto path_in  = "data/enwiktionary-pages-articles.xml";
    auto path_out = "data/enwiktionary-pages-articles.txt";

    str esc = "###################################";

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

    void logout (str pass, int64_t entries, int64_t cargo)
    {
        auto c = std::to_string(cargo);
        auto e = std::to_string(entries);
        pass += str(' ', max(0, 9 - pass.size()));
        c.insert(0, max(0, 7 - (int)c.size()), ' ');
        e.insert(0, max(0, 7 - (int)e.size()), ' ');
        print(pass, " ", e, " entries ", c, " cargo ");
    }
}
