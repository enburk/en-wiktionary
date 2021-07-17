#pragma once
#include "3.h"
namespace pass3
{
    const str vowels = "aeiouáéíóúàèìòùâêîôûäëïöüæœø";
	const str VOWELS = "AEIOUÁÉÍÓÚÀÈÌÒÙÂÊÎÔÛÄËÏÖÜÆŒØ";
    const str Vowels = vowels + VOWELS;

    bool last_cons (str s) {
	    const std::regex r1("^[Yy][" + vowels + "y]([^A-Z" + vowels + "ywxh])$");
		const std::regex r2("^[^" + Vowels + "yY]*[" + Vowels + "yY]([^A-Z" + vowels + "ywxh])$");
        return std::regex_match(s, r1) or
               std::regex_match(s, r2); }

    Pass <entry, entry> lexforms1v = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
            for (auto & [header, forms, content] : topic)
            {
                if (header != "verb") continue;
                str report = "verb ";
                str & r = report;
                str & f = forms;
                str & t = title;

                str original_forms = forms;

                auto suffix = [&] (str kind, str form="", bool full=false)
                {
                    form.replace_all("~", t);

                    if (form == "") {
                        form = t;
                        if (kind == "ing")
                        {
                            if (last_cons(form)) form += form.back();
                            else
                            if (std::regex_match(form, std::regex("^(.*)ie$"))) {
                                form.truncate(); form.truncate(); form += "y";
                            }
                            else
                            if (std::regex_match(form, std::regex("^(.*)ue$")) or
                                std::regex_match(form, std::regex("^(.*[" + Vowels + "yY][^" + vowels + "y]+)e$"))) {
                                form.truncate();
                            }
                            form += "ing";
                        }
                        else
                        if (kind == "ed" or kind == "pp")
                        {
                            if (last_cons(form)) form += form.back(); else
                            if (form.ends_with("ay")) { ; } else
                            if (form.ends_with("ey")) { ; } else
                            if (form.ends_with("iy")) { ; } else
                            if (form.ends_with("oy")) { ; } else
                            if (form.ends_with("uy")) { ; } else
                            if (form.ends_with("y" )) { form.truncate(); form += "i"; } else
                            if (form.ends_with("e" )) { form.truncate(); }
                            form += "ed";
                        }
                        else
                        if (kind == "es")
                        {
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
                    }

                    std::lock_guard lock{lexforms_mutex};

                    bool found = false; str list;
                    for (auto & [k, ack, w] : lexforms[t]) { bool aha = false
                        or  kind == "es"  and k.contains("third-person singular simple present")
                        or  kind == "2pr" and k.contains("second-person singular simple present")
                        or  kind == "2pa" and k.contains("second-person singular simple past")
                        or  kind == "ing" and k.contains("present participle")
                        or  kind == "ed"  and k.contains("simple past tense")
                        or  kind == "pp"  and k.contains("past participle");
                        if (aha and w == form) { found = true; ack = "+"; }
                        if (aha) list += "[" + w + "]";
                    }

                    if (not found and form != t and list != "") result.report(
                        t + " | " + f + " => " + form + " <> " + list,
                        "- conflicts " + kind + (full? str("--") : str("++")));

                    if (not found and form != t and list == "") result.report(
                        t + " | " + f + " => " + form,
                        "- unacknowledged " + kind);

                    if (not found and form != t)
                        lexforms[t] += lexform{
                        kind == "es" ? "third-person singular simple present indicative form of":
                        kind == "ing"? "present participle of":
                        kind == "ed" ? "simple past tense of":
                        kind == "pp" ? "past participle of":
                        "some verb form", "?", form};

                    return form;
                };

                auto es  = [&](str form="", bool full=false){ return suffix("es" , form, full); };
                auto ing = [&](str form="", bool full=false){ return suffix("ing", form, full); };
                auto ed  = [&](str form="", bool full=false){ return suffix("ed" , form, full); };
                auto pp  = [&](str form="", bool full=false){ return suffix("pp" , form, full); };

                auto same = [](str s1, str s2) {
                    if (s1.contains("<")) return false;
                    if (s1.contains(">")) return false;
                    return s1.upto(2) == s2.upto(2) or
                        (s2.starts_with("be") and s1.starts_with("ba")) or
                        (s2.starts_with("be") and s1.starts_with("bo")) or
                        (s2.starts_with("bi") and s1.starts_with("bo")) or
                        (s2.starts_with("bu") and s1.starts_with("bo")) or
                        (s2.starts_with("di") and s1.starts_with("du")) or
                        (s2.starts_with("do") and s1.starts_with("di")) or
                        (s2.starts_with("ea") and s1.starts_with("ate")) or
                        (s2.starts_with("ge") and s1.starts_with("go")) or
                        (s2.starts_with("go") and s1.starts_with("went")) or
                        (s2.starts_with("gi") and s1.starts_with("ga")) or
                        (s2.starts_with("gi") and s1.starts_with("go")) or
                        (s2.starts_with("gi") and s1.starts_with("gu")) or
                        (s2.starts_with("fa") and s1.starts_with("fe")) or
                        (s2.starts_with("fi") and s1.starts_with("fo")) or
                        (s2.starts_with("la") and s1.starts_with("lo")) or
                        (s2.starts_with("ly") and s1.starts_with("la")) or
                        (s2.starts_with("ma") and s1.starts_with("mi")) or
                        (s2.starts_with("ri") and s1.starts_with("ra")) or
                        (s2.starts_with("ri") and s1.starts_with("ro")) or
                        (s2.starts_with("ri") and s1.starts_with("ru")) or
                        (s2.starts_with("se") and s1.starts_with("sa")) or
                        (s2.starts_with("se") and s1.starts_with("so")) or
                        (s2.starts_with("si") and s1.starts_with("sa")) or
                        (s2.starts_with("si") and s1.starts_with("su")) or
                        (s2.starts_with("ta") and s1.starts_with("to")) or
                        (s2.starts_with("te") and s1.starts_with("to")) or
                        (s2.starts_with("ye") and s1.starts_with("yo")) or
                        (s2.starts_with("yi") and s1.starts_with("ya")) or
                        (s2.starts_with("we") and s1.starts_with("wo")) or
                        (s2.starts_with("wi") and s1.starts_with("wo")) or
                        false; };

                if (f == ""  ) {
                    result.report (t + " == ", "-");
                    continue;
                }

                args args ("verb|"+f);

                str es_= args.acquire("pres_3sg_qual");
                str ng_= args.acquire("pres_ptc_qual");
                str ed_= args.acquire("past_qual");
                str pp_= args.acquire("past_ptc_qual");

                array<str> ess, ngs, eds, pps;

                auto add = [&r, &args, t, same](array<str> & forms, str kind)
                {
                    str s, q;
                    s = args.acquire(kind); 
                    q = args.acquire(kind + "_qual");
                    s.replace_all("~", t);
                    if (q != "" and s == "") r = "problem";
                    if (s != "" and not same(s, t)) r = "problem";
                    if (q != "" and s != "") s = "'''(" + italic(q) + ")''' " + s;
                    if (s != "") forms += s;
                };

                add(ess, "pres_3sg2");
                add(ngs, "pres_ptc2");
                add(ngs, "pres_ptc3");
                add(eds, "past2");
                add(eds, "past3");
                add(eds, "past4");
                add(pps, "past_ptc2");
                add(pps, "past_ptc3");
                add(pps, "past_ptc4");
                add(pps, "past_ptc5");

                str v1, v2, v3, v4;

                f = str(args.unnamed, "|");

                if (f == "#" )
                {
                    r += "0 #";
                    v1 = es ();
                    v2 = ing();
                    v3 = ed ();
                    v4 = pp ();
                }
                else
                if (f == "++")
                {
                    r += "0 ++";
                    v1 = t.ends_with("s") ? t + "ses" :
                         t.ends_with("z") ? t + "zes" : es();
                    v2 = t + t.back() + "ing";
                    v3 = t + t.back() + "ed";
                    v4 = t + t.back() + "ed";
                    
                }
                else
                if (f == "*" )
                {
                    r += "0 star";
                    str v, s; t.split_by(" ", v, s);
                    if (v != "" and s != "") {
                        str tt = t; t = v;
                        v1 = es () + " " + s;
                        v2 = ing() + " " + s;
                        v3 = ed () + " " + s;
                        v4 = pp () + " " + s;
                        t = tt;
                    }
                }
                else
                if (f == "++*")
                {
                    r += "0 ++";
                    str v, s; t.split_by(" ", v, s);
                    if (v != "" and s != "") {
                        str tt = t; t = v;
                        v1 = v.ends_with("s") ? v + "ses " + s :
                             v.ends_with("z") ? v + "zes " + s : es() + " " + s;
                        v2 = v + v.back() + "ing" + " " + s;
                        v3 = v + v.back() + "ed"  + " " + s;
                        v4 = v + v.back() + "ed"  + " " + s;
                        t = tt;
                    }
                }
                else
                if (args.complexity == 0)
                {
                    r += "0";
                    v1 = es ();
                    v2 = ing();
                    v3 = ed ();
                    v4 = pp ();
                }
                else
                if (args.complexity == 1)
                {
                    r += "1";
                    str v, s; f.split_by(" ", v, s);
                    if (v == "get<,,got,got[UK]:gotten[US]>"){
                        r += "get";
                        v1 = "gets " + s;
                        v2 = "getting " + s;
                        v3 = "got " + s;
                        v4 = "'''(''UK'')''' got " + s + ", " +
                                "'''(''US'')''' gotten " + s;
                    }
                    else
                    if (v == "beat<,,beat,beaten:beat[colloquial]>"){
                        r += "beat";
                        v1 = "beats " + s;
                        v2 = "beating " + s;
                        v3 = "beat " + s;
                        v4 = "beaten " + s + ", " +
                                "'''(''colloquial'')''' beat " + s;
                    }
                    else
                    if (v.ends_with(">") and not
                        s.contains ("<") and not
                        v.contains ("[")) {
                        r += "1";
                        v.truncate();
                        str x; v.split_by("<", v, x);
                        auto xx = x.split_by(",");
                        if (xx.size() == 4) {
                            str tt = t; t = v;
                            v1 = (xx[0] == "" ? es () : xx[0]) + " " + s;
                            v2 = (xx[1] == "" ? ing() : xx[1]) + " " + s;
                            v3 = (xx[2] == "" ? ed () : xx[2]) + " " + s;
                            v4 = (xx[3] == "" ? pp () : xx[3]) + " " + s;
                            t = tt;
                        }
                        if (xx.size() == 3) {
                            str tt = t; t = v;
                            v1 = (xx[0] == "" ? es () : xx[0]) + " " + s;
                            v2 = (xx[1] == "" ? ing() : xx[1]) + " " + s;
                            v3 = (xx[2] == "" ? ed () : xx[2]) + " " + s;
                            v4 = (xx[2] == "" ? pp () : xx[2]) + " " + s;
                            t = tt;
                        }
                    }
                }
                else
                if (args.complexity == 3)
                {
                    r += "3";
                    if (same(args[0], t)
                    and same(args[1], t)
                    and same(args[2], t)) {
                    v1 = es (args[0], true);
                    v2 = ing(args[1], true);
                    v3 = ed (args[2], true);
                    v4 = pp (args[2], true); }
                }
                else
                if (args.complexity == 4)
                {
                    r += "4";
                    if (same(args[0], t)
                    and same(args[1], t)
                    and same(args[2], t)
                    and same(args[3], t)) {
                    v1 = es (args[0], true);
                    v2 = ing(args[1], true);
                    v3 = ed (args[2], true);
                    v4 = pp (args[3], true); }
                    else {
                        r += "4";
                        if (same(args[0], "")
                        and same(args[1], "")
                        and same(args[2], "")
                        and same(args[3], t)) {
                        v1 = es ();
                        v2 = ing();
                        v3 = ed ();
                        v4 = pp (args[3], true); }
                    }
                }

                if (v1 != "" and v1 != "" and v3 != "" and v4 != "")
                {
                    if (not ngs.empty() or
                        not eds.empty() or
                        not pps.empty()) r = "complex1";

                    if (es_ != "" or
                        ng_ != "" or
                        ed_ != "" or
                        pp_ != "") r = "complex2";

                    v1 = bold(v1);
                    v2 = bold(v2);
                    v3 = bold(v3);
                    v4 = bold(v4);

                    if (es_ != "") v1 = "(" + italic(es_) + ") " + v1;
                    if (ng_ != "") v2 = "(" + italic(ng_) + ") " + v2;
                    if (ed_ != "") v3 = "(" + italic(ed_) + ") " + v3;
                    if (pp_ != "") v4 = "(" + italic(pp_) + ") " + v4;

                    for (str s : ess) v1 += ", " + bold(s);
                    for (str s : ngs) v2 += ", " + bold(s);
                    for (str s : eds) v3 += ", " + bold(s);
                    for (str s : pps) v4 += ", " + bold(s);
                }

                if (v1 != "" and v1 != "" and v3 != "" and v4 != "")
                f = "(" + v1 + "; " + v2 + "; " + v3 + "; " + v4 + ")";
                else r += " quest";

                result.report (t + " == " + original_forms + " == " + f, r);
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
