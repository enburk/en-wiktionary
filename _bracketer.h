struct bracketer
{
    str output;
    std::stack <std::pair<str,str>,
    std::vector<std::pair<str,str>>> stack;
    array<str> report;

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

            proceed_nowiki (paragraph.from(p).upto(b.offset));

            payload () += paragraph.from(b.offset).upto(e.offset + e.length);

            p = e.offset + e.length;
        }

        proceed_nowiki (paragraph.from(p));

        if (stack.size() != 1) dump();
        output += payload();
        payload() = "";
    }

    void proceed_nowiki (str paragraph)
    {
        int p = 0; while (p < paragraph.size())
        {
            auto b = paragraph.find("<nowiki>", str::start_from(p)); if (!b) break;
            auto e = paragraph.find("</nowiki>", str::start_from(b.offset+b.length)); if (!e) break;

            proceed_sequence (paragraph.from(p).upto(b.offset));

            payload () += paragraph.from(b.offset).upto(e.offset + e.length);

            p = e.offset + e.length;
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
            case '[': if (closing != "") proceed_closing(c); if (payload() == "") opening() += c; else stack.emplace(c, ""); break;
            case '{': if (closing != "") proceed_closing(c); if (payload() == "") opening() += c; else stack.emplace(c, ""); break;
            default : if (closing != "") proceed_closing(c);     payload() += c; break;
            }
        }
        proceed_closing(' ');
    }

    void proceed_closing (char next)
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

            if (payload().ends_with("'") and next == '\'')
                payload() += zws; // zero-width space

            closing.upto(op.size()).erase();

            opening().truncate (opening().size() - op.size());

            if (opening() == "" && stack.size() > 1)
            {
                str s = std::move(payload());
                stack.pop();

                if (payload().ends_with("'") and s.starts_with("'"))
                    payload() += zws; // zero-width space

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
