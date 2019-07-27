const std::map<str, std::unordered_set<str>> rejected_templates
{
    {   "mid",
    {
        "top",  "mid",  "bottom",  "der-top",  "der-mid",  "der-bottom",  "rel-top",  "rel-mid",  "rel-bottom",
        "top1", "mid1", "bottom1", "der-top1", "der-mid1", "der-bottom1", "rel-top1", "rel-mid1", "rel-bottom1",
        "top2", "mid2", "bottom2", "der-top2", "der-mid2", "der-bottom2", "rel-top2", "rel-mid2", "rel-bottom2",
        "top3", "mid3", "bottom3", "der-top3", "der-mid3", "der-bottom3", "rel-top3", "rel-mid3", "rel-bottom3", 
        "top4", "mid4", "bottom4", "der-top4", "der-mid4", "der-bottom4", "rel-top4", "rel-mid4", "rel-bottom4", 
        "top5", "mid5", "bottom5", "der-top5", "der-mid5", "der-bottom5", "rel-top5", "rel-mid5", "rel-bottom5",
    }},
    {   "wiki",
    {
        "wikipedia",

        "plural of"
    }}
};

str template_kind (str name)
{
    for (const auto & [kind, set] : rejected_templates)
        if (set.find (name) != set.end())
            return kind;
    return "";
}

str proceed_template (str title, str header, str body, Result<entry> & result)
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

        if (name == "head" && args.size() == 2)
        {
            str s = args[1]; s.replace_all("[[", ""); s.replace_all("]]", "");
            kind = "template head " + (s == title ? str("1") : str("2"));
            body = "";
        }

        if (name == "l" && args.size() == 2)
        {
            kind = "template l";
            body = "[[" + args[1] + "]]";
            braced += " ==> " + body;
        }

        if (name == "label" && args.size() >= 2)
        {
            body = "";
            args.erase(args.begin());
            for (auto arg : args) body += arg + ", ";
            body.truncate(); body.truncate();
            body = "(''" + body + "'')";
            kind = "template label " + std::to_string(std::min(5,args.size()));
            braced += " ==> " + body;
        }

        if (name == "mention" && args.size() == 2)
        {
            body = "''" + args[1] + "''";
            str what;
            if (opt["tr" ] != "") what = "''" + opt["tr"] + "''";
            if (opt["lit"] != "") what += ", literally ''" + opt["lit"] + "''";
            if (what != "") body += " (" + what + ")";
            braced += " ==> " + body;
            kind = "template m " + (opt.size() == 0 ? str("1") : str("2"));
        }

        if (name.starts_with("quote-")) { kind = "quote quote-"; body = "'''0000'''"; } else
        if (name.starts_with("cite-" )) { kind = "quote cite-";  body = "'''0000'''"; } else
        {}
    }
    
    result.report (braced, kind);

    return kind == "{{" ? braced : body;
}
