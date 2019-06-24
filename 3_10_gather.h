Pass <pass2::entry, entry> gather = [](auto & input, auto & output)
{
    Result result {__FILE__, output, false};

    array<str> rejected
    {
        "top",  "mid",  "bottom",  "der-top",  "der-mid",  "der-bottom",  "rel-top",  "rel-mid",  "rel-bottom",
        "top1", "mid1", "bottom1", "der-top1", "der-mid1", "der-bottom1", "rel-top1", "rel-mid1", "rel-bottom1",
        "top2", "mid2", "bottom2", "der-top2", "der-mid2", "der-bottom2", "rel-top2", "rel-mid2", "rel-bottom2",
        "top3", "mid3", "bottom3", "der-top3", "der-mid3", "der-bottom3", "rel-top3", "rel-mid3", "rel-bottom3", 
        "top4", "mid4", "bottom4", "der-top4", "der-mid4", "der-bottom4", "rel-top4", "rel-mid4", "rel-bottom4", 
        "top5", "mid5", "bottom5", "der-top5", "der-mid5", "der-bottom5", "rel-top5", "rel-mid5", "rel-bottom5",
    };

    std::sort(rejected.begin(), rejected.end());

    std::vector<std::pair<str,str>> content; content.reserve (1024*1024);

    for (auto && [title, topic_] : input)
    {
        static int64_t nn = 0; if (++nn % 20'000 == 0) print("gather ", nn, " entries ", input.cargo, " cargo ");

        str topic; for (auto s : topic_) {
            topic += s;
            topic += '\n';
        }
        if (topic != "") topic.pop_back();

        //for (int e, m, b = 0; ; )
        //{
        //    str name;
        //    b = topic.find("{{", str::start_from(b)); if (b == str::nope){ break; }
        //    e = topic.find("}}", str::start_from(b)); if (e == str::nope){ break; }
        //    m = topic.find("|" , str::start_from(b)); if (e == str::nope)
        //        name = topic.substr (b+2, e-b-2); else
        //        name = topic.substr (b+2, m-b-2);
        //
        //    if (rejected.binary_found (name)) {
        //        if (topic.substr (b+2, e-b-4).find("{{") != str::nope) continue;
        //        result.reject (topic.begin() + b, topic.begin() + e-b+2);
        //        topic.erase (b, e-b+2);
        //    }
        //}

        //if (title.starts_with("Template:"))
        //{
        //    title.erase(0,9); title.strip();
        //    if (rejected.binary_found (title))
        //        result.report (entry {title, topic}, "Template reject"); else {
        //        result.report (entry {title, topic}, "Template accept"); 
        //        templates [title] = topic;
        //        }
        //    continue;
        //}

        content.emplace_back (std::move(title), std::move(topic));
    }

    for (auto && [title, topic] : content)
    {
        result.accept (entry {std::move(title), std::move(topic)});
    }
};
