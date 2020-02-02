#pragma once
#include "0.h"
namespace pass0
{
    Pass<entry, entry> meta = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, UPDATING_REPORTS};

        array<str> forms { "noun", "verb", "adj", "conj", "decl", "pron", "adjectives", "anagram", "convert", "translit" };

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("meta ", nn, input.cargo);

            if (title.starts_with("Template:"))
            {
                str name = title.from(9);
                str kind = 
                    name.starts_with ("User:"    ) ? "User" :  
                    name.starts_with ("User "    ) ? "User" :  
                    name.starts_with ("RQ:"      ) ? "RQ"   :  
                    name.starts_with ("R:"       ) ? "R"    :  
                    "";

                if (kind == "")
                if (name.size() >= 4 && name[3] == '-' ||
                    name.size() >= 3 && name[2] == '-' )
                {
                    if (auto ss = name.split_by(str::one_of{"- /"});
                        ss.size() >= 2 && ss[0] != "en" && forms.contains(ss[1]))
                        kind = "xxx-" + ss[1];
                }

                if (kind == "") if (name.ends_with("/documentation")) kind = "documentation";

                if (kind == "") if (topic.contains("====")) kind = "====";
                if (kind == "") if (topic.contains("===")) kind = "===";
                if (kind == "") if (topic.contains("==")) kind = "==";

                if (kind == "") { Templates [name] = topic;
                result.accept (entry {std::move(title), std::move(topic)}, "templates", true); } else
                result.reject (entry {std::move(title), std::move(topic)}, "templates " + kind);
                continue;
            }

            if (title.starts_with("Module:"))
            {
                str name = title.from(7);
                str kind = 
                    name.starts_with ("User:"    ) ? "User" :  
                    name.starts_with ("User "    ) ? "User" :  
                    name.starts_with ("RQ:"      ) ? "RQ"   :  
                    name.starts_with ("R:"       ) ? "R"    :  
                    name.starts_with ("zh/"      ) ? "zh"   :  
                    name.starts_with ("zh-"      ) ? "zh"   :  
                    name.starts_with ("vi/"      ) ? "vi"   :  
                    name.starts_with ("vi-"      ) ? "vi"   :  
                    name.starts_with ("data tables/") ? "data" :  
                    name.starts_with ("category tree/") ? "category" :  
                    name.contains    ("testcases") ? "testcases" :  
                    name.contains    ("wikidata" ) ? "wikidata"  :  
                    name.contains    ("appendix" ) ? "appendix"  :  
                    name.contains    ("sandbox"  ) ? "sandbox"   :  
                    name.contains    ("images"   ) ? "images"    :  
                    name.contains    ("cjk"      ) ? "cjk"       :  
                    "";

                if (kind == "")
                if (name.size() >= 4 && name[3] == '-' ||
                    name.size() >= 3 && name[2] == '-' )
                {
                    if (auto ss = name.split_by(str::one_of{"- /"});
                        ss.size() >= 2 && ss[0] != "en" && forms.contains(ss[1]))
                        kind = "xxx-" + ss[1];
                }

                if (kind == "") if (name.ends_with("/documentation")) kind = "documentation";

                if (kind == "") { Modules [name] = topic;
                result.accept (entry {std::move(title), std::move(topic)}, "modules", true); } else
                result.reject (entry {std::move(title), std::move(topic)}, "modules " + kind);
                continue;
            }

            result.accept (entry {std::move(title), std::move(topic)}, "", true);
        }
    };
}


