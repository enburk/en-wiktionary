template <class Entry1, class Entry2> struct Pass
{
    using iqueue = ioqueue<Entry1>;
    using oqueue = ioqueue<Entry2>;
    iqueue input; oqueue * output = nullptr;

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

Pass <nothing, nothing> terminator = [](auto & input, auto & output)
{
};
