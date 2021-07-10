#pragma once
#include "4.h"
namespace pass4
{
    str templates_zzz_(str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; auto & a = args;
        args.languaged();
        args.dotcapped();

        name.replace_all(":", "..");
        name.replace_all("/", "~");
        name.replace_all("·", "middot");

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        args.kind  = "{{" + name + "}}";

        if (name == "audio-IPA"            ) { a.ignore_all(); output = a[1]; } else
        if (name == "only used in"         ) { a.ignore_all(); output = "''Only used in'' '''" + a[0] + "'''"; } else
        if (name == "acronym"              ) output = a.capitalized("Acronym of ") + a.link(); else
        if (name == "City nickname"        ) output = "''A nickname for " + a[0] + "''"; else
        if (name == "masculine of"         ) output = "''masculine of'' '''" + a[0] + "'''"; else
        if (name == "indtr")
        {
            str intr = a.acquire("intr");
            output = "(''";
            while (a.unnamed.back().starts_with(".")) a.unnamed.truncate();
            output += intr == "" ? "transitive with" : "intransitive, or transitive with";
            output += "'' '''";
            output += str::list(a.unnamed, "''', '''", "''' ''or'' '''");
            output += "''')";
        }
        else
        if (name == "Latn-def")
        {
            if (a[0] == "letter" or a[0] == "ordinal")
            {
                int n = std::stoi(a[1]);
                if (n < 0 or n >26) { n = 0; a.kind += " quest n"; }
                std::vector list = {"zeroth", "first", "second", "third", "fourth", "fifth",
                    "sixth", "seventh", "eighth", "ninth", "tenth", "eleventh", "twelfth",
                    "thirteenth", "fourteenth", "fifteenth", "sixteenth", "seventeenth", "eighteenth", "nineteenth",
                    "twentieth", "twenty-first", "twenty-second", "twenty-third", "twenty-fourth", "twenty-fifth", "twenty-sixth"};
                str nth = list[n];
                str s = a[2]; if (a.size() > 3) s += " ''or'' " + a[3];
                output = a[0] == "letter" ?
                    "''The "+nth+" letter of the English alphabet, called'' "+s+" ''and written in the Latin script.''":
                    "''The ordinal number '''"+nth+"''', derived from this letter of the English alphabet, "
                    "called "+s+" and written in the Latin script.''";
            }
            else if (a[0] == "name") output = "''The name of the Latin-script letter'' '''"+a[1]+"'''.";
            else a.kind += " quest";
        }
        else
        if (name == "alt")
        {
            while (a.size() >= 2) {
                if (a[1] == "") break;
                a[0] += ", " + a[1];
                a.unnamed.erase(1);
            }
            output = a[0]; if (a.size() >= 3)
            output += "(''"+a[2]+"'')";
        }
        else
        if (name == "alternative plural of")
        {
            if (a.complexity == 2)
            output = "''plural of " + a[0]+
            " (alternative form of ("+a[1]+")''"; else
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
        if (name == "col"  or name == "col-u"  or
            name == "col1" or name == "col1-u" or
            name == "col2" or name == "col2-u" or
            name == "col3" or name == "col3-u" or
            name == "col4" or name == "col4-u" or
            name == "col5" or name == "col5-u")
        {
            output = "";
            a.ignore("collapse");
            if (name == "col") a.unnamed.erase(0);
            str q = a.acquire("title"); if (q != "") output += "''("+q+"):''\n";
            for (str s : a.unnamed)     if (s != "") output += "* " + s + "\n";
            a.kind = "{{col}}";
        }
        else
        if (name == "&lit")
        {
            a.altqual();
            str qual  = a.acquire("qualifier");
            output = qual == "often" ? 
                "Often used other than figuratively or idiomatically: ":
                "Used other than figuratively or idiomatically: ";
            if (a.size() > 1) output += "see ";
            output += str::list(a.unnamed, ", ", " and ");
            output = "''" + output + "''";
            if (a.nodot != "") output += a.dot == "" ? "." : a.dot;
        }
        else
        if (name == "u"  or
            name == "ux" or // usex, eg, example
            name == "uxi")
        {
            str q;
            a.ignore("tr");
            a.ignore("ref");
            a.ignore("inline");
            q = a.acquire("q");           if (q != "") a[0] += " (''" + q + "'')";
            q = a.acquire("q1");          if (q != "") a[0] += " (''" + q + "'')";
            q = a.acquire("t");           if (q != "") a[0] += " (''" + q + "'')";
            q = a.acquire("footer");      if (q != "") a[0] += " (''" + q + "'')";
            q = a.acquire("translation"); if (q != "") a[0] += " (''" + q + "'')";
            if (a.complexity == 2) { output = "''"+a[0]+"'' ― ''"+a[1]+"''"; a.kind += " 2"; } else
            if (a.complexity == 1) { output = "''"+a[0]+"''"; } else
            { output = ""; a.kind += " skip"; }
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
        if (name == "IPA" or
            name == "synonyms" or
            name == "antonyms" or
            name == "holonyms" or
            name == "hyponyms" or
            name == "hypernyms" or
            name == "meronyms"  or
            name == "troponyms" or
            name == "homophone" or
            name == "homophones" or
            name == "coordinate terms" or
            false)
        {
            a.altqual();
            a.ignore_all("t");
            a.ignore_all("n");
            a.ignore_all("id");
            a.ignore_all("pos");
            a.ignore("nocount");
            if (a.opt.size() > 0) a.kind += " quest";

            output = str(a.unnamed, ", ");
            if (name == "synonyms"  or
                name == "antonyms"  or
                name == "holonyms"  or
                name == "hyponyms"  or
                name == "hypernyms" or
                name == "meronyms"  or
                name == "troponyms" or
                name == "homophone" or
                name == "homophones")
            {
                str s = name;
                if (a.size() < 2) s.truncate();
                output = s + ": " + output;
            }
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

    Pass <entry, entry> templates_zzz = [](auto & input, auto & output)
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
                    b.proceed_template = [&](str s){ return templates_zzz_(t, h, s, result); };
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
