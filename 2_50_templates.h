#pragma once
#include "2.h"
namespace pass2
{
    const std::map<str, std::unordered_set<str>> rejected_templates
    {
        {   "rf",
        {
            "rfquotek",
        }},
        {   "mid",
        {
            "top",  "mid",  "bottom",  "col-top",  "col-bottom",  "der-top",  "der-mid",  "der-bottom",  "rel-top",  "rel-mid",  "rel-bottom", 
            "top1", "mid1", "bottom1", "col-top1", "col-bottom1", "der-top1", "der-mid1", "der-bottom1", "rel-top1", "rel-mid1", "rel-bottom1",
            "top2", "mid2", "bottom2", "col-top2", "col-bottom2", "der-top2", "der-mid2", "der-bottom2", "rel-top2", "rel-mid2", "rel-bottom2",
            "top3", "mid3", "bottom3", "col-top3", "col-bottom3", "der-top3", "der-mid3", "der-bottom3", "rel-top3", "rel-mid3", "rel-bottom3", 
            "top4", "mid4", "bottom4", "col-top4", "col-bottom4", "der-top4", "der-mid4", "der-bottom4", "rel-top4", "rel-mid4", "rel-bottom4", 
            "top5", "mid5", "bottom5", "col-top5", "col-bottom5", "der-top5", "der-mid5", "der-bottom5", "rel-top5", "rel-mid5", "rel-bottom5",
        }},
        {   "miscellaneous",
        {
            "Letter", "multiple images", "picdic", "projectlinks",
            "examples", "examples-right",
            "en-decades",
        }},
        {   "TEST",
        {
        }}
    };
    array<str> label_same =
    {
        "countable", "uncountable", "transitive", "intransitive",
        "archaic", "by extension", "collective", "colloquial", "dated", "derogatory", "dialect", "dialectal",
        "especially in combination", "figuratively", "humorous", "idiomatic", "informal", "literary", "modal",
        "nonstandard", "offensive", "obsolete", "poetic", "rare", "regional", "slang", "vulgar",

        "algebra", "anatomy", "architecture", "astronomy", "aviation",
        "baseball", "basketball", "biblical", "biochemistry", "biology", "botany", "business",
        "chemistry", "computing", "cricket", "cytology", "electronics", "economy", "economics",
        "genetics", "geography", "geology", "geometry", "grammar",
        "finance", "historical", "immunology", "inorganic chemistry", "Internet", "Internet slang", "law", "linguistics",
        "mathematics", "medicine", "military", "mineralogy", "music", "nautical", "organic chemistry",
        "pathology", "pharmacology", "philosophy", "physics", "physiology", "politics", "poker", "programming", "psychology",
        "religion", "rhetoric", "sports", "statistics", "surgery", "telecommunications", "transport", "zoology",

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
        { "legal", "law" }, { "math", "mathematics" }, { "maths", "mathematics" }, { "mineral", "mineralogy" }, 
        { "enzyme", "biochemistry" }, { "protein", "biochemistry" }, { "organic compound", "organic chemistry" },
    };

