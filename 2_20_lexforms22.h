#pragma once
#include "2.h"
#include "2_20_lexforms2.h"
namespace pass2
{
    str lexforms22_(str title, str header, str & forms, str body, Result<entry> & result)
    {
        args args (body);//, false);

        auto it = lexforms2_internal::list_of_forms.find(args.name);
        if (it == lexforms2_internal::list_of_forms.end()) return "{{" + body + "}}";
        str lexical_form = it->second;
        str output = "{{" + body + "}}";
        str kind = "";

        str opts;
        str first  = lexical_form.upto(1);
        str dot    = args.acquire("dot");
        str cap    = args.acquire("cap"); // ignore it
        str ending = args.acquire("ending"); // ignore it
        str nodot  = args.acquire("nodot"); if (nodot == "yes" || nodot == "y" || nodot == "t" || nodot == "a") nodot = "1";
        str nocap  = args.acquire("nocap"); if (nocap == "yes" || nocap == "y" || nocap == "t" || nocap == "a") nocap = "1";
        str notext = args.acquire("notext");

        if (cap    != "") { opts += " - cap"; }
        if (ending != "") { opts += " - ending"; }
        if (dot != "" && dot != "." && dot != "," && dot != ":" && dot != ";") { opts += " - dot"; }
        if (nodot != "" && nodot != "1") { opts += " - nodot"; }
        if (nocap != "" && nocap != "1") { opts += " - nocap"; }

        if (nocap != "") lexical_form.upto(1).replace_by(first.ascii_lowercased());
        if (dot   == "") dot = first == first.ascii_lowercased() ? "" : ".";
        if (nodot != "") dot = "";

        if (opts  != "") kind = opts;

        str q;
        auto & a = args;
        //if (a.unnamed.size() > 0) { if (not Languages.contains(a[0])) kind = "quest lang";
        //    a.unnamed.erase(0);
        //    a.complexity--;
        //}

        array<str> froms; str
        from = args.acquire("from" ); if (from != "") froms += from.split_by(", ");
        from = args.acquire("from2"); if (from != "") froms += from.split_by(", ");
        from = args.acquire("from3"); if (from != "") froms += from.split_by(", ");
        from = args.acquire("from4"); if (from != "") froms += from.split_by(", ");
        from = args.acquire("from5"); if (from != "") froms += from.split_by(", ");
        from = args.acquire("from6"); if (from != "") froms += from.split_by(", ");

        for (str & from : froms) {
            if (from == "AU") from = "Australia";
            if (from == "NZ") from = "New Zealand";
            if (from == "UK") from = "Britain";
            if (from == "AAVE") from = "African-American Vernacular English";
            if (from == "American") from = "American English";
            if (from == "Caribbean") from = "Caribbean English";
            if (from == "colloquial") from = "colloquial English";
            if (from == "dialect") from = "dialectal English";
            if (from == "dialectal") from = "dialectal English";
            if (from == "Dialectal") from = "dialectal English";
            if (from == "NYC") from = "New York City English";
            if (from == "Non-Oxford") from = "Non-Oxford British English";
            if (from == "southern US black") from = "southern US black English";
        }
        str extra = str::list(froms, ", ", " and ");
        if (extra.ends_with(" in") or
            extra.ends_with("American") or
            extra.ends_with("British") or
            extra.ends_with("Canadian") or
            extra.ends_with("Australian"))
            extra += " English"; 

        if (a.unnamed.size() > 1 and a[1] != "") { output = a[1];  kind += " 2"; } else
        if (a.unnamed.size() > 0 and a[0] != "") { output = a[0];  kind += " 1"; } else
                                                 { output = "(?)"; kind += " 0"; }

        {
            std::lock_guard lock{lexforms_mutex};
            lexforms[output] += lexform{lexical_form, "-", title};
        }

        q = a.acquire("alt"); if (q != "") output = q;

        if (not output.contains("[[")
        and not output.contains("]]"))
        output = "[[" + output + "]]";
        output = "'''" + output + "'''";

        a.ignore("gloss"); a.ignore("pos"); a.ignore("id"); a.ignore("g");
        str tr = a.acquire("tr"); // transcript
        str tt = a.acquire("t"); // translation
        q = a.acquire("ts"); if (q != "") tr = q;
        if (a.unnamed.size() >= 3 and a[2] != "") tt = a[2];
        if (tr == "-") tr = "";
        if (tt == "-") tt = "";
        if (tr != "") tr = "''"+tr+"''";
        if (tt != "") tt = "“" +tt+ "”";
        q = a.acquire("lit"); if (q != "") tt = "literally “" +q+ "”";
        if (tr == "" and tt != "") { output += " ("+tt+")"; kind += "t"; } else
        if (tr != "" and tt == "") { output += " ("+tr+")"; kind += "t"; } else
        if (tr != "" and tt != "") { output += " ("+tr+", "+tt+")"; kind += "t"; }

        if (notext == "")
        output = "''" + lexical_form + "'' " + output;
        output += dot;

        if (extra != "" and
           (lexical_form == "Standard spelling of" or
            lexical_form == "standard spelling of")) {
            extra[0] = str::ascii_toupper(extra[0]);
            output[2] = str::ascii_tolower(output[2]);
            if (output.ends_with(" spelling"))
                output.resize(output.size()-9);
            output = "''" + extra + "'' " + output;
            kind = "extra standard";
        }
        else if (extra != "") {
            if (output.ends_with(".")) output.truncate();
            output += ", ''representing " + extra + "''";
            kind = "extra";
        }

        if (a.complexity >= 4) kind += " quest";

        result.report("{{" + body + "}} => " + output + " ==== " + title + " ==== " + header, kind);
        return output;
    }

    Pass<entry, entry> lexforms22 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        std::set<entry> entries;

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0)
                logout("lexforms22", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;
                auto & f = forms;

                bracketer b;
                b.proceed_sbrakets  = [&] (str s) { return   "[" + s + "]"  ; };
                b.proceed_qbrakets  = [&] (str s) { return   "{" + s + "}"  ; };
                b.proceed_link      = [&] (str s) { return  "[[" + s + "]]" ; };
                b.proceed_parameter = [&] (str s) { return "{{{" + s + "}}}"; };
                b.proceed_template  = [&] (str s) { return lexforms22_(t, h, f, s, result); };
                b.proceed(content);
        
                content = std::move(b.output);
            }

            entries.emplace(entry{
                std::move(title),
                std::move(topic)});
        }

        for (auto && [title, topic] : entries)
        {
            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
