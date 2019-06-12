namespace pass1
{
    inline const char esc [] = "#################################################################";

    struct entry { str title; str topic;
    
        auto size () { return title.size() + topic.size(); }

        auto empty () { return title.empty() && topic.empty(); }

        void canonicalize () {}

        friend std::ostream & operator << (std::ostream & out, const entry & entry)
        {
            out << esc         << std::endl 
                << entry.title << std::endl
                << esc         << std::endl 
                << entry.topic << std::endl;
            return out;
        }
    };

    #include "1_00_unzip.h"
    #include "1_10_unxml.h"
    #include "1_20_skip.h"
}
