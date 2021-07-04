#pragma once
#include "4.h"
namespace pass4
{
    str templates_names_(str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; auto & a = args;
        args.languaged();

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        args.kind  = "{{" + name + "}}";

        if (name == "surname" or
            name == "given name" or
            name == "historical given name")
        {
            str q;
            str A = a.acquire("a"); if (A == "") A = "A";

            auto
            dims  = a.acquire_all("diminutive");
            dims += a.acquire_all("dim");

            auto froms = a.acquire_all("from");
            for (str & s : froms)
            if (s.contains(":")) {
                str l, n; s.split_by(":", l, n);
                s = Languages[l] + " " + n;
                a.kind += " lang";
            }

            str dim  = str::list(dims,  ", ", " or ");
            str from = str::list(froms, ", ", " or ");
            str var  = str::list(a.acquire_all("var"), ", ", " or ");
            str eq   = str::list(a.acquire_all("eq" ), ", ", " or ");

            str usage = a.acquire("usage");
            str dot   = a.acquire("dot"  );
            str nodot = a.acquire("nodot");
            str dimin = dim == "" ? " " : " diminutive of the ";

            str what;
            if (a.size() == 0) { ; } else 
            if (a.size() >= 1 and a[0] == "") { ; } else
            if (a.size() >= 1 and a[0] != "") { what = a[0];
                if (a[0].starts_with("English") and A == "A") A = "An"; } else
                a.kind += " quest";

            str gen   = a.acquire("gender"); if (gen != "") { what = gen; a.kind += " gen"; }
            str gor   = a.acquire("or"); if (gor != "") { what += " or " + gor; a.kind += " or"; }

            if (what  != "") what += " "; what += name; output = A + dimin + what;

            if (dim   != "") output += " " + dim;
            if (from  != "") output += ", from " + from;
            if (var   != "") output += ", variant of " + var;
            if (eq    != "") output += ", equivalent to English " + eq;
            if (usage != "") output += ", of " + usage + " usage";
            if (name == "surname" and dot   != "") output += dot; else
            if (name == "surname" and nodot == "") output += ".";
            output = "''" + output + "''";
        }
        else
        if (name == "coinage" or
            name == "named-after")
        {
            a.ignore("w");
            a.ignore("wp");
            a.ignore("wplink");
            a.ignore("srclang");
            a.ignore("nobycat");
            str dot    = a.acquire("dot");
            str nodot  = a.acquire("nodot");
            str nocap  = a.acquire("nocap");
            str notext = a.acquire("notext");
            str born   = a.acquire("born");
            str died   = a.acquire("died");
            str alt    = a.acquire("alt");
            str tr     = a.acquire("tr");
            str in     = a.acquire("in");

            auto
            nats  = a.acquire_all("nationality");
            nats += a.acquire_all("nat");
            str nat = str::list(nats,  ", ", " and ");
            
            auto
            occs  = a.acquire_all("occupation");
            occs += a.acquire_all("occ");
            str occ = str::list(occs,  ", ", " and ");

            output = str(
                alt != "" ? alt :
                notext != "" ? "" : 
                name == "coinage"     ? "coined by "     :
                name == "named-after" ? "named after of ":
                "").capitalized(nocap == "");

            if (nat   != "") output += nat + " ";
            if (occ   != "") output += occ + " ";

            output += str::list(a.unnamed, ", ", " and ");

            if (tr    != "") output += " ("+tr+")";            
            if (born  != "") output += " ("+born+"–"+died+")";
            if (in    != "") output += " in " + in;
            if (nodot != "") output += dot == "" ? "." : dot;
        }
        else
        {
            a.kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }

        if (a.kind != "{{}}") 
        if (not a.opt.empty()) a.kind += " opt";

        if (a.kind.contains(" quest")) a.kind += " !!!!!";
        if (output.contains("\n")) a.kind +=  " #br#";
        if (output.contains("\n")) report = "==== "+title+" ==== "+header+" ==== "+"\n\n" + report;
        if (output.contains("\n")) output.replace_all("\n", " ");
        if (a.kind != "{{}}") result.report (report + " => " + output + " == " + title, a.kind);
        return output;
    }

    Pass <entry, entry> templates_names = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                for (auto & line : content)
                {
                    bracketer b;
                    b.proceed_template = [&](str s){ return templates_names_(t, h, s, result); };
                    b.proceed(line);
                    line = b.output;
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
