struct bracketer
{
    str output;
    array<str> report;
    std::stack <std::pair<str,str>,
    std::vector<std::pair<str,str>>> stack;

    str   closing; 
    str & opening () { return stack.top().first; }
    str & payload () { return stack.top().second; }
    str paragraph; 

    void dump ()
    {
        str repair = closing;
        report += paragraph;
        report += "=================================================================";
        report += "CLOSING: " + closing;
        while (stack.size() > 0) { auto [oo, xx] = stack.top(); stack.pop(); repair = oo + xx + repair;
        report += "STACK: " + oo + xx; }
        stack.emplace ("", repair);
        closing = "";
    }

    void proceed (str topic)
    {
        output.clear();
        report.clear();
        while (stack.size() > 0) stack.pop();
        paragraph.clear();
        closing.clear();

        stack.emplace("",""); 

        int p = 0; while (p < topic.size())
        {
            int b = p+1 < topic.size() ? topic.find("==== ", str::start_from(p+1)) : str::nope;
            
            if (b == str::nope) b = topic.size();

            paragraph = topic.substr(p, b-p); proceed_paragraph ();

            p = b;

            if (stack.size() != 1) dump();
            output += payload();
            payload() = "";
        }
    }

    void proceed_paragraph ()
    {
        int p = 0; while (p < paragraph.size())
        {
            int b = paragraph.find("&lt;math&gt;", str::start_from(p)); if (b == str::nope){ break; }
            int e = paragraph.find("&lt;/math&gt;", str::start_from(b)); if (e == str::nope){ break; }

            proceed_sequence (paragraph.substr(p, b-p));

            payload () += paragraph.substr (b, e-b+13);

            p = e+13;
        }

        proceed_sequence (paragraph.from(p));
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

            closing.erase (0, op.size());
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
