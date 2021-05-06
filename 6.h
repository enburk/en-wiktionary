#pragma once
#include "5.h"
namespace pass6
{
    struct paragraph
    {
        str header, forms; array<str> content;
    
        auto size () const { return header.size() + forms.size() + content.size(); }

        auto empty () const { return content.empty(); }

        void strip () { ::strip(content); }

        friend std::ostream & operator << (std::ostream & out, const paragraph & p)
        {
            out << "  == " << p.header  << " == " << p.forms << "\n"; for (auto & s : p.content)
            out << "     # " << s << std::endl;
            return out;
        }
    };

    struct entry { str title; array<paragraph> topic;
    
        auto size () const {
            auto n = title.size();
            for (const auto & p : topic) n += p.size();
            return n;
        }

        auto empty () const { return topic.empty(); }

        void canonicalize ()
        {
            for (auto & p : topic) p.strip ();
            topic.erase_if([](auto & p){ return p.empty(); });
        }

        friend std::ostream & operator << (std::ostream & out, const entry & entry)
        {
            out << entry.title << "\n" << entry.topic;
            return out;
        }
    };
}
