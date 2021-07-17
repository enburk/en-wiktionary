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
            "rfc-sense", "rfv-sense", "rfc", "rfclarify", "rfd-sense", // "rfv-pron",
            "rfref", "rfquote", "rfscript", "rfd-redundant", "rfm-sense",
            "rfd", "rfm", "rfv", // "rfdate",
            "attention", "checksense",
            "sic", 
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
            "hyp-top",  "hyp-mid",  "hyp-bottom", 
            "hyp-top1", "hyp-mid1", "hyp-bottom1",
            "hyp-top2", "hyp-mid2", "hyp-bottom2",
            "hyp-top3", "hyp-mid3", "hyp-bottom3", 
            "hyp-top4", "hyp-mid4", "hyp-bottom4", 
            "hyp-top5", "hyp-mid5", "hyp-bottom5",
        }},
        {   "pronunciation",
        {
            "audio", "audio-pron", "rhymes", "enPR", // non IPA pronunciation
        }},
        {   "etymology",
        {
            ",",    // Oxford comma
            "lena", // Requests for attention in etymologies in Latin entries
            "root", "nonlemma", "PIE word", "listen",  
        }},
        {   "seealso",
        {
            "projectlink", "projectlinks", "morse links", 
            "PL..commons", "PL..1911", "comcatlite", "ethnologue", 
            "wikipedia", "slim-wikipedia", "wikispecies", "interwiktionary",  
            "catlangname", "langlist", "langcat", "mul-script~Latn-list", 
            "ISO 639", "letters", "lookfrom", "Book-B", "punctuation", 
            "interfixsee", "prefixsee", "seeCites", "syndiff", 
        }},
        {   "categories",
        {
            "topics", "rootsee",
        }},
        {   "miscellaneous",
        {
            "Letter", "multiple images", "picdic", "color panel", 
            "examples", "examples-right", "broken ref",
            "en-decades", "senseid", "senseid-close", "hot sense", "tea room sense",
            "+preo", "+obj", "abbreviated", "colorbox", "colour_panel", "construed with",
            "ja-def", "Runr-def", "l2c", "maintenance line",  
            "anchor", "-",
        }},
        {   "TEST",
        {
        }}
    };
    str templates_(str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; auto & a = args;

        name.replace_all(":", "..");
        name.replace_all("/", "~");

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        args.kind  = "{{" + name + "}}";

        for (const auto & [group, set] : rejected_templates) {
            if (set.find (name) != set.end()) {
                result.reject(report,
                "{{"+group+"}}");
                return "";
            }
        }

        if (name == "glossary" or
            name == "pedlink"  or
            name == "IPAchar"  or
            name == "nobr")
        {
            output = a.link("", "");
        }
        else
        if (name == "ll"     or
            name == "link"   or
            name == "l-self" or
            name == "m-self" or
            name == "alter")
        {
            a.languaged();
            a.ignore("g");
            output = a.link("", "");
        }
        else
        if (name == "gloss")
        {
            output = a.link("(", ")");
        }
        else
        if (name == "non-gloss definition")
        {
            output = a.link("''", "''");
        }
        else
        if (name.ends_with(" Hypernyms"))
        {
            a.kind = "Hypernyms";
            output = Templates[a.name];
            output.replace_all("\n", " ");
            output.strip();
            bracketer b;
            b.proceed_template = [&](str s){ return templates_(title, header, s, result); };
            b.proceed(output);
            output = b.output;
        }
        else
        if (name.starts_with("list.."))
        {
            a.kind = "list..";
            output = Templates[a.name];
            output.replace_all("\n", " ");
            output.strip();
            bracketer b;
            b.proceed_template = [&](str s){ return templates_(title, header, s, result); };
            b.proceed(output);
            output = b.output;
        }
        else
        if (name == "list helper")
        {
            a.kind = "list helper";
            str title = a.acquire("title");
            output = "("+italic(title)+"): ";
            bracketer b;
            b.proceed_template = [&](str s){ return templates_(title, header, s, result); };
            b.proceed(str::list(a.unnamed));
            output += b.output;
        }
        else
        if (name == "list helper 2")
        {
            a.kind = "list helper 2";
            str title = a.acquire("title");
            str list  = a.acquire("list");
            output = "("+italic(title)+"): ";
            bracketer b;
            b.proceed_template = [&](str s){ return templates_(title, header, s, result); };
            b.proceed(list);
            output += b.output;
        }
        else
        {
            a.kind = "{{}}"; templates_statistics[__FILE__][name]++;
        }

        if (a.kind.contains(" quest")) a.kind += " !!!!!";
        if (output.contains("\n")) a.kind +=  " #br#";
        if (output.contains("\n")) report = "==== "+title+" ==== "+header+" ==== "+"\n\n" + report;
        if (output.contains("\n")) output.replace_all("\n", " ");
        if (a.kind != "{{}}") result.report (report + " => " + output + " == " + title, a.kind);
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
                    line.replace_all("E=mc²", "E&eq;mc²");

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
