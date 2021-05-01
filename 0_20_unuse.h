#pragma once
#include "0.h"
namespace pass0
{
    Pass<entry, entry> unuse = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, false};

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 500'000 == 0) logout("unuse", nn, input.cargo);

            bool accept = true; str report = "";

            if (title.starts_with("Unsupported titles/" )) { report = "Unsupported titles"; accept = false; } else

            if (title.starts_with("Module:"    )) { report = "meta Module"    ; accept = true;  } else
            if (title.starts_with("Template:"  )) { report = "meta Template"  ; accept = true;  } else
            if (title.starts_with("Wiktionary:")) { report = "meta Wiktionary"; accept = false; } else
            if (title.starts_with("Appendix:"  )) { report = "meta Appendix"  ; accept = false; } else
            if (title.starts_with("Category:"  )) { report = "meta Category"  ; accept = false; } else
            if (title.starts_with("Index:"     )) { report = "meta Index"     ; accept = false; } else
            if (title.contains   (":"          )) { report = "meta"           ; accept = false; } else
            {
                bool latin = false; for (signed char c : title) if (c > ' ') { latin = true; break; }
                if (!latin) { report = "non-English titles"; accept = false; }
            }

            if (accept
            && !topic.contains("English")
            && !topic.contains("Translingual")
            && !topic.starts_with("#REDIRECT")
            && !topic.starts_with("#redirect")
            && !title.starts_with("Template:")
            && !title.starts_with("Module:" ))
            {
                accept = false; report = "non-English topics";
            }

            accept ?
            result.accept (entry {std::move(title), std::move(topic)}, report, true):
            result.reject (entry {std::move(title), std::move(topic)}, report, true);
        }
    };
}


