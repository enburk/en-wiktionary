#pragma once
#include "4.h"
namespace pass4
{
    str templates_taxon_(str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; auto & a = args;

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        if (name == "vern")
        {
            a.ignore("dis");
            a.ignore("pedia");
            a.ignore("novern");
            str pl = a.acquire("pl");
            output = a.link("", "") + pl;
        }
        else
        if (name == "taxlink" or
            name == "taxlinknew")
        {
            const array<str> ignored = {
            "entry", "i", "noi", "noshow", "nowshow", "nosohw", "noshhow", "boshow",
            "nomul", "nospe", "obs", "source", "status", "pedia", "pedia\\",
            "ber", "nover", "ver", "vere", "vr", "wplink", "wplnk", "wslink", };
            for (auto ignore : ignored) a.ignore(ignore);

            str sname; if (a.size() >= 1) sname = a[0];
            str taxon; if (a.size() >= 2) taxon = a[1];
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
            if (a.complexity == 2 && is_taxon  ) { output =        sname ; kind += " 1 taxon";   } else
            if (a.complexity == 2 && is_genus  ) { output = italic(sname); kind += " 1 genus";   } else
            if (a.complexity == 2 && is_scepies) { output = italic(sname); kind += " 1 species"; } else
            if (a.complexity == 2              ) { output = italic(sname); kind += " 2"; } else
            if (a.complexity == 3              ) { output = italic(a[2] ); kind += " 3"; } else
            if (a.complexity == 4              ) { output = italic(a[2] ); kind += " 4"; } else
            kind += " quest";
        }
        else
        if (name == "taxlink2")
        {
            output = italic(a[0]);
        }
        else
        if (name == "taxon")
        {
            a.ignore("i");
            str nodot = a.acquire("nodot");
            if (a.complexity <= 2) kind += " quest";
            if (a.complexity >= 5) kind += " quest";
            if (a.complexity >= 3) output = "A taxonomic " + a[0] + " within the " + a[1] + " " + a[2];
            if (a.complexity >= 4) output += " – " + a [3];
            if (nodot        =="") output += ".";
        }
        else
        if (name == "SI-unit" or
            name == "SI-unit-2" or
            name == "SI-unit-np" or
            name == "SI-unit-abb" or
            name == "SI-unit-abb2" or
            name == "SI-unit-abbnp")
        {
            if (name == "SI-unit" or
                name == "SI-unit-np") a.languaged();

            str t2;
            if (name == "SI-unit-2"   ) { t2 = a[3]; a.unnamed.erase(2); }
            if (name == "SI-unit-abb2") { t2 = a[2]; a.unnamed.erase(2); }

            if (a.size() == 2) a.unnamed +=
                a[1] == "ampere" ? "current" :
                a[1] == "candela" ? "luminous intensity" :
                a[1] == "kelvin" ? "temperature" :
                a[1] == "gram" ? "mass" :
                a[1] == "gramme" ? "mass" :
                a[1] == "meter" ? "length" :
                a[1] == "metre" ? "length" :
                a[1] == "mole" ? "amount of substance" :
                a[1] == "second" ? "time" :
                a[1] == "coulumb" ? "charge" :
                a[1] == "farad" ? "capacitance" :
                a[1] == "hertz" ? "frequency" :
                a[1] == "joule" ? "energy" :
                a[1] == "newton" ? "force" :
                a[1] == "ohm" ? "resistance" :
                a[1] == "pascal" ? "pressure" :
                a[1] == "watt" ? "power" :
                a[1] == "tesla" ? "magnetic flux intensity" :
                a[1] == "volt" ? "electric potential" :
                "??????????????????????????????????????????";

            if (a.size() != 3) a.kind += " quest size"; else
            {
                str pow =
                    a[0] == "yocto" ?"-24" :
                    a[0] == "zepto" ?"-21" :
                    a[0] == "atto"  ?"-18" :
                    a[0] == "femto" ?"-15" :
                    a[0] == "pico"  ?"-12" :
                    a[0] == "nano"  ? "-9" :
                    a[0] == "micro" ? "-6" :
                    a[0] == "milli" ? "-3" :
                    a[0] == "centi" ? "-2" :
                    a[0] == "deci"  ? "-1" :
                    a[0] == "deca"  ?  "1" :
                    a[0] == "hecto" ?  "2" :
                    a[0] == "kilo"  ?  "3" :
                    a[0] == "mega"  ?  "6" :
                    a[0] == "giga"  ?  "9" :
                    a[0] == "tera"  ? "12" :
                    a[0] == "peta"  ? "15" :
                    a[0] == "exa"   ? "18" :
                    a[0] == "zetta" ? "21" :
                    a[0] == "yotta" ? "24" :
                    "";
                if (pow == "") a.kind += " quest";

                output = "(''metrology'') ";

                output +=
                    name == "SI-unit" or
                    name == "SI-unit-2" ?
                    "An ": "Symbol for " + bold(a[0]+a[1]);

                if (t2 != "") output += " (" + italic(a[0]+t2) + ")";
                
                output += ", an SI unit of " + a[2] +
                    " equal to 10<small><sup>" + pow
                    + "</sup></small> " + a[1];
                
                if (name != "SI-unit-abbnp" and
                    name != "SI-unit-np")
                    output += "s";

                if (t2 != "") output += " (''" + t2 + "s'')";

                output += ".";
            }
        }
        else
        if (name == "frac")
        {
            if (a.complexity == 1) output =        "<small><sup>"  "1" "</sup><big>⁄</big><sub>"+a[0]+"</sub></small>"; else
            if (a.complexity == 2) output =        "<small><sup>"+a[0]+"</sup><big>⁄</big><sub>"+a[1]+"</sub></small>"; else
            if (a.complexity == 3) output = a[0] + "<small><sup>"+a[1]+"</sup><big>⁄</big><sub>"+a[2]+"</sub></small>"; else 
            kind += " quest";
        }
        else
        {
            kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }

        if (a.kind != "{{}}") 
        if (not a.opt.empty()) a.kind += " opt";

        if (kind.contains(" quest")) kind += " !!!!!";
        if (output.contains("\n")) kind +=  " #br#";
        if (output.contains("\n")) report = "==== "+title+" ==== "+header+" ==== "+"\n\n" + report;
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
