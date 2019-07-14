Pass <entry, entry> templating = [](auto & input, auto & output)
{
    Result result {__FILE__, output, true};

    array<str> accepted
    {
        "l"
    };

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

    for (auto && [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("templates ", nn, " entries ", input.cargo, " cargo ");

        bracketer b;
        b.proceed_template  = [&] (str s)
        {
            str report =  "{{" + s + "}}";
            array<str> ss = s.split_by("|");
            str name = ss [0]; ss.erase(ss.begin());
            str kind = "unprocessed";

            if (rejected.binary_found (name)) { kind = "rejected"; s = ""; } else
            if (accepted.binary_found (name)) { kind = "accepted";

                std::map<str,str> params;
                array<str> unnamed_params;
                for (str param : ss) {
                    str key, value;
                    if (param.split_by ("=", key, value))
                        params [key] = value; else
                        unnamed_params += param;
                }

                if (auto   it =  redirects_templates.find(name);
                           it != redirects_templates.end ())
                    name = it -> second;
                
                if (auto   it =  templates.find(name);
                           it != templates.end ())
                    s    = it -> second; else
                {
                    result.report(s + " : " + name, "404 Not found");
                }

                report += "\n";
                report += "---------------------\n";
                for (auto [key, value] : params) report += key + " = " + value + "\n";
                for (int i=0; i<unnamed_params.size(); i++)  report += "{{{" + std::to_string(i+1) + "}}} = " + unnamed_params [i] + "\n";
                report += "---------------------\n";
                report += s + "\n";
                report += "=====================\n";
            }
            else
            {
                s =  "{{" + s + "}}";
            }

            result.report(report, kind);
            return s;
        };
                                                                                         
        b.proceed(topic);

        topic = std::move(b.output);

        result.accept (entry {std::move(title), std::move(topic)});
    }
};
