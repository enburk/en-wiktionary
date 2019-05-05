namespace pass2
{
    inline const auto esc = pass1::esc;

    struct entry { str title; array<str> topic;
    
        auto size () { auto n = title.size(); for (const auto & s : topic) n += s.size(); return n; }

        auto empty () { return title.empty() && topic.empty(); }

        void canonicalize ()
        {
            for (auto & s : topic) s.strip ();
            while (topic.back() == "") topic.pop_back();
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

    #include "2_10_english.h"
}
