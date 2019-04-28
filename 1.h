namespace pass1
{
    inline const char esc [] =
    "#############################################################################################################################";

    struct entry { str title; str topic;
    
        auto size () { return title.size() + topic.size(); }

        auto empty () { return title.empty() && topic.empty(); }

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
//    #include "1_15_uncomment.h"

    Pass <entry, nothing> stop = [](auto & input, auto & output)
    {
        int64_t nn = 0;

        for (auto && e : input) { //; }
            if (++nn % 30'000 == 0) print("stop  ", nn, " entries ", input.cargo, " cargo ");
        }

        print("pass1::stop stop");
    };
}
