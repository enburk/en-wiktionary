struct args
{
    str name, body;
    str kind, info;
    str gloss1, gloss2;
    str lang, lang1, lang2;
    std::map<str,str> opt;
    array<str> unnamed;
    int complexity = 0;

    str & operator [] (int i) { return unnamed[i]; }

    str acquire (str option)
    {
        if (auto
            it = opt.find(option);
            it != opt.end())
        {
            str s = it->second;
            opt.erase(it);
            complexity -= 10;
            return s;
        };
        return "";
    }
    void ignore (str option) { acquire (option); }
    void ignore_all () { complexity -= 10 * opt.size(); opt.clear(); }

    args (str s)
    {
        if (auto r = s.find('|'); r)
            name = s.upto(r.offset); else
            name = s;
        
        name.strip(" \t\n");
        name.canonicalize();

        if (auto   it =  redirect_templates.find(name);
                   it != redirect_templates.end ()) {
            name = it -> second;
        }

        std::map<str,str> piped;

        auto proceed = [&piped] (str s)
        {
            if (s.contains(str::one_of("|=")))
            {
                str ss = "#####"+std::to_string(piped.size());
                piped[ss] = s;
                s = ss;
            };
            return s;
        };

        bracketer b;
        b.proceed_sbrakets  = [proceed] (str s) { return proceed(  "[" + s + "]"  ); };
        b.proceed_qbrakets  = [proceed] (str s) { return proceed(  "{" + s + "}"  ); };
        b.proceed_link      = [proceed] (str s) { return proceed( "[[" + s + "]]" ); };
        b.proceed_template  = [proceed] (str s) { return proceed( "{{" + s + "}}" ); };
        b.proceed_parameter = [proceed] (str s) { return proceed("{{{" + s + "}}}"); };
        b.proceed(s);
        s = b.output;

        while (true)
        {
            auto b = s.find( "<math>"); if (!b) break;
            auto e = s.find("</math>"); if (!e) break; if (e.offset < b.offset) break;

            auto range = s.from(b.offset).upto(e.offset+7);
            str ss = "#####"+std::to_string(piped.size());
            piped[ss] = str(range);
            range.replace_by(ss);
        }

        while (true)
        {
            auto b = s.find( "<nowiki>"); if (!b) break;
            auto e = s.find("</nowiki>"); if (!e) break; if (e.offset < b.offset) break;

            auto range = s.from(b.offset).upto(e.offset+9);
            str ss = "#####"+std::to_string(piped.size());
            piped[ss] = str(range);
            range.replace_by(ss);
        }

        array<str> args = s.split_by("|");
        for (str & arg : args) arg.strip(" \t\n"); if (args.size() > 0) args.erase(args.begin());
        for (str & arg : args)
        {
            str key, value; if (arg.split_by("=", key, value))
            {
                for (auto & p : piped) key.replace_all(p.first, p.second);
                for (auto & p : piped) value.replace_all(p.first, p.second);

                key.strip(); value.strip();
                key = key.ascii_lowercased();

                if (key == "head"  ) continue;
                if (key == "head2" ) continue;
                if (key == "gloss" ) { gloss1 = value; continue; }
                if (key == "gloss1") { gloss1 = value; continue; }
                if (key == "gloss2") { gloss2 = value; continue; }
                if (key == "lang"  ) { lang1  = value; continue; }
                if (key == "lang1" ) { lang1  = value; continue; }
                if (key == "lang2" ) { lang2  = value; continue; }
                if (key == "sort"  ) continue;
                if (key == "nocat" ) continue;

                body += key + "=" + value + "|";

                if (key.digital())
                {
                    int n = std::stoi(key);
                    if (n < 0 or n > 99) kind += " quest key"; else
                    {
                        unnamed.resize(max(n+1, unnamed.size()));
                        unnamed[n] = value;
                    }
                }
                else
                {
                    opt [key] = value;
                }
            }
            else
            {
                for (auto & p : piped) arg.replace_all(p.first, p.second);

                body += arg + "|";

                if (not arg.contains("[[")
                and not arg.contains("]]"))
                {
                    if (arg.starts_with(":" )) arg.upto(1).erase();
                    if (arg.starts_with("W:")) arg.upto(2).erase();
                    if (arg.starts_with("w:")) arg.upto(2).erase();
                    if (arg.starts_with("s:")) arg.upto(2).erase();
                    if (arg.starts_with("Wikipedia:") or
                        arg.starts_with("wikipedia:"))
                        arg.upto(10).erase();
                    if (arg.starts_with("Citations:") or
                        arg.starts_with("citations:"))
                        arg.upto(10).erase();
                    if (arg.starts_with("Thesaurus:") or
                        arg.starts_with("thesaurus:"))
                        arg.upto(10).erase();
                    if (arg.starts_with("Appendix:") or
                        arg.starts_with("appendix:"))
                        arg.upto(9).erase();

                    str anchor;
                    arg.split_by("#A", arg, anchor);
                    arg.split_by("#E", arg, anchor);
                    arg.split_by("#D", arg, anchor);
                    arg.split_by("#I", arg, anchor);
                    arg.split_by("#M", arg, anchor);
                    arg.split_by("#N", arg, anchor);
                    arg.split_by("#O", arg, anchor);
                    arg.split_by("#P", arg, anchor);
                    arg.split_by("#S", arg, anchor);
                    arg.split_by("#U", arg, anchor);
                    arg.split_by("#V", arg, anchor);
                    arg.split_by("##", arg, anchor);

                    if (arg.contains("#")) info += "#";
                    if (arg.contains(":")) info += ".";
                }

                unnamed += arg;
            }
        }
        body.truncate(); //  + "|";

        complexity = (int)(unnamed.size() + 10 * opt.size());
    }

    void languaged ()
    {
        if (unnamed.empty()) kind += " quest lang";
        if (unnamed.empty()) return;
        lang = unnamed[0];
        unnamed.erase(0);
        complexity--;
        if (not Languages.contains(lang))
            kind += " quest lang";
    }

    str cap, nocap;
    str dot, nodot;
    str notext;

    void dotcapped ()
    {
        dot    = acquire("dot");
        cap    = acquire("cap");
        nodot  = acquire("nodot");
        nocap  = acquire("nocap");
        notext = acquire("notext");

        if (dot != "" and nodot != "") kind += " quest dot";
        if (cap != "" and nocap != "") kind += " quest cap";

        if (nodot == "yes" or nodot == "y" or nodot == "t") nodot = "1";
        if (nocap == "yes" or nocap == "y" or nocap == "t") nocap = "1";
        if (  cap == "yes" or   cap == "y" or   cap == "t")   cap = "1";

        if (nodot != "") nodot = "1";
        if (nocap != "") nocap = "1";

        if (nodot != "" and nodot != "1") kind += " quest dot";
        if (nocap != "" and nocap != "1") kind += " quest cap";
        if (  cap != "" and   cap != "1") kind += " quest cap";

        if (dot != ""  and
            dot != "." and
            dot != "," and
            dot != ":" and
            dot != ";") info += " dot";

        if (dot == "" and nodot != "1") dot = ".";
    }

    str capitalized (str s)
    {
        if (s      == "") return "";
        if (notext != "") return "";
        if (nocap  != "") s[0] = str::ascii_tolower(s[0]);
        if (cap   == "1") s[0] = str::ascii_toupper(s[0]);
        return s;
    }

    str link (str o = "'''", str c = "'''")
    {
        str output;

        auto & a = unnamed;

        if (a.size() > 1 and a[1] != "") { output = a[1]; kind += " 2"; } else
        if (a.size() > 0 and a[0] != "") { output = a[0]; kind += " 1"; } else
                                         { output ="(?)"; kind += " 0"; }

        str
        alt = acquire("alt" ); if (alt != "") output = alt;
        alt = acquire("alt1"); if (alt != "") output = alt;

        output = o + output + c;

        ignore("sc"); // script
        ignore("id");
        ignore("pos");

        str tr = acquire("tr"); // transcript
        str tt = acquire("t" ); // translation
        str ts = acquire("ts"); if (ts != "") tr = ts;
        if (a.size() >= 3 and a[2] != "") tt = a[2];

        if (tt == "" and gloss1 != "") tt = gloss1;
        if (tr == "-") tr = "";
        if (tt == "-") tt = "";
        if (tr != "") tr = "''"+tr+"''";
        if (tt != "") tt = "“" +tt+ "”";

        str lit = acquire("lit"); if (lit != "") tt = "literally “" +lit+ "”";

        if (tr == "" and tt != "") { output += " ("+tt+")";         kind += " tr"; } else
        if (tr != "" and tt == "") { output += " ("+tr+")";         kind += " tr"; } else
        if (tr != "" and tt != "") { output += " ("+tr+", "+tt+")"; kind += " tr"; }

        if (not opt.empty()) kind += " quest";

        return output;
    }

    void altqual ()
    {
        {
            str q = acquire("alt");
            if (q != "" and unnamed.size() >= 1)
                unnamed[0] = q;

            for (int i=1; i<20; i++) {
                str q = acquire("alt" + std::to_string(i));
                if (q != "" and unnamed.size() >= i)
                    unnamed[i-1] = q;
            }
        }
        for (str qual : {"q", "qual"})
        {
            str q = acquire(qual);
            if (q != "" and unnamed.size() >= 1)
                unnamed[0] = "(''" + q + "'') " +
                unnamed[0];

            for (int i=1; i<20; i++) {
                str q = acquire(qual + std::to_string(i));
                if (q != "" and unnamed.size() >= i)
                    unnamed[i-1] = "(''" + q + "'') " +
                    unnamed[i-1];
            }
        }
    }

    auto acquire_all (str s)
    {
        array<str> ss;
        str q = acquire(s); if (q != "") ss += q; for (int i=0; i<20; i++) { str S = s + std::to_string(i);
        str q = acquire(S); if (q != "") ss += q; }
        return ss;
    }
    void ignore_all (str s)
    {
        ignore(s); for (int i=0; i<20; i++) { str S = s + std::to_string(i);
        ignore(S); }
    }
};