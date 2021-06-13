#pragma once
#include "4.h"
namespace pass4
{
    str templates_taxon_(str title, str header, str body, Result<entry> & result)
    {
        args args (body, false); str name = args.name; str arg = args.body; auto & a = args;

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        if (body == "QUOTE" or
            body == "rfdate" or
            body == "RQ") return output;

        if (name == "vern")
        {
            str pl = a.acquire("pl"); a.ignore("pedia");
            if (a.complexity == 1) { output = a[0] + pl; } else kind += " quest";
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
        if (name == "taxon")
        {
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

    Pass <entry, entry> templates_taxon = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("templates4", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                for (auto & line : content)
                {
                    bracketer b;
                    b.proceed_template = [&](str s){ return templates_taxon_(t, h, s, result); };
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
