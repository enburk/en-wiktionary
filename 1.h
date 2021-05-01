#pragma once
#include "0.h"
namespace pass1
{
    using pass0::esc;
    using pass0::logout;

    auto path_in  = "data/enwiktionary-pages-articles.txt";
    auto path_out = "data/enwiktionary-step1.txt";

    struct entry { str title; array<str> topic;
    
        auto size () const
        {
            auto n = title.size();
            for (const auto & s : topic) n += s.size();
            return n;
        }

        auto empty () const { return title.empty() && topic.empty(); }

        void canonicalize () { strip(topic); }

        friend std::ostream & operator << (std::ostream & out, const entry & entry)
        {
            out << esc         << "\n"
                << entry.title << "\n"
                << esc         << "\n\n"
                << entry.topic << "\n";
            return out;
        }

		bool operator < (const entry & e) const { return title < e.title; }
    };
}
