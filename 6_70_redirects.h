#pragma once
#include "6.h"
namespace pass6
{
    const std::set<str> list_of_forms =
    {
        "Alternative form of"                                    ,
        "Alternative letter-case of"                             ,
        "Alternative name of"                                    ,
        "Alternative spelling of"                                ,
        "Alternative capitalization of"                          ,
        "comparative form of"                                    ,
        "past participle of"                                     ,
        "plural of"                                              ,
        "plural form of"                                         ,
        "present participle of"                                  ,
        "simple past of"                                         ,
        "simple past tense of"                                   ,
        "simple past tense and past participle of"               ,
        "superlative form of"                                    ,
        "third-person singular simple present indicative form of",
    };

    Pass <entry, entry> redirects = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        std::map<str, array<paragraph>> entries;

        for (auto && entry : input)
            entries.emplace(
                std::move(entry.title),
                std::move(entry.topic));
        
        print("=== ", entries.size(), " entries");

        std::map<str, str> redirects;

        for (auto & [title, topic] : entries)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("redirects", nn, input.cargo);

            bool complex = false;
            bool notfound = false;
            str report, origin;

            for (auto & [header, forms, content] : topic)
            {
                if (not lexical_items.contains(header))
                {
                    if (header != "pronunciation" and
                        header != "hyphenation")
                        complex = true;

                    continue;
                }

                for (auto & line : content)
                {
                    if (line == "") continue;

                    bool not_a_form = true;

                    for (auto & form : list_of_forms)
                    {
                        if (line.starts_with("# ''" + form + "''"))
                        {
                            not_a_form = false;

                            str s = line.from(form.size());
                            str x; s.split_by("'''", x, s);
                            str y; s.split_by("'''", s, y);
                            s.strip();

                            if (s == "") {
                                result.report(line, "- empty target");
                                complex = true;
                                break;
                            }

                            if (origin == "") origin = s; else
                            if (origin != s ) complex = true;

                            bool found = false;
                            auto it = ::lexforms.find(s);
                            if (it != ::lexforms.end())
                            {
                                for (auto & [form_, ack, word] : it->second)
                                {
                                    if (form == form_ and word == title and
                                        (ack == "?" or ack == "+")) {
                                        report = report == "" ? form : "x2";
                                        found = true; ack = "+";
                                        break;
                                    }
                                }
                            }
                            if (not found)
                            {
                                complex = true;
                                notfound = true;
                                result.report(form + " " + s + " = " + title,
                                    " - not found");
                            }
                        }
                    }

                    if (not_a_form) complex = true;
                }
            }

            if (not complex)
            {
                redirects[title] = origin;
                std::ostringstream sstream; sstream << entry{title, topic};
                result.report (sstream.str() + ">>>>> " + origin + " <<<<<\n\n", report);
            }
            if (complex and origin != "" and notfound)
            {
                std::ostringstream sstream; sstream << entry{title, topic};
                result.report (sstream.str() + ">>>>> " + origin + " <<<<<\n\n", "--");
            }
            if (complex and origin != "" and not notfound)
            {
                std::ostringstream sstream; sstream << entry{title, topic};
                result.report (sstream.str() + ">>>>> " + origin + " <<<<<\n\n", "-");
            }
        }

        for (auto [title, forms] : lexforms)
        for (auto [form, ack, word] : forms)
        {
            str report = "";
        
            if (ack == "+")
                report = "302 found";
            else
            if (!entries.contains(title)
            ||  !entries.contains(word))
                report = "501 not implemented";
            else
            if (!word.starts_with("more ")
            &&  !word.starts_with("most "))
                report = "404 not found";
        
            if (report != "") result.report (word + " == " +
                form + " == " + title + " == " + ack, report);
        }

        std::erase_if(entries,
            [redirects](auto const& e){
                auto const& [title, topic] = e;
                return redirects.contains(title); });

        print("=== ", entries.size(), " orginal");

        std::map<str, str> good, miss;

        for (auto [from, to] : redirects)
            if (entries.contains(to))
                good[from] = to; else
                miss[from] = to;

        while (true)
        {
            std::map<str, str> next;

            for (auto [from, to] : miss)
                if (good.contains(to))
                    good[from] = good[to]; else
                    next[from] = to;

            if (next.size() ==
                miss.size()) break;

            miss = next;
        }

        for (auto [from, to] : miss)
            result.report(from + " => " + to,
                "- unresolved");

        print("=== ", good.size(), " pure forms");

        for (auto [from, to] : ::redirect)
        {
            if (entries.contains(to) and
            not entries.contains(from) and
            not good   .contains(from)) {
                good[from] = to;
                str report;
                if (auto it = ::lexforms.find(to); it != ::lexforms.end())
                    for (auto & [form, ack, word] : it->second)
                        if (word == from) {
                            report += " forms";
                            ack = "+"; }
                result.report(from + " => " + to,
                    "- extra" + report);
            }
        }

        print("=== ", good.size(), " redirects");

        for (auto [from, to] : good)
        {
            entry e;
            e.title = from;
            e.topic += paragraph();
            e.topic.back().header = ">>>";
            e.topic.back().forms = to;
            e.topic.back().content += ">>>";
            entries.emplace(e.title, e.topic);
        }

        for (auto & [title, topic] : entries)
            result.accept(entry{
                std::move(title),
                std::move(topic)});
    };
}
                
