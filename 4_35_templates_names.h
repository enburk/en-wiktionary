#pragma once
#include "4.h"
namespace pass4
{
    str templates_names_(str title, str header, str body, Result<entry> & result)
    {
        args args (body, false); str name = args.name; str arg = args.body; auto & a = args;

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        if (body == "QUOTE" or
            body == "rfdate" or
            body == "RQ") return output;

        if (a.unnamed.size() > 0) { if (not Languages.contains(a[0])) kind += " quest lang";
            a.unnamed.erase(0);
            a.complexity--;
        }

        if (name == "surname" or
            name == "given name")
        {
            str q;
            str A = a.acquire("a"); if (A == "") A = "A";

            array<str> dims;
            q = a.acquire("dim");        if (q != "") dims += q;
            q = a.acquire("diminutive"); if (q != "") dims += q;
            q = a.acquire("dim1");       if (q != "") dims += q;
            q = a.acquire("dim2");       if (q != "") dims += q;
            q = a.acquire("dim3");       if (q != "") dims += q;
            q = a.acquire("dim4");       if (q != "") dims += q;
            str dim = str::list(dims);

            array<str> froms;
            q = a.acquire("from");       if (q != "") froms += q;
            q = a.acquire("from1");      if (q != "") froms += q;
            q = a.acquire("from2");      if (q != "") froms += q;
            q = a.acquire("from3");      if (q != "") froms += q;
            q = a.acquire("from4");      if (q != "") froms += q;
            q = a.acquire("from5");      if (q != "") froms += q;
            for (str & s : froms)
            if (s.contains(":")) {
                str l, n; s.split_by(":", l, n);
                s = Languages[l] + " " + n;
                kind += " lang";
            }
            str from = str::list(froms);

            array<str> vars;
            q = a.acquire("var");        if (q != "") vars += q;
            q = a.acquire("var1");       if (q != "") vars += q;
            q = a.acquire("var2");       if (q != "") vars += q;
            str var = str::list(vars);

            array<str> eqs;
            q = a.acquire("eq");        if (q != "") eqs += q;
            q = a.acquire("eq1");       if (q != "") eqs += q;
            q = a.acquire("eq2");       if (q != "") eqs += q;
            str eq = str::list(eqs);

            str usage = a.acquire("usage");
            str dot   = a.acquire("dot"  );
            str nodot = a.acquire("nodot");
            str dimin = dim == "" ? " " : " diminutive of the ";

            str what;
            if (a.unnamed.size() == 0) { ; } else 
            if (a.unnamed.size() >= 1 and a[0] == "") { ; } else
            if (a.unnamed.size() >= 1 and a[0] != "") { what = a[0];
                if (a[0].starts_with("English") and A == "A") A = "An"; } else
                kind += " quest";

            str gen   = a.acquire("gender"); if (gen != "") { what = gen; kind += " gen"; }
            str gor   = a.acquire("or"); if (gor != "") { what += " or " + gor; kind += " or"; }

            //if (a.unnamed.size() >= 1 and a[0] == "en") { ; } else kind += " quest";

            if (what != "") what += " "; what += name; output = A + dimin + what;

            //if (dims .size() > 1) kind += " dim";
            //if (froms.size() > 1) kind += " from";
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
        if (name == "place")
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

    Pass <entry, entry> templates_names = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("templates3", nn, input.cargo);

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

        dump_templates_statistics(result);
    };
}
