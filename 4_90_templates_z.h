#pragma once
#include "4.h"
namespace pass4
{
    std::map<str, array<str>> z_report_s;
    std::map<str, array<str>> z_report_l;
    std::map<str, bool>       z_report_x;

    const array<str> skipped_templates = {
        "QUOTE", "quote", "rfdate", "RQ", "ISBN", "ref", "Q", // lex items
        "translation only", "caret notation of", // empty entry
        "name translit", // name/surname translit senses
        "projectlink~Wikipedia", // see also
        "seeSynonyms", // see also
        "etystub", // etymology
        "zh-l", "zh-m",
        "ja-r", "ja-l",
        "ko-l", "he-m",
        "he-l", "ne-l",
        "vi-l",
    };

    str templates_z_(str title, str header, str body, const str & line, Result<entry> & result)
    {
        args args (body); str name = args.name; auto & a = args;

        name.replace_all(":", "..");
        name.replace_all("/", "~");

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        args.kind  = "{{" + name + "}}";

        if (skipped_templates.contains(name)) return output;

        if (name.starts_with("U..en..")) a.kind = "U..en..";

        str zws = u8"\u200B"; // zero width space

        if (name == "'"             ) { output = zws + "'"  + zws; a.kind = "sybol apostroph"; } else
        if (name == "!"             ) { output = "_PIPE_";         a.kind = "sybol pipe"; } else
        if (name == "="             ) { output = "_EQUAL_";        a.kind = "sybol equal"; } else
        if (name == "unsupported"   )
        {
            a.kind = "sybol unsupported";
            if (a[0] == "."           ) { a.kind += " 0"; output = a[0]; }
            if (a[0] == " "           ) { a.kind += " 0"; output = a[0]; }
            if (a[0] == "_"           ) { a.kind += " 0"; output = a[0]; }
            if (a[0] == "[…]"         ) { a.kind += " 0"; output = a[0]; }
            if (a[0] == "_ _"         ) { a.kind += " 0"; output = a[0]; }
            if (a[0] == "period"      ) { a.kind += " 1"; output = "."; }
            if (a[0] == "<"           ) { a.kind += " 1"; output = "&lt;"; }
            if (a[0] == ">"           ) { a.kind += " 1"; output = "&gt;"; }
            if (a[0] == "<>"          ) { a.kind += " 1"; output = "&lt;&gt;"; }
            if (a[0] == "<3"          ) { a.kind += " 1"; output = "&lt;3"; }
            if (a[0] == "[ ]"         ) { a.kind += " 1"; output = "[ ]"; }
            if (a[0] == "square brackets") { a.kind += " 1"; output = "[ ]"; }
            if (a[0] == "#"           ) { a.kind += " 1"; output = "#"; }
            if (a[0] == "C sharp"     ) { a.kind += " 1"; output = "C#"; }
            if (a[0] == "|"           ) { a.kind += " 2"; output = "_PIPE_"; }
            if (a[0] == "="           ) { a.kind += " 2"; output = "_EQUAL_"; }
            if (a[0] == "pipe"        ) { a.kind += " 2"; output = "_PIPE_"; }
            if (a[0] == "colon equals") { a.kind += " 2"; output = "_COLONEQ_"; }
            if (a[0] == "left curly bracket" ) { a.kind += " 2"; output = "_LCURLY_"; }
            if (a[0] == "right curly bracket") { a.kind += " 2"; output = "_RCURLY_"; }
            if (a[0] == "curly brackets"     ) { a.kind += " 2"; output = "_LCURLY_ _RCURLY_"; }
            result.report ("\n", a.kind);
            result.report (line, a.kind);
        }
        else

        if (name == "CURRENTDAY"      ) { output = name; a.kind = name; } else
        if (name == "CURRENTMONTHNAME") { output = name; a.kind = name; } else
        if (name == "CURRENTYEAR"     ) { output = name; a.kind = name; } else

        if (name == "PL..species"   ) { output = "{{..}}"; a.kind = name; } else
        if (name == "PL..quote"     ) { output = "{{..}}"; a.kind = name; } else
        if (name.starts_with("R.." )) { output = "{{..}}"; a.kind = "R.."; } else

        if (name.starts_with("PL..")) { output = ""; a.kind = "PL.."; } else
        if (name.starts_with("mul-")) { output = ""; a.kind = "mul-"; } else
        if (name.starts_with("wiki")) { output = ""; a.kind = "wiki"; } else

        if (name == "U..en..imitating-challenged") output = "An ironic imitation of the perceived oversensitivity of language like ''mentally challenged''."; else
        if (name == "U..en..NNES-and-children"   ) output = "Generally an error made by children and non-native speakers."; else
        if (name == "U..en..NNES"   ) output = "Generally an error made by non-native speakers."; else
        if (name == "U..en..an h"   ) output = "Like many terms that start with a non-silent ''h'' but have emphasis on their second syllable, some people precede ''"+title+"'' with ''an'', others with ''a''."; else
        if (name == "U..en..equal"  ) output = "In mathematics, this adjective can be used in phrases like \"A and B are congruent\", \"A is congruent to B\", and, less commonly, \"A is congruent with B\"."; else
        if (name == "U..en..foreignism"   ) output = "Often written in italics (''"+title+"''), or pronounced as a "+(a.size() > 1 ? a[1] : "foreign")+" word."; else
        if (name == "U..en..-ese"   )
        {
            str an = a.acquire("an") == "" ? "a" : "an";
            output = "As with other nationalities formed with ''-ese'', the countable singular noun in reference to a person "
                "(as in \"I am "+an+" "+title+"\", \"writing about "+title+" cuisine as "+an+" "+title+"\") is uncommon and often taken as incorrect. "
                "In its place, the adjective is used, by itself (as in \"I am "+title+"\") or with a word like ''person'', ''man'', or ''woman'' "
                "(\"writing about "+title+" cuisine as "+an+" "+title+" person\").";
        }
        else
        if (name == "U..en..plurals of letters")
        {
            output = "There is some difference of opinion regarding the use of apostrophes "
                "in the pluralization of references to letters as symbols. ''New Fowler's Modern English Usage'', "
                "after noting that the usage has changed, states on page 602 that \"after letters an apostrophe is obligatory.\" "
                "The 15th edition of ''The Chicago Manual of Style'' states in paragraph 7.16, \"To avoid confusion, "
                "lowercase letters ... form the plural with an apostrophe and an s\". The ''Oxford Style Manual'' "
                "on page 116 advocates the use of common sense.";
        }
        else
        if (name.starts_with("U..en..")) output = ""; else
        if (name == "")
        {
        }
        else
        {
            a.kind = "{{}}"; templates_statistics [__FILE__][name]++;

            z_report_s[a.name] += report+" => "+output+" == "+header+" == "+title;
            z_report_l[a.name] += line; if (lexical_items.contains(header))
            z_report_x[a.name] = true;
        }

        if (a.kind.contains(" quest")) a.kind += " !!!!!";
        if (output.contains("\n")) a.kind +=  " #br#";
        if (output.contains("\n"))
            report = esc+" "+title+"\n"+report+"\n====\n"+output; else
            report = report+" => "+output+" == "+title+" == "+header;
        if (a.kind != "{{}}") result.report (report, a.kind);
        return output;
    }

