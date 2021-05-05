#pragma once
#include "3.h"
namespace pass3
{
    Pass <entry, entry> lexforms1a = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
            for (auto & [header, forms, content] : topic)
            {
                if (header != "adjective" and
                    header != "adverb")
                    continue;

                str report = "adv ";
                str & r = report;
                str & f = forms;
                str & t = title;

                auto suffix = [&] (str kind, str form="", bool full=false)
                {
                    if (form == "" and kind == "more") form = "more " + t; else
                    if (form == "" and kind == "most") form = "most " + t; else
                    if (form == "") {
                        form = t;
                        if (form.size() >= 3)
                        {
                            char x = form[form.size()-3];
                            char y = form[form.size()-2];
                            char z = form[form.size()-1]; if (y == 'e' and z == 'y')
                            if (x != 'a' && x != 'e' && x != 'i' && x != 'o' && x != 'u')
                            { form.truncate(); form.truncate(); form += "i"; }
                        }
                        if (form.size() >= 2)
                        {
                            char x = form[form.size()-2];
                            char y = form[form.size()-1]; if (y == 'y')
                            if (x != 'a' && x != 'e' && x != 'i' && x != 'o' && x != 'u')
                            { form.truncate(); form += "i"; }
                        }
                        if (form.ends_with("e")) { form.truncate(); }
                        form += kind;
                    }

                    std::lock_guard lock{lexforms_mutex};

                    bool found = false; str list;
                    for (auto & [k, ack, w] : lexforms[t]) { bool aha = false
                        or  kind == "er"   and k.contains("comparative form of")
                        or  kind == "est"  and k.contains("superlative form of")
                        or  kind == "more" and k.contains("comparative form of")
                        or  kind == "most" and k.contains("superlative form of");
                        if (aha and w == form) { found = true; ack = "+"; }
                        if (aha) list += "[" + w + "]";
                    }

                    if (kind == "er" or kind == "est")
                    if (not found and form != t and list != "") result.report(
                        t + " | " + f + " => " + form + " <> " + list,
                        "- conflicts " + kind + (full? str("--") : str("++")));

                    if (kind == "er" or kind == "est")
                    if (not found and form != t and list == "") result.report(
                        t + " | " + f + " => " + form,
                        "- unacknowledged " + kind);

                    if (kind == "er")
                    if (not found and form != t)
                        lexforms[t] += lexform{"comparative form of", "?", form};

                    if (kind == "est")
                    if (not found and form != t)
                        lexforms[t] += lexform{"superlative form of", "?", form};

                    return "'''" + form + "'''";
                };

                auto er   = [&](str form="", bool full=false){ return suffix("er"  , form, full); };
                auto est  = [&](str form="", bool full=false){ return suffix("est" , form, full); };
                auto more = [&](str form="", bool full=false){ return suffix("more", form, full); };
                auto most = [&](str form="", bool full=false){ return suffix("most", form, full); };

                auto cmp = [](str a, str b = "", str c = ""){
                    str s  =  "''comparative'' " + a;
                    if (b != "") s += " ''or'' " + b;
                    if (c != "") s += " ''or'' " + c;
                    return s;
                };
                auto sup = [](str a, str b = "", str c = ""){
                    str s  =  "''superlative'' " + a;
                    if (b != "") s += " ''or'' " + b;
                    if (c != "") s += " ''or'' " + c;
                    return s;
                };

                auto same = [](str s1, str s2) {
                    return s1.upto(2) == s2.upto(2) or
                        (s1.starts_with("wors") and s2.starts_with("bad" )) or
                        (s1.starts_with("best") and s2.starts_with("good")) or
                        (s1.starts_with("bett") and s2.starts_with("good")) or
                        (s1.starts_with("best") and s2.starts_with("well")) or
                        (s1.starts_with("bett") and s2.starts_with("well")) or
                        (s1.starts_with("more") and s2.starts_with("many")) or
                        (s1.starts_with("most") and s2.starts_with("many")) or
                        (s1.starts_with("more") and s2.starts_with("much")) or
                        (s1.starts_with("most") and s2.starts_with("much")) or
                        false; };

                if (f == ""       ) { r += "0 "; } else
                if (f == "?"      ) { r += "0 _";       f = ""; } else
                if (f == "-"      ) { r += "0 -";       f = "''not comparable''"; } else
                if (f == "+"      ) { r += "0 +";       f = cmp(more()) + ", " + sup(most()); } else
                if (f == "#"      ) { r += "0 #";       f = cmp(more()) + ", " + sup(most()); } else
                if (f == "more"   ) { r += "0 more";    f = cmp(more()) + ", " + sup(most()); } else
                if (f == "er"     ) { r += "0 er";      f = cmp(er  ()) + ", " + sup(est ()); } else
                if (f == "er|more") { r += "0 er+more"; f = cmp(er(),more()) + ", " + sup(est(),most()); } else
                if (f == "more|er") { r += "0 more+er"; f = cmp(more(),er()) + ", " + sup(most(),est()); } else
                if (f == "-|more" ) { r += "0 -more";   f = "''not generally comparable'', " + cmp(more()) + ", " + sup(most()); } else
                if (f == "-|-"    ) { r += "0 --";      f = "''not generally comparable''"; } else
                {
                    str result; array<str> cc, ss; bool and_more = false;

                    if (f.starts_with("-|")) {
                        f.upto(2).erase();
                        result = "''not generally comparable'', ";
                    }
                    if (f.ends_with("|more") and t != "many" and t != "much") {
                        f.truncate(); f.truncate(); f.truncate();
                        f.truncate(); f.truncate();
                        and_more = true;
                    }
                    if (f.starts_with("more|") and t != "many" and t != "much") {
                        f.upto(5).erase();
                        cc += more();
                        ss += most();
                    }

                    args args ("adv|"+f);

                    if (args.complexity == 1)
                    {
                        r += "1";
                        str c = args[0];
                        if (c == "further" and t != "far") {
                            cc += "'''further " + t + "'''";
                            ss += "'''furthest "+ t + "'''";
                        }
                        else
                        if (c == "er") {
                            cc += er();
                            ss += est();
                        }
                        else {
                            if (not same(c, t)) r += " quest"; else {
                            cc += er (c, true); c.truncate(); c += "st";
                            ss += est(c, true); }
                        }
                    }
                    else
                    if (args.complexity == 11)
                    {
                        r += "11";
                        str c = args[0];
                        str s = args.opt["sup"];
                        if (s == "") s = args.opt["sup2"];
                        if (s == ""
                            or not same(c, t)
                            or not same(s, t)) r += " quest";
                        else {
                            cc += er (c, true);
                            ss += est(s, true); }
                    }
                    else r += "quest";

                    if (and_more) {
                        cc += more();
                        ss += most();
                    }

                    str ccc; int cn = cc.size();
                    str sss; int sn = ss.size();
                    for (int i=0; i<cn; i++) { ccc += cc[i]; if (i != cn-1) ccc += " ''or'' "; }
                    for (int i=0; i<sn; i++) { sss += ss[i]; if (i != sn-1) sss += " ''or'' "; }
                    result += cmp(ccc) + ", " + sup(sss);
                        
                    if (args.complexity >= 10) r += " complex";
                        
                    if (not r.contains("quest")) f = result;
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
