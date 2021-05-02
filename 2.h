#pragma once
#include "1.h"
namespace pass2
{
    auto path_in  = "data/enwiktionary-step1.txt";
    auto path_out = "data/enwiktionary-step2.txt";

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
                << paragraph.forms   << "\n"
                << paragraph.content << "\n";
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
            topic.erase_if([](auto & p){
                return p.empty(); });
        }

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