    Pass <entry, entry> templates_z = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

        for (auto && [title, topic] : input)
        {
            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                for (auto & line : content)
                {
                    line.replace_all("{{unsupported|:}}", "_COLON_");
                    line.replace_all("{{unsupported|:)}}", "_SMILE1_");
                    line.replace_all("{{unsupported|:-)}}", "_SMILE2_");

                    bracketer b;
                    b.proceed_template = [&](str s){ return templates_z_(t, h, s, line, result); };
                    b.proceed(line);
                    line = b.output;
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }

        dump_templates_statistics(result);

        std::multimap<int, str> counter;
        for (auto [name, ss] : z_report_s) counter.emplace(1000-ss.size(), name);
        for (auto [nn, name] : counter)
        {
            array<str> templines;
            str t = Templates[name]; t.strip("\n ");
            while (t != "") { templines += t.upto(80); t.upto(80).erase(); }

            str z = "{{z}}";
            if (name.starts_with("R:"  )) z += " R..";
            if (name.starts_with("U:"  )) z += " U..";
            if (name.starts_with("PL:" )) z += " PL..";
            if (name.starts_with("SI-" )) z += " SI-";
            if (name.starts_with("mul-")) z += " mul-";
            if (name.starts_with("wiki")) z += " wiki";

            if (z_report_x[name]) z += " lexical";

            result.report(esc + "\n\"" + name + "\"\n" + esc + "\n", z);
            result.report("\n", z);
            for (str s : templines       ) result.report(s, z);
            result.report("\n", z);
            for (str s : z_report_s[name]) result.report(s, z);
            result.report("\n", z);
            for (str s : z_report_l[name]) result.report(s, z);
            result.report("\n", z);
        }
    };
}
