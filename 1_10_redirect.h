#pragma once
#include "1.h"
namespace pass1
{
    Pass<pass0::entry, pass0::entry> redirects = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        for (auto && [title, topic] : input)
        {
            if (topic.starts_with("#REDIRECT")
            ||  topic.starts_with("#redirect"))
            {
                auto b = topic.find("[[", str::start_from(9));
                if (!b){ result.report (pass0::entry{title, topic},
                    "redirect broken 1"); break; }

                auto e = topic.find("]]", str::start_from(b.offset+2));
                if (!e){ result.report (pass0::entry{title, topic},
                    "redirect broken 2"); break; }
                
                str where = topic.
                    from(b.offset+2).
                    size(e.offset-b.offset-2);

                if (title.ascii_lowercased().starts_with("template:")
                !=  where.ascii_lowercased().starts_with("template:")
                ||  title.ascii_lowercased().starts_with("module:")
                !=  where.ascii_lowercased().starts_with("module:"))
                {
                     result.report(title + " => " + where,
                         "redirects mismatch");
                }
                else
                if (title.ascii_lowercased().starts_with("template:"))
                {
                     redirect_templates [title.from(9)] = where.from(9);
                     result.report(str()+title.from(9) + " => " + where.from(9),
                         "redirect templates");
                }
                else
                if (title.ascii_lowercased().starts_with("module:"))
                {
                     redirect_modules   [title.from(7)] = where.from(7);
                     result.report(str()+title.from(7) + " => " + where.from(7),
                         "redirect modules");
                }
                else
                {
                     redirect      [title] = where;
                     result.report (title + " => " + where,
                         "redirect");
                }
                        
                if(e.offset+2+2 < topic.size())
                result.report (pass0::entry{title, topic.from (e.offset+2)}, "redirects skipped tails");
                result.report (pass0::entry{
                    std::move(title),
                    std::move(topic)},
                    "redirects full topics");

                continue;
            }

            output.push(pass0::entry{
                std::move(title),
                std::move(topic)});
        }

        if (not redirect.empty()) {
            print("total redirect ", redirect.size());
            print("total redirect_modules ", redirect_modules.size());
            print("total redirect_templates ", redirect_templates.size());
        }
    };
}


