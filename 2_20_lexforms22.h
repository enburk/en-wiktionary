#pragma once
#include "2.h"
#include "2_20_lexforms2.h"
namespace pass2
{
    str lexforms22_(str title, str header, str & forms, str body, Result<entry> & result)
    {
        args args (body);

        auto it = lexforms2_internal::list_of_forms.find(args.name);
        if (it == lexforms2_internal::list_of_forms.end()) return "{{" + body + "}}";
        str lexical_form = it->second;
        str output = "{{" + body + "}}";
        str report = lexical_form;

        str opts;
        str first  = lexical_form.upto(1);
        str dot    = args.acquire("dot");
        str cap    = args.acquire("cap"); // ignore it
        str ending = args.acquire("ending"); // ignore it
        str nodot  = args.acquire("nodot"); if (nodot == "yes" || nodot == "y" || nodot == "t" || nodot == "a") nodot = "1";
        str nocap  = args.acquire("nocap"); if (nocap == "yes" || nocap == "y" || nocap == "t" || nocap == "a") nocap = "1";

        if (cap    != "") { opts += " - cap"; }
        if (ending != "") { opts += " - ending"; }
        if (dot != "" && dot != "." && dot != "," && dot != ":" && dot != ";") { opts += " - dot"; }
        if (nodot != "" && nodot != "1") { opts += " - nodot"; }
        if (nocap != "" && nocap != "1") { opts += " - nocap"; }

        if (nocap != "") lexical_form.upto(1).replace_by(first.ascii_lowercased());
        if (dot   == "") dot = first == first.ascii_lowercased() ? "" : ".";
        if (nodot != "") dot = "";

        if (opts  != "") report = opts;

        array<str> froms; str
        from = args.acquire("from" ); if (from != "") froms += from.split_by(", ");
        from = args.acquire("from2"); if (from != "") froms += from.split_by(", ");
        from = args.acquire("from3"); if (from != "") froms += from.split_by(", ");
        from = args.acquire("from4"); if (from != "") froms += from.split_by(", ");
        from = args.acquire("from5"); if (from != "") froms += from.split_by(", ");
        from = args.acquire("from6"); if (from != "") froms += from.split_by(", ");

        str extra;
        if (not froms.empty())
        {
            for (str from : froms) {
                if (from == "AU") from = "Australia";
                if (from == "NZ") from = "New Zealand";
                if (from == "UK") from = "Britain";
                if (from == "AAVE") from = "African-American Vernacular English";
                if (from == "American") from = "American English";
                if (from == "Caribbean") from = "Caribbean English";
                if (from == "colloquial") from = "colloquial English";
                if (from == "dialect") from = "dialect English";
                if (from == "dialectal") from = "dialect English";
                if (from == "NYC") from = "New York City English";
                if (from == "Non-Oxford") from = "Non-Oxford British English";
                if (from == "southern US black") from = "southern US black English";
                if (from.ends_with(" in")) from += " English";
                if (extra != "")
                    extra += ", ";
                extra += from;
            }
            extra = ", ''representing " + extra + "''";
        }

        str a1, a2, a3;
        args.ignore("t"); args.ignore("tr"); args.ignore("gloss"); args.ignore("pos"); args.ignore("id");
        if (args.unnamed.size() > 0 && args[0] != "") a1 = "'''"+args[0]+"'''"; 
        if (args.unnamed.size() > 1 && args[1] != "") a2 = "'''"+args[1]+"'''"; 
        if (args.unnamed.size() > 2 && args[2] != "") a3 = "("+oquot+args[2]+cquot+")";

        if (a1.starts_with("W:")) a1.upto(2).erase();
        if (a1.starts_with("w:")) a1.upto(2).erase();
        if (a1.starts_with("s:")) a1.upto(2).erase();

        if (a2.starts_with("W:")) a2.upto(2).erase();
        if (a2.starts_with("w:")) a2.upto(2).erase();
        if (a2.starts_with("s:")) a2.upto(2).erase();

        str out;
        if (args.complexity == 1 && a1 != "") { out = a1; report = "1" + opts; } else
        if (args.complexity == 2 && a2 == "") { out = a1; report = "2" + opts; } else
        if (args.complexity == 2 && a2 != "") { out = a2; report = "2" + opts; } else
        if (args.complexity == 3 && a1 == "" && a2 != "") { out = a2 + " " + a3; report = "3" + opts; } else
        if (args.complexity == 3 && a1 != "" && a2 == "") { out = a1 + " " + a3; report = "3" + opts; } else
        if (args.complexity == 3 && a1 != "" && a2 != "") { out = a2 + " " + a3; report = "3" + opts; } else
        {}

        if (not froms.empty()) report = " - from " + std::to_string(froms.size());

        if (out != "")
        {
            output = out;
            if (not output.starts_with("'''")) output = "'''" + output;
            if (not output.ends_with  ("'''")) output = output + "'''";
            output = "''" + lexical_form + "'' " + output + dot;
            if (extra != "" and output.ends_with(".")) output.truncate();
            output += extra;

            out = a2 != "" ? a2 : a1;

            lexforms[out] += lexform{lexical_form, "-", title};

            out.replace_all("[[", "");
            out.replace_all("]]", "");

            if (not out.contains_only(str::one_of(ALnum))) report = "- alnum1";
            if (not out.contains_only(str::one_of(ALNUM))) report = "- alnum2";
        }

        else report += " - quest";

        result.report (title + " ==== " + header + " ==== {{" + body + "}}", report);
        return output;
    }

    Pass<entry, entry> lexforms22 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

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
