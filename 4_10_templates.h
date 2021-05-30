#pragma once
#include "4.h"
namespace pass4
{
    const std::map<str, std::unordered_set<str>> rejected_templates
    {
        {   "requests",
        {
            "rfe", // request for etymology
            "rfi", // (Image requested)
            "rfdef", // (definition required, request for definition, required definition)
            "rfquotek", "rfap", "rfex", "rfp", "rfap", "rfquote-sense", "rfv-etym",
            "rfc-sense", "rfv-sense", "rfc", "rfclarify", "rfd-sense", "rfv-pron",
            "rfref", "rfquote", "rfscript", "rfd-redundant", "rfm-sense",
            "rfd", "rfm", "rfv" //, "rfdate"
        }},
        {   "pronunciation",
        {
            "audio", "rhymes", "enPR", // non IPA pronunciation
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
            "examples", "examples-right", "broken ref",
            "en-decades",
        }},
        {   "TEST",
        {
        }}
    };
    str templates_ (str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; str arg = args.body;  auto & a = args;

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

        if (name == "IPA")
        {
            a.ignore("nocount");
            a.ignore("n"); a.ignore("n1"); a.ignore("n2"); a.ignore("n3"); a.ignore("n4"); a.ignore("n5"); str
            q = a.acquire("qual" ); if (q != "" and a.unnamed.size() >= 1) a[0] = " (''" + q + "'') " + a[0];
            q = a.acquire("qual1"); if (q != "" and a.unnamed.size() >= 1) a[0] = " (''" + q + "'') " + a[0];
            q = a.acquire("qual2"); if (q != "" and a.unnamed.size() >= 2) a[1] = " (''" + q + "'') " + a[1];
            q = a.acquire("qual3"); if (q != "" and a.unnamed.size() >= 3) a[2] = " (''" + q + "'') " + a[2];
            q = a.acquire("qual4"); if (q != "" and a.unnamed.size() >= 4) a[3] = " (''" + q + "'') " + a[3];
            q = a.acquire("qual5"); if (q != "" and a.unnamed.size() >= 5) a[4] = " (''" + q + "'') " + a[4];
            q = a.acquire("qual6"); if (q != "" and a.unnamed.size() >= 6) a[5] = " (''" + q + "'') " + a[5];
            q = a.acquire("qual7"); if (q != "" and a.unnamed.size() >= 7) a[6] = " (''" + q + "'') " + a[6];
            if (a.complexity == 1) { output = a[0]; kind += " 1"; } else
            if (a.complexity == 2) { output = a[0]+", "+a[1]; kind += " 2"; } else
            if (a.complexity == 3) { output = a[0]+", "+a[1]+", "+a[2]; kind += " 6"; } else
            if (a.complexity == 4) { output = a[0]+", "+a[1]+", "+a[2]+", "+a[3]; kind += " 6"; } else
            if (a.complexity == 5) { output = a[0]+", "+a[1]+", "+a[2]+", "+a[3]+", "+a[4]; kind += " 6"; } else
            if (a.complexity == 6) { output = a[0]+", "+a[1]+", "+a[2]+", "+a[3]+", "+a[4]+", "+a[5]; kind += " 6"; } else
            if (a.complexity == 7) { output = a[0]+", "+a[1]+", "+a[2]+", "+a[3]+", "+a[4]+", "+a[5]+", "+a[6]; kind += " 6"; } else
            if (a.complexity == 8) { output = a[0]+", "+a[1]+", "+a[2]+", "+a[3]+", "+a[4]+", "+a[5]+", "+a[6]+", "+a[7]; kind += " 6"; } else
            kind += " quest";
        }
        else
        if (name == "non-gloss definition")
        {
            if (a.complexity == 1) { output = "''"+a[0]+"''"; } else
            kind += " quest";
        }
        else
        if (name == "ux") // usex, eg, example
        {
            a.ignore("inline"); str
            q = a.acquire("q");      if (q != "") a[0] += " (''" + q + "'')";
            q = a.acquire("footer"); if (q != "") a[0] += " (''" + q + "'')";
            if (a.complexity == 1) { output = "''"+a[0]+"''"; kind += " 1"; } else
            if (a.complexity == 2) { output = "''"+a[0]+"'' ― ''"+a[1]+"''"; kind += " 2"; } else
            { output = ""; kind += " skip"; }
        }
        else
        if (name == "prefixusex")
        {
            a.ignore_all();
            if (a.complexity == 2) { output = "''"+title+"'' + ''"+a[0]+"'' → ''"+a[1]+"''"; } else
            kind += " quest";
        }
        else
        if (name == "accent" // a
        or  name == "qualifier" // qua, i, q, qf, qual
        or  name == "sense")
        {
            if (name == "accent") for (str & arg : a.unnamed) {
                if (arg == "GA"    ) arg = "General American";
                if (arg == "GenAm" ) arg = "General American";
                if (arg == "RP"    ) arg = "Received Pronunciation";
                if (arg == "AU"    ) arg = "General Australian";
                if (arg == "AuE"   ) arg = "General Australian";
                if (arg == "Aus"   ) arg = "General Australian";
                if (arg == "AusE"  ) arg = "General Australian";
                if (arg == "GenAus") arg = "General Australian";
                if (arg == "CA"    ) arg = "Canada";
                if (arg == "NZ"    ) arg = "General New Zealand";
                if (arg == "cot–caught"          ) arg = "</i>cot–caught<i> merger";
                if (arg == "pin-pen"             ) arg = "</i>pin-pen<i> merger";
                if (arg == "father-bother"       ) arg = "</i>father-bother<i> merger";
                if (arg == "wine/whine"          ) arg = "without the </i>wine-whine<i> merger";
                if (arg == "horse-hoarse"        ) arg = "without the </i>horse–hoarse<i> merger";
                if (arg == "Mary-marry-merry"    ) arg = "</i>Mary–marry–merry<i> merger";
                if (arg == "non-Mary-marry-merry") arg = "</i>Mary–marry–merry<i> distinction";
            }
            if (a.complexity == 6) { output = "(''"+a[0]+", "+a[1]+", "+a[2]+", "+a[3]+", "+a[4]+", "+a[5]+"''):"; kind += " 6"; } else
            if (a.complexity == 5) { output = "(''"+a[0]+", "+a[1]+", "+a[2]+", "+a[3]+", "+a[4]+"''):"; kind += " 6"; } else
            if (a.complexity == 4) { output = "(''"+a[0]+", "+a[1]+", "+a[2]+", "+a[3]+"''):"; kind += " 6"; } else
            if (a.complexity == 3) { output = "(''"+a[0]+", "+a[1]+", "+a[2]+"''):"; kind += " 6"; } else
            if (a.complexity == 2) { output = "(''"+a[0]+", "+a[1]+"''):"; kind += " 2"; } else
            if (a.complexity == 1) { output = "(''"+a[0]+"''):"; kind += " 1"; } else
            { output = ""; kind += " skip"; }
        }
        else
        if (name == "suffix")
        {
            if (a.lang != "") { a.unnamed = a.lang + a.unnamed; a.complexity++; }; str
            q = a.acquire("alt1"); if (q != "" and a.unnamed.size() >= 2) { a[1] = q; }//kind = "{{suffix}} alt"; }
            q = a.acquire("alt2"); if (q != "" and a.unnamed.size() >= 3) { a[2] = q; }//kind = "{{suffix}} alt"; }
            q = a.acquire("alt3"); if (q != "" and a.unnamed.size() >= 4) { a[3] = q; }//kind = "{{suffix}} alt"; }
            if (a.unnamed.size() >= 3 and a[2] != "" and not a[2].starts_with("-")) a[2] = "-" + a[2];
            if (a.unnamed.size() >= 4 and a[3] != "" and not a[3].starts_with("-")) a[3] = "-" + a[3];
            if (a.unnamed.size() >= 2 and a[1] != "") a[1] = "''" + a[1] + "''";
            if (a.unnamed.size() >= 3 and a[2] != "") a[2] = "''" + a[2] + "''";
            if (a.unnamed.size() >= 4 and a[3] != "") a[3] = "''" + a[3] + "''";
            q = a.acquire("t1" ); if (q != "" and a.unnamed.size() >= 2) { a[1] += " (“"+q+"”)"; }//kind = "{{suffix}} trans"; }
            q = a.acquire("t2" ); if (q != "" and a.unnamed.size() >= 3) { a[2] += " (“"+q+"”)"; }//kind = "{{suffix}} trans"; }
            q = a.acquire("t3" ); if (q != "" and a.unnamed.size() >= 4) { a[3] += " (“"+q+"”)"; }//kind = "{{suffix}} trans"; }
            q = a.acquire("lit"); if (q != "" and a.unnamed.size() >= 2) { a[1] += " (literally “"+q+"”)"; }
            a.ignore_all();
            if (a.complexity == 5) a.complexity = 4;
            if (a.complexity == 3 and a[2] != "" and a[1] == "") { output = a[2]; kind += " 1"; } else
            if (a.complexity == 3 and a[2] != "" and a[1] != "") { output = a[1]+" + "+a[2]; kind += " 2"; }  else
            if (a.complexity == 4 and a[3] == "" and a[2] != "" and a[1] == "") { output = a[2]; kind += " 3"; }  else
            if (a.complexity == 4 and a[3] == "" and a[2] != "" and a[1] != "") { output = a[1]+" + "+a[2]; kind += " 3"; }  else
            if (a.complexity == 4 and a[3] != "" and a[2] != "" and a[1] == "") { output = a[2]+" + "+a[3]; kind += " 3"; }  else
            if (a.complexity == 4 and a[3] != "" and a[2] != "" and a[1] != "") { output = a[1]+" + "+a[2]+" + "+a[3]; kind += " 4"; }  else
            kind += " quest";
        }
        else
        if (name == "taxlink")
        {
            const array<str> ignored = {
            "entry", "i", "noi", "noshow", "nowshow", "nosohw", "noshhow", "boshow",
            "nomul", "nospe", "obs", "source", "status", "pedia", "pedia\\",
            "ber", "nover", "ver", "vere", "vr", "wplink", "wplnk", "wslink", };
            for (auto ignore : ignored) a.ignore(ignore);

            str sname; if (a.unnamed.size() >= 1) sname = a.unnamed[0];
            str taxon; if (a.unnamed.size() >= 2) taxon = a.unnamed[1];
            str genus, species; sname.split_by(" ", genus, species);
            const array<str> taxons = { "division", "taxon",
                "superkingdom", "kingdom", "subkingdom", "infrakingdom",
                "superphylum", "phylum", "subphylum", "subspecies",
                "superclass", "class", "subclass", "infraclass",
                "superorder", "order", "suborder", "infraorder",
                "superfamily", "family",  "subfamily", "tribe" };
            bool is_taxon   = taxons.contains(taxon) && species == "";
            bool is_genus   = taxon == "genus"       && species == "";
            bool is_scepies = taxon == "species";
            if (a.complexity == 2 && is_taxon  ) { output = "["  + sname +  "]"; kind += " 1 taxon";   } else
            if (a.complexity == 2 && is_genus  ) { output = "[''"+ sname +"'']"; kind += " 1 genus";   } else
            if (a.complexity == 2 && is_scepies) { output = "[''"+ sname +"'']"; kind += " 1 species"; } else
            if (a.complexity == 2              ) { output = "[''"+ sname +"'']"; kind += " 2"; } else
            if (a.complexity == 3              ) { output = "[''"+ a[2]  +"'']"; kind += " 3"; } else
            if (a.complexity == 4              ) { output = "[''"+ a[2]  +"'']"; kind += " 4"; } else
            kind += " quest";
        }
        else
        if (name.ends_with(" Hypernyms"))
        {
            kind = " Hypernyms";
            output = Templates[name];
            output.replace_all("\n", " ");
            output.strip();
            bracketer b;
            b.proceed_template = [&](str s){ return templates_(title, header, s, result); };
            b.proceed(output);
            output = b.output;
        }
        else
        {
            kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }

        if (kind.contains(" quest")) kind += " !!!!!";
        if (output.contains("\n")) kind +=  " #br#";
        if (output.contains("\n")) report = "==== " + title + " ==== " + header + " ==== " + "\n\n" + report;
        if (output.contains("\n")) output.replace_all("\n", " ");
        result.report (report + " => " + output + " == " + title, kind);
        return output;
    }

    Pass <entry, entry> templates = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

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
