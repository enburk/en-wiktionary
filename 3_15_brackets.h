const std::map<str, std::unordered_set<str>> rejected_templates
{
    {   "head",
    {
        "head", 
    }},
    {   "mid",
    {
        "top",  "mid",  "bottom",  "der-top",  "der-mid",  "der-bottom",  "rel-top",  "rel-mid",  "rel-bottom",
        "top1", "mid1", "bottom1", "der-top1", "der-mid1", "der-bottom1", "rel-top1", "rel-mid1", "rel-bottom1",
        "top2", "mid2", "bottom2", "der-top2", "der-mid2", "der-bottom2", "rel-top2", "rel-mid2", "rel-bottom2",
        "top3", "mid3", "bottom3", "der-top3", "der-mid3", "der-bottom3", "rel-top3", "rel-mid3", "rel-bottom3", 
        "top4", "mid4", "bottom4", "der-top4", "der-mid4", "der-bottom4", "rel-top4", "rel-mid4", "rel-bottom4", 
        "top5", "mid5", "bottom5", "der-top5", "der-mid5", "der-bottom5", "rel-top5", "rel-mid5", "rel-bottom5",
    }},
    {   "maybe",
    {
        "examples-right",
    }},
    {   "misc",
    {
        "Letter", "multiple images", "picdic", "projectlinks", "rfelite",
    }}
};

std::unordered_set<str> related
{
    "rel1", "der1", "hyp1", "coord1", "col1", "syn1", "ant1", "derived terms",
    "rel2", "der2", "hyp2", "coord2", "col2", "syn2", "ant2",
    "rel3", "der3", "hyp3", "coord3", "col3", "syn3", "ant3", "der3-u",
    "rel4", "der4", "hyp4", "coord4", "col4", "syn4", "ant4",
    "rel5", "der5", "hyp5", "coord5", "col5", "syn5", "ant5",
};

//str proceed_link (str title, str header, str body, Result<entry> & result, kind, output, report)
//{
//    str kind   = o;
//    str output = o + body + c;
//    str report = o + body + c;
//
//}

str proceed_brackets (str title, str header, str body, Result<entry> & result, str o, str c)
{
    str kind   = o;
    str output = o + body + c;
    str report = o + body + c;

    if (o == "[")
    {
        if (body.contains("\n")) { output = ""; kind = "[ multiline ]"; }
        if (body.contains( "|")) { kind = "[ pipe ]"; }
    }
    if (o == "{")
    {
        if (body.contains( "|"))
        {
            output = "";
            kind = "{ tables ";
            kind += lexical_items.find(header) == lexical_items.end() ? " 1 }" : " 2 }";
            report = "==== " + title + " ==== " + header + " ==== " + "\n\n" + report;
        }
        else
        if (body.contains("\n")) { output = ""; kind = "{ multiline }"; }
    }
    if (o == "[[")
    {
        if (body.contains("|")) {
            auto ss = body.split_by("|");
            if (ss.size() >= 3) kind = "[[ pipe pipe ]]";
            if (ss.size() == 2 &&
                ss[0].find_first_not_of(alnum + " .,;!?'-") == str::npos &&
                ss[1].find_first_not_of(alnum + " .,;!?'-") == str::npos ){
                body = output = "[[" + ss[1] + "]]";
                kind = "[[ pipe ok ]]"; } else
                kind = "[[ pipe no ]]";
        }

        if (lexical_items.find(header) != lexical_items.end() && body.ascii_isalnum()) { kind = "[[ ascii ]]"; output = body; } else
        {
            if (body.starts_with("Category:")) { output = ""; kind = "[[ Category ]]"; }
            if (body.starts_with("category:")) { output = ""; kind = "[[ Category ]]"; }
            if (body.starts_with("Image:"   )) { output = ""; kind = "[[ Image ]]"; }
            if (body.starts_with("image:"   )) { output = ""; kind = "[[ Image ]]"; }
            if (body.starts_with("File:"    )) { output = ""; kind = "[[ File ]]"; }
            if (body.starts_with("file:"    )) { output = ""; kind = "[[ File ]]"; }
        }
    }
    if (o == "{{")
    {
        str name; if (auto r = body.find('|'); r) name = body.upto(r.offset); else name = body; name.strip(" \t\n");

        for (const auto & [group, set] : rejected_templates)
            if (set.find (name) != set.end()) {
                kind = "{{ "+group+" }}";
                output = "";
                break;
            }

        if (related.find(name) != related.end())
        {
            kind = "{{ related items }}"; output.replace_all("\n", " ");
        }
        else
        if (name.starts_with("quote-")) { kind = "{{ quote quote- }}"; output = "'''0001'''"; } else
        if (name.starts_with("cite-" )) { kind = "{{ quote cite-  }}"; output = "'''0002'''"; } else
        if (name.starts_with("rfdate")) { kind = "{{ quote rfdate }}"; output = "'''0003'''"; } else
        if (name.starts_with("RQ:"   )) { kind = "{{ quote RQ }}";     output = "'''0004'''"; } else
        {}
    }

    if (output.contains("\n")) kind +=  " #br#";
    if (output.contains("\n")) report = "==== " + title + " ==== " + header + " ==== " + "\n\n" + report;
    if (output.contains("\n")) output.replace_all("\n", " ");
    if (output.contains( "|")) result.report (output, " - pipe");
    result.report (report, kind);
    return output;
}

Pass <entry, entry> brackets = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    for (auto && [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("brackets  ", nn, " entries ", input.cargo, " cargo ");

        for (auto & [header, content] : topic)
        {
            bracketer b;
            b.proceed_sbrakets  = [&] (str s) { return proceed_brackets (title, header, s, result,   "[", "]"  ); };
            b.proceed_qbrakets  = [&] (str s) { return proceed_brackets (title, header, s, result,   "{", "}"  ); };
            b.proceed_link      = [&] (str s) { return proceed_brackets (title, header, s, result,  "[[", "]]" ); };
            b.proceed_template  = [&] (str s) { return proceed_brackets (title, header, s, result,  "{{", "}}" ); };
            b.proceed_parameter = [&] (str s) { return proceed_brackets (title, header, s, result, "{{{", "}}}"); };
            b.proceed(content);
            
            if (b.report.size() > 0)
            {
                entry report;
                report.title = title;
                report.topic += paragraph {header, content};
                report.topic.back().content += "\n=================================================================\n";
                report.topic.back().content += str(b.report);
                result.report(report, "- broken brackets");
            }
        
            content = std::move(b.output);
        }

        result.accept (entry {std::move(title), std::move(topic)});
    }
};

        std::multimap<int, str, std::greater<int>> templates; int total = 0;
        for (auto [name, n] : brackets_internal::templates) { templates.emplace(n, name); total += n; }
        for (auto [n, name] : templates) result.report (std::to_string(n) + " " + name, "templates");
        result.report ("====================================", "templates");
        result.report (std::to_string(total >> 00) + " total", "templates");
        result.report (std::to_string(total >> 10) + " K    ", "templates");
        result.report (std::to_string(total >> 20) + " M    ", "templates");
        brackets_internal::templates.clear();

            /// Go to: https://dumps.wikimedia.org/enwiktionary/
    /// Download: enwiktionary-20190320-pages-articles.xml.bz2 (or latest version)
    /// Unzip it and rename to: enwiktionary-pages-articles.xml
    /// Gzip it to: enwiktionary-pages-articles.xml.gz
    /// Then run and wait...
