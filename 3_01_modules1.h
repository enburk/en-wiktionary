#pragma once
#include "3.h"
namespace pass3
{
    Pass <entry, entry> modules1 = [](auto & input, auto & output)
    {
        Result result {__FILE__, output};

        array<str> groups = {"en-", "label"};

        bool first_time = true;

        for (auto && [title, topic] : input)
        {
            if (first_time) {
                first_time = false;

                for (auto & [name, text] : Modules)
                {
                    str report = "zzz";
                    for (auto group : groups)
                        if (name.starts_with(group))
                            report = group;

                    result.report(""
                    + esc + "\n" + ("Module:" + name)  + "\n"
                    + esc + "\n" + text + "\n", report);
                    result.report("Module:" + name, report + " #");
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
