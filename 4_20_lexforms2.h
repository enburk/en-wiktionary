#pragma once
#include "4.h"
namespace pass4
{
    static std::map<str, str> list_of_forms =
    {
        {   "Alternative form of"                                              , "alternative"              },
        {   "Alternative letter-case of"                                       , "alternative"              },
        {   "Alternative name of"                                              , "alternative"              },
        {   "Alternative spelling of"                                          , "alternative"              },
        {   "Alternative capitalization of"                                    , "alternative"              },
        {   "comparative form of"                                              , "comparative"              },
        {   "past participle of"                                               , "past participle"          },
        {   "plural of"                                                        , "plural"                   },
        {   "plural form of"                                                   , "plural"                   },
        {   "present participle of"                                            , "present participle"       },
        {   "Pronunciation spelling of"                                        , "alternative"              },
        {   "simple past of"                                                   , "simple past"              },
        {   "simple past tense of"                                             , "simple past"              },
        {   "simple past tense and past participle of"                         , "past and past participle" },
        {   "superlative form of"                                              , "superlative"              },
        {   "third-person singular simple present indicative form of"          , "third-person"             },
    };

    Pass <entry, entry> lexforms2 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};//UPDATING_REPORTS};

        array<entry> entries; entries.reserve (1024*1024);

        for (auto && entry : input) entries += entry;  print("============");

        for (auto & [title, topic] : entries)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0) logout("lexforms2", nn, input.cargo);

            bool complex = false; str report, origin;

            for (auto & [header, forms, content] : topic)
            {
                if (header == "etymology") continue;

                if (!lexical_items.contains(header)) { complex = true; continue; }

                for (auto & line : content)
                {
                    bool not_a_form = true;

                    for (auto & [form, what] : list_of_forms)
                    {
                        if (line.starts_with("''" + form + "''"))
                        {
                            not_a_form = false;

                            str s = line.from(form.size());
                            str x; s.split_by("'''", x, s);
                            str y; s.split_by("'''", s, y);
                            s.strip();

                            if (origin == "") origin = s; else
                            if (origin != s ) complex = true;

                            array<str> whats;
                            if (what == "past and past participle") { whats += "simple past"; whats += "past participle"; }
                            else whats += what;
                            for (auto what : whats)
                            {
                                bool found = false;
                                auto it = lexforms.find(s);
                                if (it != lexforms.end())
                                {
                                    for (auto & [form, ack, word] : it->second)
                                    {
                                        if (form == what && word == title) if (ack == "?" || ack == "+")
                                        {
                                            report = report == "" ? what : "x2";
                                            found = true; ack = "+"; break;
                                        }
                                    }
                                }
                                if (!found) lexforms[s] += lexform{what, "??", title};
                            }
                        }
                    }
                    if (not_a_form)
                    {
                        complex = true;
                    }
                }
            }

            if (!complex)
            {
                std::ostringstream sstream; sstream << entry{title, topic};
                result.report (sstream.str() + ">>>>> " + origin + " <<<<<\n\n", report);

                topic.clear();
                topic += paragraph();
                topic.back().header = ">>>";
                topic.back().forms = origin;
                topic.back().content += ">>>";
            }
            if (complex && origin != "")
            {
                std::ostringstream sstream; sstream << entry{title, topic};
                result.report (sstream.str() + ">>>>> " + origin + " <<<<<\n\n", "-");
            }

            result.accept (entry {std::move(title), std::move(topic)});
        }

        for (auto [title, forms] : lexforms)
        for (auto [form, ack, word] : forms)
        {
            str report = "";

            if (ack == "+") report = "302 found"; else
            if (!vocabulary.binary_found(title)
            ||  !vocabulary.binary_found(word)) report = "501 not implemented"; else
            if (!word.starts_with("more ")
            &&  !word.starts_with("most ")) report = "404 not found";

            if (report != "") result.report (title + " == " + form + " == " + ack + " == " + word, report);
        }
    };
}
                
