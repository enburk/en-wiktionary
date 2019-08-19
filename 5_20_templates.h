
str template_kind (str name)
{
    for (const auto & [kind, set] : rejected_templates)
        if (set.find (name) != set.end())
            return kind;
    return "";
}

str proceed_template (str title, str & header, str body, Result<entry> & result)
{
    str braced = "{{" + body + "}}";
    array<str> args = body.split_by("|");
    for (auto & arg : args) arg.strip(" \t\n");
    str name = args [0]; args.erase(args.begin());
    if (auto   it =  redirects_templates.find(name);
               it != redirects_templates.end ()) {
        name = it -> second;
    }
    str kind =  "{{" + template_kind (name);
    if (kind != "{{") body = ""; else
    {
        array<str> unnamed;
        std::map<str,str> opt;
        for (str arg : args) { str key, value;
            if (arg.split_by ("=", key, value))
                opt [key] = value; else
                unnamed += arg;
        }
        args = unnamed;

        if (name == "link")
        {
            if (args.size() != 2) kind = "template link quest"; else
            {
                kind = "template link";
                body = "[[" + args[1] + "]]";
                braced += " ==> " + body;
            }
        }
        else
        if (name == "label")
        {
            if (args.size() < 2) kind = "template label quest"; else
            {
                body = "";
                args.erase(args.begin());
                for (auto arg : args) body += arg + ", ";
                body.truncate(); body.truncate();
                body = "(''" + body + "'')";
                kind = "template label " + std::to_string(std::min(5,args.size()));
                braced += " ==> " + body;
            }
        }
        else
        if (name == "mention")
        {
            if (args.size() != 2) kind = "template m quest"; else
            {
                body = "''" + args[1] + "''";
                str what;
                if (opt["tr" ] != "") what = "''" + opt["tr"] + "''";
                if (opt["lit"] != "") what += ", literally ''" + opt["lit"] + "''";
                if (what != "") body += " (" + what + ")";
                braced += " ==> " + body;
                kind = "template m " + (opt.size() == 0 ? str("1") : str("2"));
            }
        }
        else
        if (name == "w")
        {
            if (args.size() != 1) kind = "template w quest"; else
            {
                kind = "template w";
                body = "[[" + args[0] + "]]";
                braced += " ==> " + body;
            }
        }

        else
    }
    
    result.report (braced, kind);

    return kind == "{{" ? braced : body;
}

std::unordered_set<str> related
{
    "rel1", "der1", "hyp1", "coord1", "col1", "syn1", "ant1", "derived terms",
    "rel2", "der2", "hyp2", "coord2", "col2", "syn2", "ant2",
    "rel3", "der3", "hyp3", "coord3", "col3", "syn3", "ant3", "der3-u",
    "rel4", "der4", "hyp4", "coord4", "col4", "syn4", "ant4",
    "rel5", "der5", "hyp5", "coord5", "col5", "syn5", "ant5",
};


        if (related.find(name) != related.end())
        {
            array<str> args = body.split_by("|");
            for (auto & arg : args) arg.strip(" \t\n");
            args.erase(args.begin());

            kind = "{{ related items }}";
            output = ""; for (auto arg : args) if (!arg.found("=")) output += "[[" + arg + "]], ";
            output.truncate(); output.truncate();
            report += "\n==> " + output + "\n";
        }
