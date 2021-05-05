#pragma once
#include "3.h"
namespace pass3
{
    Pass <entry, entry> lexforms1n = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("lexforms1n", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                if (header != "noun" and
                    header != "proper noun")
                    continue;

                str report = "noun ";
                str & r = report;
                str & f = forms;
                str & t = title;

                auto plural = [&] (str form="", bool full=false)
                {
                    if (form == "") {
                        form = t;
                        if (form.ends_with("s" ) or
                            form.ends_with("ss") or
                            form.ends_with("sh") or
                            form.ends_with("ch") or
                            form.ends_with("x" ) or
                            form.ends_with("z" ))
                            form += "es";
                        else
                        if (form.ends_with("ay") or
                            form.ends_with("ey") or
                            form.ends_with("iy") or
                            form.ends_with("oy") or
                            form.ends_with("uy"))
                            form += "s";
                        else
                        if (form.ends_with("y")) {
                            form.truncate();
                            form += "ies"; }
                        else
                            form = form + "s";
                    }

                    std::lock_guard lock{lexforms_mutex};

                    bool found = false; str plurals;
                    for (auto & [kind, ack, w] : lexforms[t]) {
                        if (kind == "plural of") plurals += "["+w+"]";
                        if (kind == "plural of" and w == form)
                            { found = true; ack = "+"; }
                    }

                    if (not found and form != t and plurals != "") result.report(
                        t + " | " + f + " => " + form + " <> " + plurals,
                        "- conflicts" + (full? str("--") : str("++")));

                    if (not found and form != t and plurals == "") result.report(
                        t + " | " + f + " => " + form,
                        "- unacknowledged");

                    if (not found and form != t)
                        lexforms[t] += lexform{"plural of", "?", form};

                    return " '''" + form + "'''";
                };

                auto same = [](str s1, str s2) {
                    return s1.upto(2) == s2.upto(2) or
                        (s1.starts_with("man") and s2.starts_with("men")) or
                        (s1.starts_with("goo") and s2.starts_with("gee")) or
                        false; };

                if (f == "#" and header == "proper noun") f = "";

                if (f.starts_with("##")) {
                    f.upto(2).erase();
                    r = "plural only";
                    if (f.starts_with("sg=")) { r += " sg"; f = f.from(3);
                    f = "''normally plural, singular'' '''" + f + "'''"; }
                    else if (f != "" and f != t) { r += " quest"; f = ""; }
                    else f = "''plural only''";
                }
                else
                if (f == ""    ) { r += "0 "; } else
                if (f == "?"   ) { r += "0 _";  f = ""; } else
                if (f == "-"   ) { r += "0 -";  f = "''uncountable''"; } else
                if (f == "!"   ) { r += "0 !";  f = "''plural not attested''"; } else
                if (f == "#"   ) { r += "0 #";  f = "''plural''" + plural(); } else
                if (f == "+"   ) { r += "0 +";  f = "''plural''" + plural(); } else
                if (f == "s"   ) { r += "0 s";  f = "''plural''" + plural(t+"s"); } else
                if (f == "es"  ) { r += "0 es"; f = "''plural''" + plural(t+"es"); } else
                if (f == "~"   ) { r += "0 ~";  f = "''countable and uncountable, plural''" + plural(); } else
                {
                    args args ("noun|"+f);

                    if (args.complexity == 1)
                    {
                        r += "1";
                        if (args[0] == "uncountable" or
                            args[0] == "undefined" or
                            args[0] == "none" or
                            args[0] == "unc") { r += " uncountable"; f = "''uncountable''"; } else
                        {
                            if (not same(args[0], t)) r += " quest";
                            f = "''plural''" + plural(args[0], true);
                        }
                    }
                    else
                    {
                        r += "2";
                        str s; array<str> plurals;

                        if (f.starts_with("-|")) { f.upto(2).erase(); s = "''usually uncountable, plural''"; } else
                        if (f.starts_with("~|")) { f.upto(2).erase(); s = "''countable and uncountable, plural''"; } else
                        s = "''plural''";

                        args = ::args("noun|"+f);

                        for (auto arg : args.unnamed)
                        {
                            if (arg == "?"  ) ; else
                            if (arg == "+"  ) plurals += plural(); else
                            if (arg == "s"  ) plurals += plural(t+"s"); else
                            if (arg == "es" ) plurals += plural(t+"es"); else
                            if (same(arg, t)) plurals += plural(arg, true); else
                            if (not r.contains("quest")) r += " quest";
                        }

                        int n = plurals.size();

                        for (auto [key, value] : args.opt)
                        {
                            if (key == "plqual"  and n >= 1) plurals[0] = " ''("+value+")''" + plurals[0]; else
                            if (key == "pl1qual" and n >= 1) plurals[0] = " ''("+value+")''" + plurals[0]; else
                            if (key == "pl2qual" and n >= 2) plurals[1] = " ''("+value+")''" + plurals[1]; else
                            if (key == "pl3qual" and n >= 3) plurals[2] = " ''("+value+")''" + plurals[2]; else
                            if (key == "pl4qual" and n >= 4) plurals[3] = " ''("+value+")''" + plurals[3]; else
                            if (key == "pl5qual" and n >= 5) plurals[4] = " ''("+value+")''" + plurals[4]; else
                            if (key == "pl6qual" and n >= 6) plurals[5] = " ''("+value+")''" + plurals[5]; else
                            if (key == "pl7qual" and n >= 7) plurals[6] = " ''("+value+")''" + plurals[6]; else
                            if (key == "pl8qual" and n >= 8) plurals[7] = " ''("+value+")''" + plurals[7]; else
                            if (key == "pl9qual" and n >= 9) plurals[8] = " ''("+value+")''" + plurals[8]; else
                            if (not r.contains("quest")) r += " quest";
                        }

                        for (int i=0; i<n; i++)
                        {
                            s += plurals[i];
                            if (i <= n-1-2) s += ","; else
                            if (i == n-1-1) s += " ''or''";
                        }

                        if (args.complexity >= 10) r += " complex";

                        if (not r.contains("quest")) r += " " + std::to_string(n);

                        f = s;
                    }
                }

                if (f != "") f = "(" + f + ")";

                result.report (t + " == " + header + " == " + f, r);
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
