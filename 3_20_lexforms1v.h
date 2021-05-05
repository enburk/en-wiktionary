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

        bool first_time = true;

        for (auto && [title, topic] : input)
        {
            if (result.on and
                first_time) {
                first_time = false;
                std::ofstream fstream("data/modules en-.txt");
                for (auto & [name, text] : Modules) 
                if (name.starts_with("en-")) { fstream
                    << esc << "\n" << ("Module:" + name)  << "\n"
                    << esc << "\n" << text << "\n";
                    Modules.erase(name);
                }
            }

            for (auto & [header, forms, content] : topic)
            {
                if (header != "verb") continue;
                str report = "verb ";
                str & r = report;
                str & f = forms;
                str & t = title;

                auto suffix = [&] (str kind, str form="", bool full=false)
                {
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

                    return "'''" + form + "'''";
                };

                auto es  = [&](str form="", bool full=false){ return suffix("es" , form, full); };
                auto ing = [&](str form="", bool full=false){ return suffix("ing", form, full); };
                auto ed  = [&](str form="", bool full=false){ return suffix("ed" , form, full); };
                auto pp  = [&](str form="", bool full=false){ return suffix("pp" , form, full); };

                auto same = [](str s1, str s2) {
                    if (s1.contains("<")) return false;
                    if (s1.contains(">")) return false;
                    return s1.upto(2) == s2.upto(2) or
                        false; };

                if (f == ""  ) { r += "0"; } else
                if (f == "#" ) { r += "0 #";  f = "("+es()      +"; "+ing()+"; "+ed()+"; "+pp()+")"; } else
//                if (f == "es") { r += "0 es"; f = "("+es(t+"es")+"; "+ing()+"; "+ed()+"; "+pp()+")"; } else
                {
                    args args ("verb|"+f);

                    if (args.complexity == 1)
                    {
                        r += "1";
                        if (not same(args[0], t)) r += " quest";
                        else f = "(" + 
                        es (args[0]+"es" ) + "; " + 
                        ing(args[0]+"ing") + "; " + 
                        ed (args[0]+"ed" ) + "; " + 
                        pp (args[0]+"ed" ) + ")";
                    }
                    else
                    if (args.complexity == 3)
                    {
                        r += "3";
                        if (not same(args[0], t)
                        or  not same(args[1], t)
                        or  not same(args[2], t)) r += " quest";
                        else f = "(" + 
                        es (args[0], true) + "; " + 
                        ing(args[1], true) + "; " + 
                        ed (args[2], true) + "; " + 
                        pp (args[2], true) + ")";
                    }
                    else
                    if (args.complexity == 4)
                    {
                        r += "4";
                        if (not same(args[0], t)
                        or  not same(args[1], t)
                        or  not same(args[2], t)
                        or  not same(args[3], t)) r += " quest";
                        else f = "(" + 
                        es (args[0], true) + "; " + 
                        ing(args[1], true) + "; " + 
                        ed (args[2], true) + "; " + 
                        pp (args[3], true) + ")";
                    }
                    else report = "quest";
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
