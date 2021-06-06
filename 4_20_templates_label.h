#pragma once
#include "4.h"
namespace pass4
{
    struct lbl
    {
        str display, language;
        bool omit_preComma = false;
        bool omit_postComma = false;
    };
    std::map<str, lbl> labels_labels;
    std::map<str, str> labels_aliases;

    str templates_label_ (str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; str arg = args.body;

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        if (name != "label") return output; output = "";

        str complexity = std::to_string(min(args.complexity, 9));
        if (complexity.size() == 1) complexity = "0" + complexity;
        str complexity_plus = "";

        bool OMIT_postComma = true;

        for (auto arg : args.unnamed)
        {
            str out = arg; bool alias = false;
            if (labels_aliases.contains(arg)) {
                out = labels_aliases[arg];
                alias = true;
            }

            bool omit_preComma = false;
            bool omit_postComma = false;

            auto it = labels_labels.find(out);
            if (it != labels_labels.end()) {

                if (it->second.language == "" or
                    it->second.language == "en")
                {
                    omit_preComma  = it->second.omit_preComma;
                    omit_postComma = it->second.omit_postComma;
                    out =
                        it->second.display != "" ?
                        it->second.display : out;
                }
                else if (alias) out = arg;
            }

            if (not omit_preComma and
                not OMIT_postComma)
                output += ",";

            else if (output != "") complexity_plus = "+";

            if (output != "")
                output += " ";

            output += out;

            OMIT_postComma = omit_postComma;
        }

        complexity += complexity_plus;
        result.report (report + " => " + output, complexity);
        return "(''"+output+"'')";
    }

    Pass <entry, entry> templates_label = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        bool first_time = true;

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("templates2", nn, input.cargo);

            if (first_time) {
                first_time = false;
            
                for (auto & [name, analysis] : Repo)
                {
                    if (not name.starts_with("label"))
                        continue;

                    for (auto & cluster : analysis.clusters)
                    {
                        auto ee = cluster.elements;
                        auto cc = cluster.clusters;

                        if (ee.size() == 4
                        and ee[0].token
                        and ee[0].token->text == "aliases"
                        and ee[1].elements.size() == 1
                        and ee[1].elements[0].elements.size() == 1
                        and ee[1].elements[0].elements[0].token
                        and ee[3].token) { str to =
                            ee[3].token->text; str from = 
                            ee[1].elements[0].elements[0].token->text;
                            from.upto(1).erase(); from.truncate();
                            to.upto(1).erase(); to.truncate();
                            result.report(from + " => " + to, "aliases");
                            labels_aliases[from] = to;
                        }

                        if (ee.size() == 4
                        and ee[0].token
                        and ee[0].token->text == "labels"
                        and ee[1].elements.size() == 1
                        and ee[1].elements[0].elements.size() == 1
                        and ee[1].elements[0].elements[0].token)
                        {
                            lbl l; str from = 
                            ee[1].elements[0].elements[0].token->text;
                            from.upto(1).erase(); from.truncate();

                            for (auto & pp : ee[3].elements)
                            {
                                auto & p = pp.elements;
                                if (p.size() == 3
                                and p[0].token
                                and p[2].token)
                                {
                                    str x = p[0].token->text;
                                    str y = p[2].token->text;
                                    if (y != "true") y.upto(1).erase();
                                    if (y != "true") y.truncate();

                                    if (x == "display" ) l.display  = y;
                                    if (x == "language") l.language = y;
                                    if (x == "omit_preComma" ) l.omit_preComma  = y == "true";
                                    if (x == "omit_postComma") l.omit_postComma = y == "true";
                                }
                            }

                            str report = "labels";
                            if (l.omit_preComma ) report += " omit-pre-comma";
                            if (l.omit_postComma) report += " omit-post-comma";
                            result.report(from + " => " + l.display, report);
                            labels_labels[from] = l;
                        }
                    }
                }
            }

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                for (auto & line : content)
                {
                    bracketer b;
                    b.proceed_template = [&](str s){ return templates_label_(t, h, s, result); };
                    b.proceed(line);
                    line = b.output;
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
