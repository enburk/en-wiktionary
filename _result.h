template <class Entry> struct Result
{
    std::filesystem::path dir;
    std::filesystem::path passpath; ioqueue<Entry> * output; bool on; Result (
    std::filesystem::path passpath, ioqueue<Entry> & output, bool on = true) :
    passpath (passpath), output (&output), on (on && GENERATE_REPORTS)
    {
        std::filesystem::path name = passpath.stem();
        if (name.empty()) throw std::logic_error("Result::Result: " + passpath.string());
        dir = passpath.parent_path() / name;
        if (std::filesystem::is_directory(dir) && on) { print("remove dir: ", dir);
            std::filesystem::remove_all(dir);
        }
    }

    struct outstream
    {
        std::stringstream sstream; std::ofstream fstream; int size = 0;
        outstream (std::filesystem::path path) : fstream (path) {}
        outstream & operator = (outstream &&) = default;
        outstream (outstream &&) = default;
        outstream () = default;
       ~outstream () { fstream << sstream.str(); }

        template <typename entry> // allow strings also
        outstream & operator << (entry && e) {
            size += e.size();
            sstream << e;
            if (size >= 8*512*1024) {
                fstream << sstream.str();
                sstream.str("");
                size = 0;
            }
            return *this;
        }
    };

    std::map<str, outstream> streams;

    outstream & stream (str kind, str filename)
    {
        filename.strip(); filename = filename == "" ? kind : kind + " " + filename;
        auto [it, inserted] = streams.emplace (filename, outstream{}); if (inserted)
        {
            std::filesystem::create_directories(dir);
            it->second = outstream{dir / (filename + ".txt")};
        }
        return it->second;
    }

    void accept (Entry entry, str filename = "", bool content = false)
    {
        entry.canonicalize();
        if (entry.topic.empty()) return;
        output->push (entry);
        if (on) stream ("accept", filename) << entry; if (content)
        if (on) stream ("accept", filename + " #") << entry.title << str("\n");
    }
    void reject (Entry entry, str filename = "", bool content = false)
    {
        entry.canonicalize();
        if (entry.topic.empty()) return;
        if (on) stream ("reject", filename) << entry; if (content)
        if (on) stream ("reject", filename + " #") << entry.title << str("\n");
    }
    void report (Entry entry, str filename = "", bool content = false)
    {
        entry.canonicalize();
        if (entry.topic.empty()) return;
        if (on) stream ("report", filename) << entry; if (content)
        if (on) stream ("report", filename + " #") << entry.title << str("\n");
    }

    void reject (str s, str filename = "") { if (on) if (s != "") stream ("reject", filename) << s << str("\n"); }
    void report (str s, str filename = "") { if (on) if (s != "") stream ("report", filename) << s << str("\n"); }

    // std::map<string, std::map<string, array<string>>> reports;
    // 
    // ~Result()
    // {
    //     for (const auto & [filename, entries] : reports) { auto & fs = stream ("report", filename);
    //     for (const auto & [title, topic] : entries) {
    //     for (const auto & s : topic) {
    //     }}}
    // }
};
