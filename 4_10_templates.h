#pragma once
#include "4.h"
namespace pass4
{
    const std::map<str, std::unordered_set<str>> rejected_templates
    {
        {   "rf",
        {
            "rfquotek",
        }},
        {   "mid",
        {
            "top",  "mid",  "bottom",  "col-top",  "col-bottom", 
            "top1", "mid1", "bottom1", "col-top1", "col-bottom1",
            "top2", "mid2", "bottom2", "col-top2", "col-bottom2",
            "top3", "mid3", "bottom3", "col-top3", "col-bottom3", 
            "top4", "mid4", "bottom4", "col-top4", "col-bottom4", 
            "top5", "mid5", "bottom5", "col-top5", "col-bottom5",
            "der-top",  "der-mid",  "der-bottom",  "rel-top",  "rel-mid",  "rel-bottom", 
            "der-top1", "der-mid1", "der-bottom1", "rel-top1", "rel-mid1", "rel-bottom1",
            "der-top2", "der-mid2", "der-bottom2", "rel-top2", "rel-mid2", "rel-bottom2",
            "der-top3", "der-mid3", "der-bottom3", "rel-top3", "rel-mid3", "rel-bottom3", 
            "der-top4", "der-mid4", "der-bottom4", "rel-top4", "rel-mid4", "rel-bottom4", 
            "der-top5", "der-mid5", "der-bottom5", "rel-top5", "rel-mid5", "rel-bottom5",
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
    str templates_ (str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; str arg = args.body;

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        for (const auto & [group, set] : rejected_templates) {
            if (set.find (name) != set.end()) {
                result.reject(report,
                "{{"+group+"}}");
                return "";
            }
        }

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
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("templates", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                for (auto & line : content)
                {
                    bracketer b;
                    b.proceed_template = [&](str s){ return templates_(t, h, s, result); };
                    b.proceed(line);
                    line = b.output;
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }

        dump_templates_statistics(result);
    };
}
