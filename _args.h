struct args
{
    str name, body, lang;
    array<str> unnamed;
    std::map<str,str> opt;
    int complexity = 0;

    str & operator [] (int i) { return unnamed[i]; }

    void ignore (str option) { if (auto it = opt.find(option); it != opt.end())
        { opt.erase(it); complexity -= 10; }
    }
    str acquire (str option) { if (auto it = opt.find(option); it != opt.end())
        { str s = it->second; opt.erase(it); complexity -= 10; return s; };
        return "";
    }

    args (str s)
    {
        if (auto r = s.find('|'); r)
            name = s.upto(r.offset); else
            name = s;
        
        name.strip(" \t\n");

        if (auto   it =  redirect_templates.find(name);
                   it != redirect_templates.end ()) {
            name = it -> second;
        }

        std::map<str,str> piped;

        auto proceed = [&piped] (str s)
        {
            if (s.contains(str::one_of("|=")))
            {
                s =   "[" + s + "]";
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

                if (key == "head" ) continue;
                if (key == "head2") continue;
                if (key == "lang" ) continue;
                if (key == "sort" ) continue;
                if (key == "nocat") continue;

                body += key + "=" + value + "|";

                if (key == "1") { unnamed.resize(max(1, unnamed.size())); unnamed[0] = value; } else
                if (key == "2") { unnamed.resize(max(2, unnamed.size())); unnamed[1] = value; } else
                if (key == "3") { unnamed.resize(max(3, unnamed.size())); unnamed[2] = value; } else
                if (key == "4") { unnamed.resize(max(4, unnamed.size())); unnamed[3] = value; } else
                if (key == "5") { unnamed.resize(max(5, unnamed.size())); unnamed[4] = value; } else
                if (key == "6") { unnamed.resize(max(6, unnamed.size())); unnamed[5] = value; } else
                if (key == "7") { unnamed.resize(max(7, unnamed.size())); unnamed[6] = value; } else
                if (key == "8") { unnamed.resize(max(8, unnamed.size())); unnamed[7] = value; } else
                if (key == "9") { unnamed.resize(max(9, unnamed.size())); unnamed[8] = value; } else
                {
                    opt [key] = value;
                }
            }
            else
            {
                for (auto & p : piped) arg.replace_all(p.first, p.second);

                body += arg + "|";
                unnamed += arg;
            }
        }
        body.truncate(); //  + "|";

        auto i = unnamed.begin(); for (auto j = unnamed.begin(); j != unnamed.end(); j++)
        {
            if ((*j == "en" || *j == "mul") && lang == "") lang = *j; else std::swap(*i++, *j);
        }
        unnamed.erase(i, unnamed.end());

        complexity = (int)(unnamed.size() + 10 * opt.size());
    }
};