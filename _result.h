template <class Entry> struct Result
{
    std::filesystem::path dir;
    std::filesystem::path filename; ioqueue<Entry> * output; bool reportage; Result (
    std::filesystem::path filename, ioqueue<Entry> & output, bool reportage = true) :
    filename(filename), output(&output), reportage(reportage && GENERATE_REPORTS)
    {
        std::filesystem::path name = filename.stem();
        if (name.empty()) throw std::logic_error("Result::Result: " + filename.string());
        dir = filename.parent_path() / name;
        if (std::filesystem::is_directory(dir) && reportage) { print("remove dir: ", dir);
            std::filesystem::remove_all(dir);
        }
    }

    std::map<str, std::ofstream> streams;

    std::ofstream & stream (str kind, str report_title)
    {
        str title = kind; report_title.strip(); if (report_title != "") title += " " + report_title;
        auto [it, inserted] = streams.emplace (title, std::ofstream{}); if (inserted) {
            std::filesystem::create_directories(dir);
            it->second = std::ofstream{dir / (title + ".txt")};
        }
        return it->second;
    }

    void accept (Entry entry, str report_title = "", bool content = false)
    {
        output->push (entry);
        if (reportage) stream("accept", report_title) << entry; if (content)
        if (reportage) stream("accept", report_title + " #") << entry.title << std::endl;
    }
    void reject (Entry entry, str report_title = "", bool content = false)
    {
        if (reportage) stream("reject", report_title) << entry; if (content)
        if (reportage) stream("reject", report_title + " #") << entry.title << std::endl;
    }
    void report (Entry entry, str report_title = "", bool content = false)
    {
        if (reportage) stream("report", report_title) << entry; if (content)
        if (reportage) stream("report", report_title + " #") << entry.title << std::endl;
    }

    void reject (str s, str report_title = "") {
        if (reportage) if (s != "") stream("reject", report_title) << s;
    }
    void report (str s, str report_title = "") {
        if (reportage) if (s != "") stream("report", report_title) << s;
    }
};
