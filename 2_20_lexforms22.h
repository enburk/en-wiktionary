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
        str first = lexical_form.upto(1);
        str dot   = args.acquire("dot"  );
        str nodot = args.acquire("nodot"); if (nodot == "yes" || nodot == "y" || nodot == "t" || nodot == "a") nodot = "1";
        str nocap = args.acquire("nocap"); if (nocap == "yes" || nocap == "y" || nocap == "t" || nocap == "a") nocap = "1";
        if (dot != "" && dot != "." && dot != "," && dot != ":" && dot != ";") { opts += " - dot"; }
        if (nodot != "" && nodot != "1") { opts += " - nodot"; }
        if (nocap != "" && nocap != "1") { opts += " - nocap"; }
        if (nocap != "") lexical_form.upto(1).replace_by(first.ascii_lowercased());
        if (dot   == "") dot = first == first.ascii_lowercased() ? "" : ".";
        if (nodot != "") dot = "";

        if (opts  != "") report = opts;

        str from  = args.acquire("from" );
        if (from  != "") report = " - from";
        if (from  != "") args.complexity += 10;

        str a1, a2, a3;
        args.ignore("t"); args.ignore("tr"); args.ignore("gloss"); args.ignore("pos"); args.ignore("id");
        if (args.unnamed.size() > 0 && args[0] != "") a1 = "'''"+args[0]+"'''"; 
        if (args.unnamed.size() > 1 && args[1] != "") a2 = "'''"+args[1]+"'''"; 
        if (args.unnamed.size() > 2 && args[2] != "") a3 = "("+oquot+args[2]+cquot+")";

        str out;
        if (args.complexity == 1 && a1 != "") { out = a1; report = "1" + opts; } else
        if (args.complexity == 2 && a2 == "") { out = a1; report = "2" + opts; } else
        if (args.complexity == 2 && a2 != "") { out = a2; report = "2" + opts; } else
        if (args.complexity == 3 && a1 == "" && a2 != "") { out = a2 + " " + a3; report = "3" + opts; } else
        if (args.complexity == 3 && a1 != "" && a2 == "") { out = a1 + " " + a3; report = "3" + opts; } else
        if (args.complexity == 3 && a1 != "" && a2 != "") { out = a2 + " " + a3; report = "3" + opts; } else
        {}

        if (out != "")
        {
            output = "''" + lexical_form + "'' " + out + dot;

            out = a2 != "" ? a2 : a1;

            if (out.starts_with("W:")) out.upto(2).erase();
            if (out.starts_with("w:")) out.upto(2).erase();
            if (out.starts_with("s:")) out.upto(2).erase();

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
