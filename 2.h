namespace pass2
{
    inline const char esc [] =
    "#############################################################################################################################";

    struct entry { str title; array<str> topic;
    
        auto size () { auto n = title.size(); for (const auto & s : topic) n += s.size(); return n; }

        friend std::ostream & operator << (std::ostream & out, entry && entry)
        {
            out << entry.title;
            out << esc; for (auto && s : entry.topic)
            out << s;
            return out;

            out << esc         << endl 
                << entry.title << endl
                << esc         << endl 
                << entry.topic << endl;
            return out;

        }
    };

    //using Pass = Pass<entry, entry>;

    #include "1_00_unzip.h"
    #include "1_10_unxml.h"
//    #include "1_15_uncomment.h"

    //PASS(stop)
    Pass<entry, entry> stop = [filename =__FILE__](auto & input, auto & output)
    {
        for (auto && i: input) { ; }

        cout << "stop" << endl;
    };
}
