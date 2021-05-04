#pragma once
#include "2.h"
namespace pass3
{
    using pass0::esc;
    using pass0::logout;

    struct paragraph
    {
        str header, forms; array<str> content;
    
        auto size () const { return header.size() + forms.size() + content.size(); }

        auto empty () const { return content.empty(); }

        void strip ()
        {
            for (auto & s : content) s.strip ();
            while (!content.empty() && content.back().empty()) content.pop_back();
            content.erase(content.begin(), std::find_if(content.begin(), content.end(), [](auto & p){ return !p.empty(); }));
            content.erase(std::unique(content.begin(), content.end(), [](auto & p1, auto & p2){ return p1.empty() && p2.empty(); }), content.end());
        }

        friend std::ostream & operator << (std::ostream & out, const paragraph & paragraph)
        {
            out << "==== "
                << paragraph.header  << " ==== "
                << paragraph.forms   << "\n" // std::endl
                << paragraph.content;
            return out;
        }
    };

    struct entry { str title; array<paragraph> topic;
    
        auto size () const { auto n = title.size(); for (const auto & p : topic) n += p.size(); return n; }

        auto empty () const { return topic.empty(); }

        void canonicalize ()
        {
            for (auto & p : topic) p.strip ();
            topic.erase_if([](auto & p){ return p.empty(); });
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