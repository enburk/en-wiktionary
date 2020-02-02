
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





    if (o == "[[")
    {
        if (body.contains("|")) {
            auto ss = body.split_by("|");
            if (ss.size() >= 3) kind = "[[ pipe pipe ]]";
            if (ss.size() == 2 &&
                ss[0].find_first_not_of(alnum + " .,;!?'-") == str::npos &&
                ss[1].find_first_not_of(alnum + " .,;!?'-") == str::npos ){
                body = output = "[[" + ss[1] + "]]";
                kind = "[[ pipe ok ]]"; } else
                kind = "[[ pipe no ]]";
        }





// str ending (str s)
// {
//     if (s.ends_with("y")) { s.truncate();
//     if (s.ends_with("e"))   s.truncate(); s += "i"; };
//     "ie"
//     "g", "p", "d", "t", "m"
//         "l"
//     return s;
// }

// toey ==== adjective+ {{en-adj|toeier|more|toeyer}}
// verye ==== adjective {{en-adj|verier}}
// sare ==== adjective {{en-adj|sarer|sup=sarest}}


str comparative (str s) { if (s.ends_with("y")) { s.truncate(); if (s.ends_with("e")) s.truncate(); return s + "ier";  }; return s + "er";  }
str superlative (str s) { if (s.ends_with("y")) { s.truncate(); if (s.ends_with("e")) s.truncate(); return s + "iest"; }; return s + "est"; }

str proceed_forms (str title, str & header, str body, Result<entry> & result)
{
    str name; if (auto r = body.find('|'); r) name = body.upto(r.offset); else name = body; name.strip(" \t\n");

    auto it = list_of_forms.find(name);
    if (it == list_of_forms.end()) return "{{" + body + "}}";
    auto [templ, item] = *it;

    array<str> args = body.split_by("|");
    for (auto & arg : args) arg.strip(" \t\n");
    args.erase(args.begin());
    array<str> unnamed;
    std::map<str,str> opt;
    for (str arg : args) { str key, value;
        if (arg.split_by ("=", key, value)) {
            if (key == "head") continue;
            else opt [key] = value; }
        else unnamed += arg;
    }
    args = unnamed;
    str report = item;

    if (item != header) report += " wrong"; else
    if (item == "adjective" ||
        item == "adverb")
    {
        bool er = false, more = false, nope = false, quest = false; array<str> raw;
        for (auto arg : args)
            if (arg == "more") more = true; else
            if (arg == "er") er = true; else
            if (arg == "-") nope = true; else
            if (arg == "?") quest = true; else
                raw += arg;
        bool complex = raw.size() > 0 || opt.size() > 0;
        if (!complex && !er && !more && !nope &&  quest) { header += " # ?";   report += " q";   } else
        if (!complex && !er && !more &&  nope && !quest) { header += " # -";   report += " -";   } else
        if (!complex && !er && !more && !nope && !quest) { header += " # +";   report += " +";   } else
        if (!complex && !er &&  more && !nope && !quest) { header += " # +";   report += " +";   } else
        if (!complex && !er &&  more &&  nope && !quest) { header += " # +-";  report += " +-";  } else // (not generally comparable, comparative more godforsaken, superlative most godforsaken)
        if (!complex &&  er && !more && !nope && !quest) { header += " # er";  report += " er";  } else
        if (!complex &&  er &&  more && !nope && !quest) { header += " # er+"; report += " er+"; } else
        if (!complex &&  er && !more &&  nope && !quest) { header += " # er-"; report += " er-"; } else // (not generally comparable, comparative doozier, superlative dooziest)
        if (!complex) report += " simple"; else
        {
            report += " complex";
            str sup1, sup2, sup3; for (auto [key, value] : opt) {
                if (key == "sup" ) sup1 = value; else
                if (key == "sup2") sup2 = value; else sup3 += value;
            }
            if (sup2 == "" && sup3 == "")
            {
                str cmp = raw.size() >= 1 ? raw[0] : "";
                str sup = raw.size() >= 2 ? raw[1] : sup;

                if (cmp == comparative(title))
                if (sup == superlative(title) || sup == "")
                     { header += " # er"; report += " er="; }
                else { header += " # " + cmp + "|" + sup; report += " raw"; }

                if (more) header += "+";
                if (nope) header += "-";
            }
            else report += " quest";
        }
    }
    else
    if (item == "noun")
    {
        if (body == "en-noun") { header += " # +";   report += " +";   } else
        {
        }
    }

    result.report (title + " ==== " + header + " {{" + body + "}}", report);
    return "";
}
