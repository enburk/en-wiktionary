struct bracketer
{
    str output;
    array<str> report;
    std::stack <std::pair<str,str>,
    std::vector<std::pair<str,str>>> stack;

    str   closing; 
    str & opening () { return stack.top().first; }
    str & payload () { return stack.top().second; }

    void dump ()
    {
        str repair = closing;
        report += "CLOSING: " + closing;
        while (stack.size() > 0) { auto [oo, xx] = stack.top(); stack.pop(); repair = oo + xx + repair;
        report += "STACK: " + oo + xx; }
        stack.emplace ("", repair);
        closing = "";
    }

    void proceed (str paragraph)
    {
        output.clear();
        report.clear();
        while (stack.size() > 0) stack.pop();
        closing.clear();

        stack.emplace("",""); 

        int p = 0; while (p < paragraph.size())
        {
            auto b = paragraph.find("<math>", str::start_from(p)); if (!b) break;
            auto e = paragraph.find("</math>", str::start_from(b.offset+b.length)); if (!e) break;

            proceed_sequence (paragraph.from(p).upto(b.offset));

            payload () += paragraph.from(b.offset).upto(e.offset + e.length);

            p = e.offset + e.length;
        }

        proceed_sequence (paragraph.from(p));

        if (stack.size() != 1) dump();
        output += payload();
        payload() = "";
    }

    void proceed_sequence (str input)
    {
        input.replace_all("{{unsupported|{}}","&lbrace;");
        input.replace_all("{{unsupported|}}}","&rbrace;");
        input.replace_all("{{unsupported|[}}","&lbrack;");
        input.replace_all("{{unsupported|]}}","&rbrack;");

        for (char c : input)
        {
            switch (c) {
            case ']':     closing += c; break;
            case '}':     closing += c; break;
            case '[': if (closing != "") proceed_closing (); if (payload() == "") opening() += c; else stack.emplace(c, ""); break;
            case '{': if (closing != "") proceed_closing (); if (payload() == "") opening() += c; else stack.emplace(c, ""); break;
            default : if (closing != "") proceed_closing ();     payload() += c; break;
            }
        }
        proceed_closing ();
    }

    void proceed_closing ()
    {
        while (closing != "")
        {
            str & oo = opening(); auto oooo = [oo](str s) { return oo.ends_with(s); };
            str & cc = closing;   auto cccc = [cc](str s) { return cc.starts_with(s); };

            str op;

            if (cc == "]"      && oooo ("["  )) op = "["  ; else
            if (cc == "}"      && oooo ("{"  )) op = "{"  ; else
            if (cc == "]]"     && oooo ("[[" )) op = "[[" ; else
            if (cc == "}}"     && oooo ("{{" )) op = "{{" ; else
            if (cc == "}}}"    && oooo ("{{{")) op = "{{{"; else
                               
            if (cccc ("]}"  )  && oo == "["   ) op = "["  ; else
            if (cccc ("}]"  )  && oo == "{"   ) op = "{"  ; else
            if (cccc ("]}"  )  && oooo ("{[" )) op = "["  ; else
            if (cccc ("}]"  )  && oooo ("[{" )) op = "{"  ; else
                               
            if (cccc ("]]]" )  && oo == "["   ) op = "["  ; else
            if (cccc ("]]]" )  && oooo ("[[" )) op = "[[" ; else
            if (cccc ("]]}" )  && oooo ("[[" )) op = "[[" ; else
            if (cccc ("}}]" )  && oooo ("{{" )) op = "{{" ; else
                               
            if (cccc ("]]]]")  && oooo ("[[" )) op = "[[" ; else
            if (cccc ("}}}]")  && oooo ("{{{")) op = "{{{"; else
                               
            if (cccc ("}}}}")  && oo == "{{"  ) op = "{{" ; else
            if (cccc ("}}}}")  && oo == "{{{" ) op = "{{{"; else
                               
            if (cccc ("}}}}}") && oooo ("{{{")) op = "{{{"; else

            { dump(); return; }

            if (op == "["  ) payload() = proceed_sbrakets  (payload()); else
            if (op == "{"  ) payload() = proceed_qbrakets  (payload()); else
            if (op == "[[" ) payload() = proceed_link      (payload()); else
            if (op == "{{" ) payload() = proceed_template  (payload()); else
            if (op == "{{{") payload() = proceed_parameter (payload());

            closing.upto(op.size()).erase();
            opening().truncate (opening().size() - op.size());

            if (opening() == "" && stack.size() > 1)
            {
                str s = std::move(payload());
                stack.pop();
                payload() += std::move(s);
            }
        }
    }

    std::function<str(str)> proceed_sbrakets  = [] (str s) { s =   "[" + s + "]";   return s; };
    std::function<str(str)> proceed_qbrakets  = [] (str s) { s =   "{" + s + "}";   return s; };
    std::function<str(str)> proceed_link      = [] (str s) { s =  "[[" + s + "]]";  return s; };
    std::function<str(str)> proceed_template  = [] (str s) { s =  "{{" + s + "}}";  return s; };
    std::function<str(str)> proceed_parameter = [] (str s) { s = "{{{" + s + "}}}"; return s; };
};

struct args
{
    str name, body, lang;
    array<str> unnamed;
    std::map<str,str> opt;
    int complexity = 0;

    str & operator [] (int i) { return unnamed[i]; }

    void ignore (str option) { if (auto it = opt.find(option); it != opt.end()) { opt.erase(it); complexity -= 10; }; }
    str acquire (str option) { if (auto it = opt.find(option); it != opt.end())
        { str s = it->second; opt.erase(it); complexity -= 10; return s; };
        return "";
    }

    args (str s)
    {
        if (auto r = s.find('|'); r)
            name = s.upto(r.offset); else
            name = s; name.strip(" \t\n");

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
        b.proceed_sbrakets  = [&piped, proceed] (str s) { return proceed(  "[" + s + "]"  ); };
        b.proceed_qbrakets  = [&piped, proceed] (str s) { return proceed(  "{" + s + "}"  ); };
        b.proceed_link      = [&piped, proceed] (str s) { return proceed( "[[" + s + "]]" ); };
        b.proceed_template  = [&piped, proceed] (str s) { return proceed( "{{" + s + "}}" ); };
        b.proceed_parameter = [&piped, proceed] (str s) { return proceed("{{{" + s + "}}}"); };
        b.proceed(s);
        s = b.output;

        array<str> args = s.split_by("|");
        for (str & arg : args) arg.strip(" \t\n"); args.erase(args.begin());
        for (str & arg : args)
        {
            str key, value;
            if (arg.split_by ("=", key, value))
            {
                if (key == "head" ) continue;
                if (key == "lang" ) continue;
                if (key == "sort" ) continue;
                if (key == "nocat") continue;

                for (auto & p : piped) key.replace_all(p.first, p.second);
                for (auto & p : piped) value.replace_all(p.first, p.second);
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
                    opt [key.ascii_lowercased()] = value;
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