    str proceed_templates (str title, str header, str body, Result<entry> & result)
    {
        if (body == "QUOTE" || body == "rfdate" || body == "RQ") return "{{" + body + "}}";

        args args (body); str name = args.name; str arg = args.body;

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        for (const auto & [group, set] : rejected_templates) {
            if (set.find (name) != set.end()) {
                result.reject(report, "{{"+group+"}}");
                return "";
            }
        }

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
        if (name == "mention")
        {
            str a1, a2, a3;
            args.ignore("t"); args.ignore("tr"); args.ignore("gloss"); args.ignore("pos");
            if (args.lang != "") { args.unnamed.insert(0, args.lang); args.lang = ""; args.complexity++; }
            if (args.unnamed.size() > 1 && args[1] != "") a1 = "[[''"+args[1]+"'']]"; 
            if (args.unnamed.size() > 2 && args[2] != "") a2 = "[[''"+args[2]+"'']]"; 
            if (args.unnamed.size() > 3 && args[3] != "") a3 = "("+oquot+args[3]+cquot+")"; a3.replace_all("\n", " ");
            if (args.complexity == 1) { output = ""; kind += " 0"; report += " ==== " + title; } else
            if (args.complexity == 2) { output = a1; kind += " 1"; } else
            if (args.complexity == 3 && a1 == "" && a2 != "") { output = a2; kind += " 2-x"; } else
            if (args.complexity == 3 && a1 != "" && a2 == "") { output = a1; kind += " 2x-"; } else
            if (args.complexity == 3 && a1 != "" && a2 != "") { output = a2; kind += " 2xx"; } else
            if (args.complexity == 4 && a1 != "" && a2 == "" && a3 != "") { output = a1 + a3; kind += " 3x-x"; } else
            if (args.complexity == 4 && a1 != "" && a2 != "" && a3 != "") { output = a2 + a3; kind += " 3xxx"; } else
            if (args.complexity == 4 && a1 == "" && a2 != "" && a3 != "") { output = a2 + a3; kind += " 3-xx"; } else
            if (args.acquire("sc") != "") { kind += " sc"; report += " ==== " + title; }
            { kind += " quest"; report += " ==== " + title; }
        }
        else
        if (name == "non-gloss definition")
        {
            if (args.complexity == 1) { output = "''"+args[0]+"''"; } else
            { kind += " quest"; report += " ==== " + title; }
        }
        else
        if (name == "sense")
        {
            if (args.complexity == 6) { output = "(''"+args[0]+", "+args[1]+", "+args[2]+", "+args[3]+", "+args[4]+", "+args[5]+"''):"; kind += " 6"; } else
            if (args.complexity == 5) { output = "(''"+args[0]+", "+args[1]+", "+args[2]+", "+args[3]+", "+args[4]+"''):"; kind += " 6"; } else
            if (args.complexity == 4) { output = "(''"+args[0]+", "+args[1]+", "+args[2]+", "+args[3]+"''):"; kind += " 6"; } else
            if (args.complexity == 3) { output = "(''"+args[0]+", "+args[1]+", "+args[2]+"''):"; kind += " 6"; } else
            if (args.complexity == 2) { output = "(''"+args[0]+", "+args[1]+"''):"; kind += " 2"; } else
            if (args.complexity == 1) { output = "(''"+args[0]+"''):"; kind += " 1"; } else
            { kind += " quest"; report += " ==== " + title; }
        }
        else
        if (name == "taxlink")
        {
            args.ignore("noshow"); args.ignore("nomul"); args.ignore("ver");
            str sname; if (args.unnamed.size() >= 1) sname = args.unnamed[0];
            str taxon; if (args.unnamed.size() >= 2) taxon = args.unnamed[1];
            str genus, species; sname.split_by(" ", genus, species);
            const array<str> taxons = { "division", "taxon",
                "superkingdom", "kingdom", "subkingdom", "infrakingdom",
                "superphylum", "phylum", "subphylum", "class", "subclass", "infraclass",
                "superorder", "order", "suborder", "infraorder",
                "superfamily", "family",  "subfamily", "tribe" };
            if (args.complexity == 2 && taxons.contains(taxon) && species == "") { output = "["  +sname+  "]"; kind += " 1 taxon";   } else
            if (args.complexity == 2 && taxon == "genus"       && species == "") { output = "[''"+sname+"'']"; kind += " 1 genus";   } else
            if (args.complexity == 2 && taxon == "species"                     ) { output = "[''"+sname+"'']"; kind += " 1 species"; } else
            if (args.complexity == 2                                           ) { output = "[''"+sname+"'']"; kind += " 2"; } else
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

    Pass <entry, entry> templates = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, UPDATING_REPORTS};

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
                b.proceed_template  = [&] (str s) { return proceed_templates (title, header, s, result); };
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
