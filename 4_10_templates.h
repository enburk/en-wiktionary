#pragma once
#include "4.h"
namespace pass4
{
    const std::map<str, std::unordered_set<str>> rejected_templates
    {
        {   "requests",
        {
            "rfe", // request for etymology
            "rfi", // (Image requested)
            "rfdef", // (definition required, request for definition, required definition)
            "rfquotek", "rfap", "rfex", "rfp", "rfap", "rfquote-sense", "rfv-etym",
            "rfc-sense", "rfv-sense", "rfc", "rfclarify", "rfd-sense", "rfv-pron",
            "rfref", "rfquote", "rfscript", "rfd-redundant", "rfm-sense",
            "rfd", "rfm", "rfv" //, "rfdate"
        }},
        {   "pronunciation",
        {
            "audio", "rhymes", "enPR", // non IPA pronunciation
            "root", // etymology
        }},
        {   "mid",
        {
            "top",  "mid",  "bottom",  "col-top",  "col-bottom", 
            "top1", "mid1", "bottom1", "col-top1", "col-bottom1",
            "top2", "mid2", "bottom2", "col-top2", "col-bottom2",
            "top3", "mid3", "bottom3", "col-top3", "col-bottom3", 
            "top4", "mid4", "bottom4", "col-top4", "col-bottom4", 
            "top5", "mid5", "bottom5", "col-top5", "col-bottom5",
            "der-top",  "der-mid",  "der-bottom",  "rel-top",  "rel-mid",  "rel-bottom", 
            "der-top1", "der-mid1", "der-bottom1", "rel-top1", "rel-mid1", "rel-bottom1",
            "der-top2", "der-mid2", "der-bottom2", "rel-top2", "rel-mid2", "rel-bottom2",
            "der-top3", "der-mid3", "der-bottom3", "rel-top3", "rel-mid3", "rel-bottom3", 
            "der-top4", "der-mid4", "der-bottom4", "rel-top4", "rel-mid4", "rel-bottom4", 
            "der-top5", "der-mid5", "der-bottom5", "rel-top5", "rel-mid5", "rel-bottom5",
        }},
        {   "miscellaneous",
        {
            "Letter", "multiple images", "picdic", "projectlinks",
            "examples", "examples-right", "broken ref",
            "en-decades", "senseid", "prefixsee"
        }},
        {   "TEST",
        {
        }}
    };
    str templates_(str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; str arg = args.body; auto & a = args;

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        str kind   = "{{" + name + "}}";

        for (const auto & [group, set] : rejected_templates) {
            if (set.find (name) != set.end()) {
                result.reject(report,
                "{{"+group+"}}");
                return "";
            }
        }

        if (name == "," or  // Oxford comma
            name == "lena") // Requests for attention in etymologies in Latin entries
        {
            output = "";
        }
        else
        if (name == "defdate")
        {
            if (a.complexity == 1) { output = "["+a[0]+"]"; } else
            if (a.complexity == 2) { output = "["+a[1]+"]"; } else
            kind += " quest";
        }
        else
        if (name == "gloss")
        {
            if (a.complexity == 1) { output = "("+a[0]+")"; } else
            if (a.complexity == 2) { output = "("+a[1]+")"; } else
            kind += " quest";
        }
        else
        if (name == "non-gloss definition")
        {
            if (a.complexity == 1) { output = "''"+a[0]+"''"; } else
            if (a.complexity == 2) { output = "''"+a[1]+"''"; } else
            kind += " quest";
        }
        else
        if (name == "IPAchar" or
            name == "glossary")
        {
            if (a.complexity == 1) { output = a[0]; } else
            if (a.complexity == 2) { output = a[1]; } else
            kind += " quest";
        }
        else
        if (name == "accent" // a
        or  name == "qualifier" // qua, i, q, qf, qual
        or  name == "sense")
        {
            if (name == "accent") for (str & arg : a.unnamed) {
                if (arg == "GA"    ) arg = "General American";
                if (arg == "GenAm" ) arg = "General American";
                if (arg == "RP"    ) arg = "Received Pronunciation";
                if (arg == "AU"    ) arg = "General Australian";
                if (arg == "AuE"   ) arg = "General Australian";
                if (arg == "Aus"   ) arg = "General Australian";
                if (arg == "AusE"  ) arg = "General Australian";
                if (arg == "GenAus") arg = "General Australian";
                if (arg == "CA"    ) arg = "Canada";
                if (arg == "NZ"    ) arg = "General New Zealand";
                if (arg == "cot–caught"          ) arg = "</i>cot–caught<i> merger";
                if (arg == "pin-pen"             ) arg = "</i>pin-pen<i> merger";
                if (arg == "father-bother"       ) arg = "</i>father-bother<i> merger";
                if (arg == "wine/whine"          ) arg = "without the </i>wine-whine<i> merger";
                if (arg == "horse-hoarse"        ) arg = "without the </i>horse–hoarse<i> merger";
                if (arg == "Mary-marry-merry"    ) arg = "</i>Mary–marry–merry<i> merger";
                if (arg == "non-Mary-marry-merry") arg = "</i>Mary–marry–merry<i> distinction";
            }
            if (a.opt.size() > 0) kind += " quest";
            output = str(a.unnamed, ", ");
            output = "(''"+output+"'')";
            if (name != "qualifier")
                output += ":";
        }
        else
        if (name.ends_with(" Hypernyms"))
        {
            kind = " Hypernyms";
            output = Templates[name];
            output.replace_all("\n", " ");
            output.strip();
            bracketer b;
            b.proceed_template = [&](str s){ return templates_(title, header, s, result); };
            b.proceed(output);
            output = b.output;
        }
        else
        {
            kind = "{{}}"; templates_statistics[__FILE__][name]++;
        }

        if (kind.contains(" quest")) kind += " !!!!!";
        if (output.contains("\n")) kind +=  " #br#";
        if (output.contains("\n")) report = "==== " + title + " ==== " + header + " ==== " + "\n\n" + report;
        if (output.contains("\n")) output.replace_all("\n", " ");
        if (kind != "{{}}") result.report (report + " => " + output + " == " + title, kind);
        return output;
    }

    Pass <entry, entry> templates = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("templates0", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                for (auto & line : content)
                {
                    bracketer b;
                    b.proceed_template = [&](str s){ return templates_(t, h, s, result); };
                    b.proceed(line);
                    line = b.output;
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }

        dump_templates_statistics(result);
    };
}
