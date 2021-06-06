#pragma once
#include "3.h"
namespace pass4
{
    auto path_in  = "data/enwiktionary-step3.txt";
    auto path_out = "data/enwiktionary-step4.txt";

    struct paragraph
    {
        str header, forms; array<str> content;
    
        auto size () const { return header.size() + forms.size() + content.size(); }

        auto empty () const { return content.empty(); }

        void strip () { ::strip(content); }

        friend std::ostream & operator << (std::ostream & out, const paragraph & paragraph)
        {
            out << "==== "
                << paragraph.header  << " ==== "
                << paragraph.forms   << "\n"
                << paragraph.content;
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
            out << esc         << "\n"
                << entry.title << "\n"
                << esc         << "\n\n"
                << entry.topic << "\n";
            return out;
        }
    };
}
