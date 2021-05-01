template <class Entry1, class Entry2> struct Pass
{
    using iqueue = ioqueue<Entry1>; iqueue input;
    using oqueue = ioqueue<Entry2>; oqueue * output = nullptr;

    std::thread thread;
    std::function<void(iqueue&, oqueue&)> run;

    template <
    class Lambda>
    Pass (Lambda lambda) : run (lambda) {}
   ~Pass () { if (thread.joinable()) thread.join (); }

    template <
    typename E1,
    typename E2
    >
    Pass<E1, E2> & operator >> (Pass<E1, E2> & nextpass)
    {
        output = & nextpass.input;

        thread = std::thread ([](auto && f, iqueue & input, oqueue & output)
            {
                std::invoke (f, input, output);
                output.stop ();
            },
            run, std::ref(input), std::ref(*output));

        return nextpass;
    }
};

struct nothing {};

inline Pass <nothing, nothing> terminator = [](auto & input, auto & output)
{
};

void strip (array<str> & content)
{
    for (auto & s : content) s.strip (" \n");
    
    while (not content.empty() and content.back().empty()) content.pop_back();

    content.erase(content.begin(),
        std::find_if(content.begin(), content.end(),
            [](auto & s){ return not s.empty(); }));

    content.erase(std::unique(content.begin(), content.end(),
        [](auto & s1, auto & s2){ return s1.empty() && s2.empty(); }),
        content.end());
}

