#pragma once
#include "0.h"
namespace pass0
{
    Pass<entry, entry> unmeta = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, false};

        for (auto && [title, topic] : input)
        {
            if (title.starts_with("Template:"))
            {
                str name = title.from(9);
                str kind = 
                    name.starts_with ("User:") ? "User" :  
                    name.starts_with ("User ") ? "User" :  
                    name.starts_with ("RQ:"  ) ? "RQ"   :  
                    name.starts_with ("R:"   ) ? "R"    :  
                    "";

                if (kind == "")
                result.accept(entry{std::move(title), std::move(topic)}, "templates", true); else
                result.reject(entry{std::move(title), std::move(topic)}, "templates " + kind);
                continue;
            }

            if (title.starts_with("Module:"))
            {
                str name = title.from(7);
                str kind = 
                    name.starts_with ("User:") ? "User" :  
                    name.starts_with ("User ") ? "User" :  
                    name.starts_with ("R:"   ) ? "R"    :  
                    name.starts_with ("zh/"  ) ? "zh"   :  
                    name.starts_with ("zh-"  ) ? "zh"   :  
                    name.starts_with ("vi/"  ) ? "vi"   :  
                    name.starts_with ("vi-"  ) ? "vi"   :  
                    name.contains    ("cjk"  ) ? "cjk"  :  
                    name.starts_with ("data tables/"  ) ? "data" :  
                    name.starts_with ("category tree/") ? "category"  :  
                    name.contains    ("testcases"     ) ? "testcases" :  
                    name.contains    ("sandbox"       ) ? "sandbox"   :  
                    "";

                if (kind == "")
                result.accept(entry{std::move(title), std::move(topic)}, "modules", true); else
                result.reject(entry{std::move(title), std::move(topic)}, "modules " + kind);
                continue;
            }

            output.push(pass0::entry{
                std::move(title),
                std::move(topic)});
        }
    };
}


