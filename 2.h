#pragma once
#include "1.h"
namespace pass2
{
    inline const auto esc = pass1::esc;

    struct paragraph
    {
        str header, forms, content;
    
        auto size () const { return header.size() + forms.size() + content.size(); }

        auto empty () const { return content.empty(); }

        void strip () { content.strip (" \n"); }

        friend std::ostream & operator << (std::ostream & out, const paragraph & paragraph)
        {
            out << "==== "
                << paragraph.header  << " ==== "
                << paragraph.forms   << std::endl
                << paragraph.content << std::endl;
            return out;
        }
    };

    struct entry { str title; array<paragraph> topic;
    
        auto size () const { auto n = title.size(); for (const auto & p : topic) n += p.size(); return n; }

        auto empty () const { return topic.empty(); }

        void canonicalize ()
        {
            for (auto & s : topic) s.strip ();
            while (!topic.empty() && topic.back().empty()) topic.pop_back();
            topic.erase(topic.begin(), std::find_if(topic.begin(), topic.end(), [](auto & p){ return !p.empty(); }));
            topic.erase(std::unique(topic.begin(), topic.end(), [](auto & p1, auto & p2){ return p1.empty() && p2.empty(); }), topic.end());
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
