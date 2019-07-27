const std::map<str, std::unordered_set<str>> accepted_templates
{
    { "l", { "l" } },
    { "r", { "surname" } },
};

str proceed_template (str title, str callstack, str body, Result<pass3::entry> & result)
{
    body.strip();
    if (body == "") return "";
    array<str> args = body.split_by("|");
    for (auto & arg : args) arg.strip(" \t\n");
    str name = args [0]; args.erase(args.begin());
    str kind = "other";

    if (name == "PAGENAME") return title;

    for (const auto & [group, set] : accepted_templates)
        if (set.find (name) != set.end()) 
            { kind = "known " + group; break; }

    std::map<str,str> params;
    str last_parameter; // switch default
    for (str param : args) { str key, value;
        if (param.split_by ("=", key, value)) {
            params [key] = value;
            params ["{{{"+std::to_string(params.size()    )+"}}}"] = last_parameter = value; } else
            params ["{{{"+std::to_string(params.size() + 1)+"}}}"] = last_parameter = param;
    }

    if (name.starts_with("#"))
    {
        str op, arg; name.split_by(":", op, arg); op.strip(" \t\n"); arg.strip(" \t\n");

        if (op == "#if") return arg != "" ? params["{{{1}}}"] : params["{{{2}}}"];

        if (op == "#ifeq") return arg == params["{{{1}}}"] ? params["{{{2}}}"] : params["{{{3}}}"];

        if (op == "#switch")
        {
            auto i = params.find (arg);        if (i != params.end()) return i->second;
            auto j = params.find ("#default"); if (j != params.end()) return j->second;
            return last_parameter;
        }
    }

    str report;
    if (kind == "other") report += "=================\n"; report += "{{" + body + "}}";
    if (kind == "other") for (auto [key, value] : params) report += "\n" + key + " = " + value;
    if (kind == "other") report += "\n" "{{PAGENAME}} = " + title;
    if (kind == "other") report += "\n" "call stack: " + callstack;
    if (kind == "other") report += "\n";

    if (auto   it =  redirects_templates.find(name);
               it != redirects_templates.end ()) {
        name = it -> second; if (kind == "other") report +=  "==> " + name + "\n";
    }
                
    if (auto   it =  templates.find(name);
               it != templates.end ())
        body = it -> second; else
    {
        body = "{{" + body + "}}";
        result.report(body + " ==> " + name, "404 Not found");
        return body;
    }

    if (kind == "other") report += " => " + body + "\n";

    bracketer b;
    b.proceed_template  = [&](str s) { return proceed_template (title, callstack+"/"+name, s, result); };
    b.proceed_parameter = [&](str s) { str key, def; s.split_by("|", key, def); auto i = params.find(key); return i == params.end() ? def : i->second; };
    b.proceed(body);

    body = std::move(b.output);

    report += " => " + body;
    result.report(report, kind);
    return body;
}

Pass <entry, entry> templating = [](auto & input, auto & output)
{
    Result result {__FILE__, output, true};

    for (auto && [title, topic] : input)
    {
        static int64_t nn = 0; if (++nn % 100'000 == 0) print("templates ", nn, " entries ", input.cargo, " cargo ");
                                                                               
        if (nn == 1)
        {
            // for (auto & [name, body] : templates)
            // {
            //     str report = body;
            // 
            //     bracketer b;
            //     b.proceed_sbrakets  = B.proceed_sbrakets ;
            //     b.proceed_qbrakets  = B.proceed_qbrakets ;
            //     b.proceed_link      = B.proceed_link     ;
            //     b.proceed_template  = B.proceed_template ;
            //     b.proceed_parameter = B.proceed_parameter;
            //     b.proceed(body);
            // 
            // 
            //     body = std::move(b.output);
            // 
            //     if (report != body) {
            //         report += "\n====================================\n";
            //         report += body;
            //         result.report (entry {name, std::move(report)}, "templates");
            //     }
            // 
            //     if (b.report.size() > 0) result.report(entry {name, str(std::move(b.report))}, "broken brackets (templates)");
            // }
            // 
            // print ("templates preprocessed");
        }

        bracketer b;
        b.proceed_template = [&](str s) { return proceed_template (title, "/", s, result); };
        b.proceed(topic);

        topic = std::move(b.output);

        result.accept (entry {std::move(title), std::move(topic)});
    }
};
