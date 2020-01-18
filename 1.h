#pragma once
#include "0.h"
namespace pass1
{
    inline const auto esc = pass0::esc;

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
            out << esc         << std::endl 
                << entry.title << std::endl
                << esc         << std::endl << std::endl 
                << entry.topic << std::endl;
            return out;
        }
    };

    void logout (str pass, int64_t entries, int64_t cargo)
    {
        auto c = std::to_string(cargo);
        auto e = std::to_string(entries);
        e.insert(0, max(0, 7 - (int)e.size()), ' ');
        c.insert(0, max(0, 7 - (int)c.size()), ' ');
        print(pass, " ", e, " entries ", c, " cargo ");
    }
}
