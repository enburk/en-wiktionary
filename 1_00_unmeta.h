#pragma once
#include "1.h"
namespace pass1
{
    Pass<pass0::entry, pass0::entry> unmeta = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, false};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 200'000 == 0) logout("unmeta ", nn, input.cargo);

            if (topic.starts_with("#REDIRECT")
            ||  topic.starts_with("#redirect"))
            {
                title.erase(0); // erase space for sort

                auto b = topic.find("[[", str::start_from(9));
                if (!b){ result.report (pass0::entry {title, topic}, "redirect broken"); break; }
                auto e = topic.find("]]", str::start_from(b.offset+2));
                if (!e){ result.report (pass0::entry {title, topic}, "redirect broken"); break; }
                
                str where = topic.from(b.offset+2).size(e.offset-b.offset-2);

                if (title.ascii_lowercased().starts_with("template:")
                !=  where.ascii_lowercased().starts_with("template:")
                ||  title.ascii_lowercased().starts_with("module:")
                !=  where.ascii_lowercased().starts_with("module:"))
                {
                     result.report(title + " => " + where, "redirects mismatch");
                }
                else
                if (title.ascii_lowercased().starts_with("template:"))
                {
                     redirect_templates [title.from(9)] = where.from(9);
                     result.report(str()+title.from(9) + " => " + where.from(9), "redirect templates");
                }
                else
                if (title.ascii_lowercased().starts_with("module:"))
                {
                     redirect_modules   [title.from(7)] = where.from(7);
                     result.report(str()+title.from(7) + " => " + where.from(7), "redirect modules");
                }
                else
                {
                     redirect      [title] = where;
                     result.report (title + " => " + where, "redirect");
                }
                        
                if(e.offset+2+2 < topic.size())
                result.report (pass0::entry {title, topic.from (e.offset+2)}, "redirects skipped tails"); 
                result.report (pass0::entry {std::move(title), std::move(topic)}, "redirects full topics");
                continue;
            }

            if (title.starts_with(" Template:"))
            {
                title.erase(0); // erase space for sort
                result.reject (pass0::entry{title, topic}, "templates", true);
                templates.emplace (std::move(title), std::move(topic));
                continue;
            }

            if (title.starts_with(" Module:" ))
            {
                title.erase(0); // erase space for sort
                result.reject (pass0::entry{title, topic}, "modules", true);
                modules.emplace (std::move(title), std::move(topic));
                continue;
            }

            result.accept (pass0::entry {std::move(title), std::move(topic)}, "", true);
        }

        // print("unmeta ", redirect          .size(), " redirect           ");
        // print("unmeta ", redirect_modules  .size(), " redirect_modules   ");
        // print("unmeta ", redirect_templates.size(), " redirect_templates ");
        // print("unmeta ", templates         .size(), " templates          ");
        // print("unmeta ", modules           .size(), " modules            ");
    };
}


