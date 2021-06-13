#pragma once
#include "4.h"
namespace pass4
{
    str templates_zz_(str title, str header, str body, Result<entry> & result)
    {
        args args (body, false); str name = args.name; str arg = args.body; auto & a = args;

        name.replace_all("/", "~");

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        if (a.unnamed.size() > 0) { if (not Languages.contains(a[0])) kind += " quest lang";
            a.unnamed.erase(0);
            a.complexity--;
        }

        if (name == "alternative plural of")
        {
            if (a.complexity == 2) { output = "''plural of "+a[0]+" (alternative form of ("+a[1]+")''"; } else
            kind += " quest";
        }
        else
        if (name == "alter")
        {
            output = "";
            for (str s : a.unnamed) {
                if (s == "" and kind.ends_with("()")) continue;
                if (s == "") { output += " ("; kind += "()"; } else
                if (output != "" and not output.ends_with("("))
                output += ", ";
                output += s;
            }
            if (kind.ends_with("()")) {
                if (output.ends_with("(")) {
                    output.truncate();
                    output.truncate(); }
                else output += ")";
            }
        }
        else
        if (name == "col2" or
            name == "col3" or
            name == "col4")
        {
            output = "";
            str q = a.acquire("title"); if (q != "") output += "''("+q+"):''\n";
            for (str s : a.unnamed) if (s != "") output += "* " + s + "\n";
        }
        else
        if (name == "doublet"     or
            name == "clipping"    or
            name == "clipping of" or
            false)
        {
            str
            q = a.acquire("t1"); if (q != "" and a.unnamed.size() >= 1) a[0] += " (''" + q + "'')";
            q = a.acquire("t2"); if (q != "" and a.unnamed.size() >= 2) a[1] += " (''" + q + "'')";
            q = a.acquire("alt1");  if (q != "" and a.unnamed.size() >= 1) { a[0] = q; }
            q = a.acquire("alt2");  if (q != "" and a.unnamed.size() >= 2) { a[1] = q; }
            q = a.acquire("alt3");  if (q != "" and a.unnamed.size() >= 3) { a[2] = q; }
            str dot    = a.acquire("dot");
            str nodot  = a.acquire("nodot");
            str nocap  = a.acquire("nocap");
            str notext = a.acquire("notext");
            a.ignore("id1"); a.ignore("pos1");
            if (a.opt.size() > 0) kind += " quest";
            for (auto & s : a.unnamed) s = "[["+s+"]]";
            output = notext != "" ? "" : 
            name == "doublet"     ? nocap != "" ? "Doublet of " : "doublet of ":
            name == "clipping"    ? nocap != "" ? "Clipping of " : "clipping of ":
            name == "clipping of" ? nocap != "" ? "Clipping of " : "clipping of ":
            "";
            output += str::list(a.unnamed, ", ", " and ");
            output += "''" + output + "''";

            if (name == "clipping of")
            if (nodot != "") output += dot == "" ? "." : dot;
        }
        else
        if (name == "named-after")
        {
            str dot    = a.acquire("dot");
            str nodot  = a.acquire("nodot");
            str nocap  = a.acquire("nocap");
            str notext = a.acquire("notext");
            str nat    = a.acquire("nat");
            str born   = a.acquire("born");
            str died   = a.acquire("died");
            a.ignore("wplink");
            str q;
            q = a.acquire("nationality");  if (q != "") nat = q;

            array<str> occs;
            q = a.acquire("occ");  if (q != "") occs += q;
            q = a.acquire("occ1"); if (q != "") occs += q;
            q = a.acquire("occ2"); if (q != "") occs += q;
            q = a.acquire("occ3"); if (q != "") occs += q;
            q = a.acquire("occ4"); if (q != "") occs += q;
            str occ = str::list(occs, ", ", " and ");

            if (a.opt.size() > 0) kind += " quest";
            output = notext != "" ? "" : nocap != "" ? "Named after " : "named after of ";
            if (nat  != "") output += nat + " ";
            if (occ  != "") output += occ + " ";
            if (born != "") output += "(" + born + "–" + died + ")";
            output += str::list(a.unnamed, ", ", " and ");
            if (nodot != "") output += dot == "" ? "." : dot;
        }
        else
        if (name == "&lit")
        {
            str
            q = a.acquire("alt1");  if (q != "" and a.unnamed.size() >= 1) { a[0] = q; }
            q = a.acquire("alt2");  if (q != "" and a.unnamed.size() >= 2) { a[1] = q; }
            q = a.acquire("alt3");  if (q != "" and a.unnamed.size() >= 3) { a[2] = q; }
            str dot = a.acquire("dot");
            str nodot = a.acquire("nodot");
            str qual = a.acquire("qualifier");
            if (a.opt.size() > 0) kind += " quest";
            output = qual == "often" ? 
                "Often used other than figuratively or idiomatically: ":
                "Used other than figuratively or idiomatically: ";
            if (a.unnamed.size() > 1) output += "see ";
            output += str::list(a.unnamed, ", ", " and ");
            output = "''" + output + "''";
            if (nodot != "") output += dot == "" ? "." : dot;
        }
        else
        if (name == "ll")
        {
            if (a.complexity == 1) { output = a[0]; } else kind += " quest";
        }
        else
        if (name == "hyphenation")
        {
            output = "hyphenation: " + str(a.unnamed, "‧");
        }
        else
        if (name == "ux") // usex, eg, example
        {
            a.ignore("inline"); str
            q = a.acquire("q");      if (q != "") a[0] += " (''" + q + "'')";
            q = a.acquire("footer"); if (q != "") a[0] += " (''" + q + "'')";
            if (a.complexity == 1) { output = "''"+a[0]+"''"; kind += " 1"; } else
            if (a.complexity == 2) { output = "''"+a[0]+"'' ― ''"+a[1]+"''"; kind += " 2"; } else
            { output = ""; kind += " skip"; }
        }
        else
        if (name == "IPA" or
            name == "synonyms" or
            name == "antonyms" or
            name == "hyponyms" or
            name == "hypernyms" or
            name == "homophones" or
            name == "coordinate terms" or
            false)
        {
            for (str & s : a.unnamed)
                if (s.starts_with("Thesaurus:") or
                    s.starts_with("thesaurus:"))
                    s.upto(10).erase();

            a.ignore("nocount");
            a.ignore("n"); a.ignore("n1"); a.ignore("n2"); a.ignore("n3"); a.ignore("n4"); a.ignore("n5");
            a.ignore("id"); a.ignore("id1"); a.ignore("id2"); a.ignore("id3");
            a.ignore("id4"); a.ignore("id5"); a.ignore("id6");
            str
            q = a.acquire("qual" ); if (q != "" and a.unnamed.size() >= 1) a[0] = "(''" + q + "'') " + a[0];
            q = a.acquire("qual1"); if (q != "" and a.unnamed.size() >= 1) a[0] = "(''" + q + "'') " + a[0];
            q = a.acquire("qual2"); if (q != "" and a.unnamed.size() >= 2) a[1] = "(''" + q + "'') " + a[1];
            q = a.acquire("qual3"); if (q != "" and a.unnamed.size() >= 3) a[2] = "(''" + q + "'') " + a[2];
            q = a.acquire("qual4"); if (q != "" and a.unnamed.size() >= 4) a[3] = "(''" + q + "'') " + a[3];
            q = a.acquire("qual5"); if (q != "" and a.unnamed.size() >= 5) a[4] = "(''" + q + "'') " + a[4];
            q = a.acquire("qual6"); if (q != "" and a.unnamed.size() >= 6) a[5] = "(''" + q + "'') " + a[5];
            q = a.acquire("qual7"); if (q != "" and a.unnamed.size() >= 7) a[6] = "(''" + q + "'') " + a[6];
            q = a.acquire("q" );    if (q != "" and a.unnamed.size() >= 1) a[0] = "(''" + q + "'') " + a[0];
            q = a.acquire("q1");    if (q != "" and a.unnamed.size() >= 1) a[0] = "(''" + q + "'') " + a[0];
            q = a.acquire("q2");    if (q != "" and a.unnamed.size() >= 2) a[1] = "(''" + q + "'') " + a[1];
            q = a.acquire("q3");    if (q != "" and a.unnamed.size() >= 3) a[2] = "(''" + q + "'') " + a[2];
            q = a.acquire("q4");    if (q != "" and a.unnamed.size() >= 4) a[3] = "(''" + q + "'') " + a[3];
            q = a.acquire("q5");    if (q != "" and a.unnamed.size() >= 5) a[4] = "(''" + q + "'') " + a[4];
            q = a.acquire("q6");    if (q != "" and a.unnamed.size() >= 6) a[5] = "(''" + q + "'') " + a[5];
            q = a.acquire("q7");    if (q != "" and a.unnamed.size() >= 7) a[6] = "(''" + q + "'') " + a[6];
            q = a.acquire("q8");    if (q != "" and a.unnamed.size() >= 8) a[7] = "(''" + q + "'') " + a[7];
            q = a.acquire("q9");    if (q != "" and a.unnamed.size() >= 9) a[8] = "(''" + q + "'') " + a[8];
            q = a.acquire("q10");   if (q != "" and a.unnamed.size() >=10) a[9] = "(''" + q + "'') " + a[9];
            q = a.acquire("q11");   if (q != "" and a.unnamed.size() >=11) a[10]= "(''" + q + "'') " + a[10];
            q = a.acquire("q12");   if (q != "" and a.unnamed.size() >=12) a[11]= "(''" + q + "'') " + a[11];
            q = a.acquire("q13");   if (q != "" and a.unnamed.size() >=13) a[12]= "(''" + q + "'') " + a[12];
            q = a.acquire("q14");   if (q != "" and a.unnamed.size() >=14) a[13]= "(''" + q + "'') " + a[13];
            q = a.acquire("q15");   if (q != "" and a.unnamed.size() >=15) a[14]= "(''" + q + "'') " + a[14];
            q = a.acquire("alt1");  if (q != "" and a.unnamed.size() >= 1) { a[0] = q; }
            q = a.acquire("alt2");  if (q != "" and a.unnamed.size() >= 2) { a[1] = q; }
            q = a.acquire("alt3");  if (q != "" and a.unnamed.size() >= 3) { a[2] = q; }

            if (a.opt.size() > 0) kind += " quest";
            output = str(a.unnamed, ", ");
            if (name == "synonyms"  or
                name == "antonyms"  or
                name == "hyponyms"  or
                name == "hypernyms" or
                name == "homophones")
            {
                str s = name;
                if (a.unnamed.size() < 2)
                    s.truncate();
                output = s + ": " + output;
            }
        }
        else
        {
            kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }
        if (kind != "{{}}" and not
            templates_usage[__FILE__].contains(name)) {
            templates_usage[__FILE__].insert  (name);
            result.report(esc + "\n" + Templates[name]
                 + "\n" + esc + "\n", "{{"+name+"}}"); }

        if (kind.contains(" quest")) kind += " !!!!!";
        if (output.contains("\n")) kind +=  " #br#";
        if (output.contains("\n")) report = "==== " + title + " ==== " + header + " ==== " + "\n\n" + report;
        result.report (report + " => " + output + " == " + title, kind);
        return output;
    }

    Pass <entry, entry> templates_zz = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("templatesz", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                for (auto & line : content)
                {
                    bracketer b;
                    b.proceed_template = [&](str s){ return templates_zz_(t, h, s, result); };
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
