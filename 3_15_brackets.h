const std::map<str, std::unordered_set<str>> rejected_templates
{
    {   "mid",
    {
        "top",  "mid",  "bottom",  "der-top",  "der-mid",  "der-bottom",  "rel-top",  "rel-mid",  "rel-bottom",
        "top1", "mid1", "bottom1", "der-top1", "der-mid1", "der-bottom1", "rel-top1", "rel-mid1", "rel-bottom1",
        "top2", "mid2", "bottom2", "der-top2", "der-mid2", "der-bottom2", "rel-top2", "rel-mid2", "rel-bottom2",
        "top3", "mid3", "bottom3", "der-top3", "der-mid3", "der-bottom3", "rel-top3", "rel-mid3", "rel-bottom3", 
        "top4", "mid4", "bottom4", "der-top4", "der-mid4", "der-bottom4", "rel-top4", "rel-mid4", "rel-bottom4", 
        "top5", "mid5", "bottom5", "der-top5", "der-mid5", "der-bottom5", "rel-top5", "rel-mid5", "rel-bottom5",
    }},
    {   "wiki",
    {
        "wikipedia",

        "l", "head", "lb", "plural of"
    }}
};

str template_kind (str name)
{
    for (const auto & [kind, set] : rejected_templates)
        if (set.find (name) != set.end())
            return kind;
    return "";
}

Pass <entry, entry> brackets = [](auto & input, auto & output)
{
    Result result {__FILE__, output, true};

    std::set<str> heads, labels;

    bracketer B;
    B.proceed_sbrakets  = [&] (str s) { s =   "[" + s + "]";   result.report(s, "["  ); return s; };
    B.proceed_qbrakets  = [&] (str s) { s =   "{" + s + "}";   result.report(s, "{"  ); return s; };
    B.proceed_link      = [&] (str s) { s =  "[[" + s + "]]";  result.report(s, "[[" ); return s; };
    B.proceed_parameter = [&] (str s) { s = "{{{" + s + "}}}"; result.report(s, "{{{"); return s; };
    B.proceed_template  = [&] (str s)
    {
        str name, args; s.split_by("|", name, args); name.strip(" \t\n");


        if (name == "head") heads.emplace(s);
        if (name == "lb" || name == "labels") labels.emplace(s);

    
        str kind = "{{" + template_kind(name);
    
        result.report ("{{" + s + "}}", kind);
    
        return kind == "{{" ? "{{" + s + "}}" : "";
    };

    for (auto && [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("brackets  ", nn, " entries ", input.cargo, " cargo ");

        if (nn == 1)
        {
            // for (auto & [name, body] : templates)
            // {
            //     str report = body;
            // 
            //     bracketer b;
            //     b.proceed_sbrakets  = B.proceed_sbrakets ;
            //     b.proceed_qbrakets  = B.proceed_qbrakets ;
            //     b.proceed_link      = B.proceed_link     ;
            //     b.proceed_template  = B.proceed_template ;
            //     b.proceed_parameter = B.proceed_parameter;
            //     b.proceed(body);
            // 
            // 
            //     body = std::move(b.output);
            // 
            //     if (report != body) {
            //         report += "\n====================================\n";
            //         report += body;
            //         result.report (entry {name, std::move(report)}, "templates");
            //     }
            // 
            //     if (b.report.size() > 0) result.report(entry {name, str(std::move(b.report))}, "broken brackets (templates)");
            // }
            // 
            // print ("templates preprocessed");
        }

        bracketer b;
        b.proceed_sbrakets  = B.proceed_sbrakets ;
        b.proceed_qbrakets  = B.proceed_qbrakets ;
        b.proceed_link      = B.proceed_link     ;
        b.proceed_template  = B.proceed_template ;
        b.proceed_parameter = B.proceed_parameter;
        b.proceed(topic);
            
        if (b.report.size() > 0) result.report(entry {title, str(std::move(b.report))}, "broken brackets");
        
        topic = std::move(b.output);

        result.accept (entry {std::move(title), std::move(topic)});
    }

    for (auto head : heads) result.report (head, "heads");
    for (auto head : labels) result.report (head, "labels");
};

