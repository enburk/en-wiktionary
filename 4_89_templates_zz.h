#pragma once
#include "4.h"
namespace pass4
{
    str templates_zz_(str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; auto & a = args;

        name.replace_all(":", "..");
        name.replace_all("/", "~");
        name.replace_all("·", "middot");

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        args.kind  = "{{" + name + "}}";

        if (name == "B.C.E." or
            name == "C.E."   or
            false) a.ignore_all();

        if (name == "middot"        ) output = ","; else
        if (name == "nbsp"          ) output = u8"\u00A0"; else
        if (name == "mdash"         ) output = "—"; else
        if (name == "LR"            ) output = "(ʾ)"; else
        if (name == "keyword"       ) output = a[0]; else
        if (name == "IPAfont"       ) output = a[0]; else
        if (name == "IPAlink"       ) output = a[0]; else
        if (name == "math"          ) { if (a.size() > 0) output = a[0]; } else
        if (name == "B.C.E."        ) output = "<small>B.C.E.</small>"; else
        if (name == "C.E."          ) output = "<small>C.E.</small>"; else
        if (name == "C."            ) output = a[0] + "th"; else
        if (name == "morse code for") output = "''Visual rendering of Morse code for'' " + a.link(); else
        if (name == "nb..."         ) output = a.acquire("otherforms") == "" ? " […]" : " [and other forms]"; else
        if (name == "..."           ) output = a.acquire("otherforms") == "" ? " […]" : " [and other forms]"; else
        if (name == "circa"         ) output = a.acquire("short") == "" ? "circa " + a[0] : "c. " + a[0]; else
        if (name == "circa2"        ) output = a.acquire("short") == "" ? "circa " + a[0] : "c. " + a[0]; else
        if (name == "smallcaps"     ) output = "<span style=\"font-variant:small-caps;\">"+a[0]+"</span>"; else
        if (name == "monospace"     ) output = "<span style=\"font-family:monospace;\">"+a[0]+"</span>"; else
        if (name == "upright"       ) output = "<span style=\"font-style:normal;\">"+a[0]+"</span>"; else
        if (name == "sub"           ) output = "<sub>"+a[0]+"</sub>"; else
        if (name == "sup"           ) output = "<sup>"+a[0]+"</sup>"; else
        if (name == "italic"        ) output = "<i>"+a[0]+"</i>"; else
        if (name == "small"         ) output = "<small>"+a[0]+"</small>"; else
        if (name == "defdate"       ) output = "<small>" + a.link("[", "]") + "</small>"; else
        if (name == "defdt"         ) output = "<small>" + a.link("[", "]") + "</small>"; else
        if (name == "native or resident of") output = "A native or resident of " + a[0]; else
        if (name == "used in phrasal verbs") output = a.capitalized("''Used in a phrasal verb'' ") + a.link(); else
        if (name == "ws"            ) output = "* " + a[0]; else
        if (name == "g"             ) output = "''" + a[0] + "''"; else
        if (name == "mention-gloss" ) output = "“" + a[0] + "”"; else
        if (name == "nuclide"       )
        {
            output = "<sup>"+a[0]+"</sup>"
                "<sub style=\"margin-left:" +
               (a[1].size() == 1 ? "-1.0ex" :
                a[1].size() == 2 ? "-2.3ex" : 
                a[1].size() == 3 ? "-3.5ex" : "0ex") +
                ";\">"+a[1]+"</sub>"+a[2];
        }
        else
        if (name == "w")
        {
            a.unnamed.erase_all("");
            a.unnamed.erase_all("en");
            if (a.size() > 0) output = a[0];
        }
        else
        if (name == "century")
        {
            if (a.complexity == 1) output = "<small>[from "+a[0]+"th c.]</small>"; else
            if (a.complexity == 2) output = "<small>["+a[0]+"th–"+a[1]+"th c.]</small>"; else
            a.kind += " quest";
        }
        else
        if (name == "hyphenation")
        {
            str text   = a.acquire("caption");
            str notext = a.acquire("nocaption");
            output = notext == "" ? text == "" ? "hyphenation: " : text+":" : "";
            output += str(a.unnamed, "‧");
        }
        else
        if (name == "unknown")
        {
            str nocap = a.acquire("nocap");
            str title = a.acquire("title");
            output = title == "" ? nocap == "" ? "Unknown" : "unknown" : title;
        }
        else
        if (name == "IPA letters")
        {
            output = "/";
            for (auto s : a.unnamed)
            {
                if (s.starts_with("'")) { output += "'"; s.erase(0); }
                output += 
                s=="A" ? "eɪ"    : s=="B" ? "biː"  : s=="C" ? "siː"    : s=="D" ? "diː"  :
                s=="E" ? "iː"    : s=="F" ? "ɛf"   : s=="G" ? "dʒiː"   : s=="H" ? "eɪtʃ" :
                s=="I" ? "aɪ"    : s=="J" ? "dʒeɪ" : s=="K" ? "keɪ"    : s=="L" ? "ɛl"   :
                s=="M" ? "ɛm"    : s=="N" ? "ɛn"   : s=="O" ? "əʊ"     : s=="P" ? "piː"  : s=="Q" ? "kjuː" :
                s=="R" ? "ɑː(ɹ)" : s=="S" ? "ɛs"   : s=="T" ? "tiː"    : s=="U" ? "juː"  : s=="V" ? "viː"  : s=="W" ? "dʌb.əl.juː" :
                s=="X" ? "ɛks"   : s=="Y" ? "waɪ"  : s=="Z" ? "zɛd"    :
                s=="0" ? "əʊ"    : s=="1" ? "wʌn"  : s=="2" ? "tuː"    : s=="3" ? "θɹiː" : s=="4" ? "fɔː(ɹ)" :
                s=="5" ? "faɪv"  : s=="6" ? "sɪks" : s=="7" ? "sɛv.ən" : s=="8" ? "eɪt"  : s=="9" ? "naɪn"   : "?????";
            }
            output += "/";
        }
        else
        {
            a.kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }
        if (a.kind != "{{}}" and not
            templates_usage[__FILE__].contains(a.name)) {
            templates_usage[__FILE__].insert  (a.name);
            result.report(esc + "\n" + Templates[name]
                 + "\n" + esc + "\n", "{{"+name+"}}"); }

        if (a.kind != "{{}}") 
        if (not a.opt.empty()) a.kind += " opt";

        if (a.kind.contains(" quest")) a.kind += " !!!!!";
        if (output.contains("\n")) a.kind +=  " #br#";
        if (output.contains("\n"))
            report = esc+" "+title+"\n"+report+"\n====\n"+output; else
            report = report + " => " + output + " == " + title;
        if (a.kind != "{{}}") result.report (report, a.kind);
        return output;
    }

    Pass <entry, entry> templates_zz = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
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
