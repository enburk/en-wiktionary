#pragma once
#include "2.h"
namespace pass2
{
    Pass <entry, entry> lexforms1n = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

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

                    return " '''" + form + "'''";
                };

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
                if (f == "~|s" ) { r += "0 ~s"; f = "''countable and uncountable, plural''" + plural(t+"s"); } else
                if (f == "~|es") { r += "0 ~s"; f = "''countable and uncountable, plural''" + plural(t+"es"); } else
                if (f == "-|s" ) { r += "0 -s"; f = "''usually uncountable, plural''" + plural(t+"s"); } else
                if (f == "-|es") { r += "0 -s"; f = "''usually uncountable, plural''" + plural(t+"es"); } else
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
                            if (args[0].upto(2) != t.upto(2)) r += " quest";
                            f = "''plural''" + plural(args[0], true);
                        }
                    }
                    else
                    if (args.complexity == 2)
                    {
                        r += "2";
                        if (args[0] == "~"  && args[1].upto(2) == t.upto(2)) { r += " ~"; f = "''countable and uncountable, plural''" + plural(args[1]); } else
                        if (args[0] == "-"  && args[1].upto(2) == t.upto(2)) { r += " -"; f = "''usually uncountable, plural''" + plural(args[1]); } else
                        if (args[0] == "s"  && args[1].upto(2) == t.upto(2)) { r += " s"; f = "''plural''" + plural(t+"s") + " ''or''" + plural(args[1]); } else
                        if (args[0] == "es" && args[1].upto(2) == t.upto(2)) { r += " s"; f = "''plural''" + plural(t+"es") + " ''or''" + plural(args[1]); } else
                        if (args[0].upto(2) != t.upto(2)
                        ||  args[1].upto(2) != t.upto(2)) r += " quest";
                        else f = "''plural''" +
                            plural(args[0]) + " ''or''" +
                            plural(args[1]);
                    }
                    else r = "- quest";
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
