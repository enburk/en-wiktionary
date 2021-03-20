#pragma once
#include "0.h"
namespace pass1
{
    using pass0::esc;
    using pass0::logout;

    struct entry { str title; array<str> topic;
    
        auto size () const { auto n = title.size(); for (const auto & s : topic) n += s.size(); return n; }

        auto empty () const { return title.empty() && topic.empty(); }

        void canonicalize ()
        {
            for (auto & s : topic) s.strip ();
            while (!topic.empty() && topic.back() == "") topic.pop_back();
            topic.erase(topic. begin(), std::find_if(topic. begin(), topic.end (), [](auto & s){ return s != ""; }));
            topic.erase(std::unique(topic.begin(), topic.end(), [](auto & s1, auto & s2){ return s1 == "" && s2 == ""; }), topic.end());
        }

        friend std::ostream & operator << (std::ostream & out, const entry & entry)
        {
            out << esc         << "\n"   // std::endl 
                << entry.title << "\n"   // std::endl
                << esc         << "\n\n" // std::endl << std::endl 
                << entry.topic << "\n";  // std::endl;
            return out;
        }
    };
}
