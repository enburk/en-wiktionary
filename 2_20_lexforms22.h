#pragma once
#include "2.h"
#include "2_20_lexforms2.h"
namespace pass2
{
    str lexforms22_(str title, str header, str & forms, str body, Result<entry> & result)
    {
        args a(body);

        if (a.name == "form of")
        {
            str output = "{{"+body+"}}";
            a.kind = "{{form of}}";

            a.languaged();
            a.dotcapped();

            if (a.unnamed.size() > 0)
            {
                str lexical_form = a[0];
                output = a.capitalized(a[0]);
                a.unnamed.erase(0);
                a.complexity--;

                if (output == "") a.kind += " quest";
                output = "''" + output + " of'' ";
                output += a.link();

                str first = lexical_form.upto(1);
                if (first != first.ascii_lowercased() or a.dot != ".")
                    output += a.dot;
            }
            result.report("{{"+body+"}} => "+output+" ==== "+title+" ==== "+header, a.kind);
            return output;
        }

        auto it = lexforms2_internal::list_of_forms1.find(a.name);
        if (it == lexforms2_internal::list_of_forms1.end())
             it = lexforms2_internal::list_of_forms2.find(a.name);
        if (it == lexforms2_internal::list_of_forms2.end())
            return "{{" + body + "}}";

        str lexical_form = it->second;
        str output = "{{" + body + "}}";

        if (a.name != "en-past of"
        and a.name != "en-irregular plural of"
        and a.name != "en-third-person singular of"
        and a.name != "en-ing form of"
        and a.name != "en-simple past of"
        and a.name != "en-comparative of"
        and a.name != "en-superlative of")
        a.languaged();
        a.dotcapped();

        if (a.unnamed.size() > 0 and a[0] != "")
        {
            std::lock_guard lock{lexforms_mutex};
            lexforms[a[0]] += lexform{lexical_form, "-", title};
        }

        a.ignore("ending");
        a.ignore("pos");
        a.ignore("id");
        a.ignore("g");

        array<str> froms; str
        from = a.acquire("from" ); if (from != "") froms += from.split_by(", ");
        from = a.acquire("from2"); if (from != "") froms += from.split_by(", ");
        from = a.acquire("from3"); if (from != "") froms += from.split_by(", ");
        from = a.acquire("from4"); if (from != "") froms += from.split_by(", ");
        from = a.acquire("from5"); if (from != "") froms += from.split_by(", ");
        from = a.acquire("from6"); if (from != "") froms += from.split_by(", ");

        for (str & from : froms) {
            if (from == "AU") from = "Australia";
            if (from == "NZ") from = "New Zealand";
            if (from == "UK") from = "Britain";
            if (from == "AAVE") from = "African-American Vernacular English";
            if (from == "American") from = "American English";
            if (from == "Caribbean") from = "Caribbean English";
            if (from == "colloquial") from = "colloquial English";
            if (from == "dialect") from = "dialectal English";
            if (from == "dialectal") from = "dialectal English";
            if (from == "Dialectal") from = "dialectal English";
            if (from == "NYC") from = "New York City English";
            if (from == "Non-Oxford") from = "Non-Oxford British English";
            if (from == "southern US black") from = "southern US black English";
        }
        str extra = str::list(froms, ", ", " and ");
        if (extra.ends_with(" in") or
            extra.ends_with("American") or
            extra.ends_with("British") or
            extra.ends_with("Canadian") or
            extra.ends_with("Australian"))
            extra += " English"; 

        output = a.capitalized(lexical_form);

        if (output != "")
            output = "''" + output + "'' ";

        output += a.link();

        if (extra != "" and
           (lexical_form == "Standard spelling of" or
            lexical_form == "standard spelling of")) {
            extra[0] = str::ascii_toupper(extra[0]);
            output[2] = str::ascii_tolower(output[2]);
            if (extra.ends_with(" spelling"))
                extra.resize(extra.size()-9);
            output = "''" + extra + "'' " + output;
            a.kind = "extra standard";
        }
        else if (extra != "") {
            if (output.ends_with(".")) output.truncate();
            output += ", ''representing " + extra + "''";
            a.kind = "extra";
        }

        str first = lexical_form.upto(1);
        if (first != first.ascii_lowercased() or a.dot != ".")
            output += a.dot;

        result.report("{{"+body+"}} => "+output+" ==== "+title+" ==== "+header, a.kind + a.info);
        return output;
    }

    Pass<entry, entry> lexforms22 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        std::set<entry> entries;

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0)
                logout("lexforms22", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;
                auto & f = forms;

                bracketer b;
                b.proceed_sbrakets  = [&] (str s) { return   "[" + s + "]"  ; };
                b.proceed_qbrakets  = [&] (str s) { return   "{" + s + "}"  ; };
                b.proceed_link      = [&] (str s) { return  "[[" + s + "]]" ; };
                b.proceed_parameter = [&] (str s) { return "{{{" + s + "}}}"; };
                b.proceed_template  = [&] (str s) { return lexforms22_(t, h, f, s, result); };
                b.proceed(content);
        
                content = std::move(b.output);
            }

            entries.emplace(entry{
                std::move(title),
                std::move(topic)});
        }

        for (auto && [title, topic] : entries)
        {
            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
