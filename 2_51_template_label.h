#pragma once
#include "2.h"
namespace pass2
{
    array<str> label_same =
    {
        "countable", "uncountable", "transitive", "intransitive",
        "archaic", "by extension", "collective", "colloquial", "dated", "derogatory", "dialect", "dialectal",
        "especially in combination", "figuratively", "humorous", "idiomatic", "informal", "literary", "modal",
        "nonstandard", "offensive", "obsolete", "poetic", "rare", "regional", "slang", "vulgar",

        "algebra", "anatomy", "architecture", "astrology", "astronomy", "astrophysics", "aviation",
        "baseball", "basketball", "biblical", "biochemistry", "biology", "botany", "business",
        "chemistry", "computing", "computer graphics", "cosmology", "cricket", "cryptography", "cytology",
        "electronics", "economy", "economics",
        "genetics", "geography", "geology", "geometry", "grammar",
        "finance", "historical", "immunology", "inorganic chemistry", "Internet", "Internet slang", "law", "linguistics",
        "mathematics", "medicine", "meteorology", "military", "mineralogy", "music", "nautical", "organic chemistry",
        "pathology", "pharmacology", "philosophy", "physics", "physiology",
        "politics", "poker", "probability theory", "programming", "psychology",
        "religion", "rhetoric", "sports", "statistics", "surgery",
        "telecommunications", "transport", "video games", "zoology",

        "Christianity", "Buddhism", "Hinduism", "Islam", "Judaism",
        "India", "Ireland", "Scotland", "South Africa"
    };
    std::map<str,str> label_map =
    {
        { "US", "US" },{ "Canada", "Canada" },
        { "UK", "Britain" }, { "British", "Britain" }, { "Britain", "Britain" },
        { "AU", "Australia" }, { "Australia", "Australia" },
        { "NZ", "New Zealand" }, { "New Zealand", "New Zealand" },
        { "figurative", "figuratively" }, { "jocular", "humorous" }, { "pejorative", "derogatory" },
        { "internet", "Internet" }, { "internet slang", "Internet slang" }, 
        { "legal", "law" }, { "math", "mathematics" }, { "maths", "mathematics" }, { "medical", "medicine" }, { "mineral", "mineralogy" }, 
        { "enzyme", "biochemistry" }, { "protein", "biochemistry" }, { "organic compound", "organic chemistry" }, { "star", "astronomy" },
    };

    str proceed_template_label (str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; str arg = args.body;

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        static bool first_time = true; if (first_time) { first_time = false;
            for (str s : label_same) label_map[s] = s;
        }

        if (name == "label")
        {
            bool ok = true;
            if (args.unnamed.size() >= 1) if (auto it = label_map.find(args[0]); it != label_map.end()) args[0] = it->second; else ok = false;
            if (args.unnamed.size() >= 2) if (auto it = label_map.find(args[1]); it != label_map.end()) args[1] = it->second; else ok = false;
            if (args.unnamed.size() >= 3) if (auto it = label_map.find(args[2]); it != label_map.end()) args[2] = it->second; else ok = false;
            if (args.unnamed.size() >= 4) if (auto it = label_map.find(args[3]); it != label_map.end()) args[3] = it->second; else ok = false;
            if (args.complexity == 4 && ok) { output = "(''"+args[0]+", "+args[1]+", "+args[2]+", "+args[3]+"'')"; kind += " 4"; } else
            if (args.complexity == 3 && ok) { output = "(''"+args[0]+", "+args[1]+", "+args[2]+"'')"; kind += " 3"; } else
            if (args.complexity == 2 && ok) { output = "(''"+args[0]+", "+args[1]+"'')"; kind += " 2"; } else
            if (args.complexity == 1 && ok) { output = "(''"+args[0]+"'')"; kind += " 1"; } else
            { kind += " quest"; report += " ==== " + title; }
        }
        else
        {
            kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }

        if (output.contains("\n")) kind +=  " #br#";
        if (output.contains("\n")) report = "==== " + title + " ==== " + header + " ==== " + "\n\n" + report;
        if (output.contains("\n")) output.replace_all("\n", " ");
        result.report (report, kind);
        return output;
    }

    Pass <entry, entry> template_label = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true}; // UPDATING_REPORTS};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("templates", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                bracketer b;
                b.proceed_sbrakets  = [&] (str s) { return   "[" + s + "]"  ; };
                b.proceed_qbrakets  = [&] (str s) { return   "{" + s + "}"  ; };
                b.proceed_parameter = [&] (str s) { return "{{{" + s + "}}}"; };
                b.proceed_link      = [&] (str s) { return  "[[" + s + "]]" ; };
                b.proceed_template  = [&] (str s) { return proceed_template_label (title, header, s, result); };
                b.proceed(content);
            
                if (b.report.size() > 0)
                {
                    entry report;
                    report.title = title;
                    report.topic += paragraph {header, forms, content};
                    report.topic.back().content += "\n==============================================================\n";
                    report.topic.back().content += str(b.report);
                    result.report(report, "- broken brackets");
                }
        
                content = std::move(b.output);
            }

            result.accept (entry {std::move(title), std::move(topic)});
        }

        std::multimap<int, str, std::greater<int>> templates; int total = 0;
        for (auto [name, n] : templates_statistics [__FILE__]) { templates.emplace(n, name); total += n; }
        for (auto [n, name] : templates) result.report (std::to_string(n) + " " + name, "# templates");
        result.report ("====================================", "# templates");
        result.report (std::to_string(total >> 00) + " total", "# templates");
        result.report (std::to_string(total >> 10) + " K    ", "# templates");
        result.report (std::to_string(total >> 20) + " M    ", "# templates");
    };
}
