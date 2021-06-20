#pragma once
#include "4.h"
namespace pass4
{
    str templates_zz_(str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; auto & a = args;
        args.languaged();

        name.replace_all("/", "~");

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        args.kind  = "{{" + name + "}}";

        if (name == "alternative plural of")
        {
            if (a.complexity == 2)
            output = "''plural of " + a[0]+
            " (alternative form of ("+a[1]+")''"; else
            a.kind += " quest";
        }
        else
        if (name == "alter")
        {
            output = "";
            for (str s : a.unnamed) {
                if (s == "" and a.kind.ends_with("()")) continue;
                if (s == "") { output += " ("; a.kind += "()"; } else
                if (output != "" and not output.ends_with("("))
                output += ", ";
                output += s;
            }
            if (a.kind.ends_with("()")) {
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
            for (str s : a.unnamed)     if (s != "") output += "* " + s + "\n";
        }
        else
        if (name == "&lit")
        {
            a.altqual();
            str dot   = a.acquire("dot");
            str nodot = a.acquire("nodot");
            str qual  = a.acquire("qualifier");
            output = qual == "often" ? 
                "Often used other than figuratively or idiomatically: ":
                "Used other than figuratively or idiomatically: ";
            if (a.unnamed.size() > 1) output += "see ";
            output += str::list(a.unnamed, ", ", " and ");
            output = "''" + output + "''";
            if (nodot != "") output += dot == "" ? "." : dot;
        }
        else
        if (name == "hyphenation")
        {
            output = "hyphenation: " + str(a.unnamed, "‧");
        }
        else
        if (name == "ux" or // usex, eg, example
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
            if (a.complexity == 1) { output = "''"+a[0]+"''"; a.kind += " 1"; } else
            { output = ""; a.kind += " skip"; }
        }
        else
        if (name == "IPA" or
            name == "synonyms" or
            name == "antonyms" or
            name == "hyponyms" or
            name == "hypernyms" or
            name == "homophone" or
            name == "homophones" or
            name == "coordinate terms" or
            false)
        {
            a.altqual();
            a.ignore_all("n");
            a.ignore_all("id");
            a.ignore("nocount");
            if (a.opt.size() > 0) a.kind += " quest";

            output = str(a.unnamed, ", ");
            if (name == "synonyms"  or
                name == "antonyms"  or
                name == "hyponyms"  or
                name == "hypernyms" or
                name == "homophone" or
                name == "homophones")
            {
                str s = name;
                if (a.unnamed.size() < 2) s.truncate();
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
