Pass <str, entry> unxml = [](auto & input, auto & output)
{
    Result result {__FILE__, output};

    const std::vector<str> tagmap = 
    {
        "<page>",
            "<title>",
            // title here
            "</title>",
            "<ns>0</ns>",
            "<revision>",
                "<contributor>",
                    "<username>MewBot</username>", "<id>345682</id>",
                    "<username>NadandoBot</username>", "<id>394541</id>",
                    "<username>TheDaveBot</username>", "<id>20393</id>",
                "</contributor>",
                "<minor />",
                "<model>wikitext</model>",
                "<format>text/x-wiki</format>",
                "<text>xml:space=\"preserve\"</text>",
                "<text xml:space=\"preserve\">",
                // topic here
                "</text>",
            "</revision>",
        "</page>"
    };

    auto got = [](str & src, str & dst, str tag, bool inclusive)
    {
        int n = src.find(tag);
        if (n == str::nope) { dst += src; src = ""; return false; }
        if (inclusive) n += tag.size();
        dst += src.head(n);
        src.erase (0, n);
        return true;
    };

    str title;
    str topic;
    str tags;
    int mode = 0;

    for (str && s : input)
    {
        static int64_t nn = 0; if (++nn % 20'000 == 0) print("unxml ", nn, " chunks ", input.cargo, " cargo ");

        while (s != "")
        {
            switch (mode) {
            case 0: if (got(s, tags,  "<page>"  , true )) mode = 1; break;
            case 1: if (got(s, tags,  "<title>" , true )) mode = 2; break;
            case 2: if (got(s, title, "</title>", false)) mode = 3; break;
            case 3: if (got(s, tags,  "<text"   , true )) if (s.empty() || s.headed(' ') || s.headed('>')) mode = 4; break;
            case 4: if (got(s, tags,  ">"       , true )) mode = 5; break;
            case 5: if (got(s, topic, "<"       , false)) mode = 0; break;
            }

            if (mode == 0 && title.size () > 0)
            {
                bool accept = true; str report = "";

                if (title.headed("Wiktionary:")) { report = "Wiktionary"; accept = false; } else
                if (title.headed("Appendix:"  )) { report = "Appendix"  ; accept = false; } else
                if (title.headed("Category:"  )) { report = "Category"  ; accept = false; } else
                if (title.headed("Template:"  )) { report = "Template"  ; accept = true;  } else
                if (title.headed("Index:"     )) { report = "Index"     ; accept = false; } else
                if (title.found (":"          )) { report = "Meta"      ; accept = false; } else
                {
                    bool latin = false; for (signed char c : title) if (c > 0) { latin = true; break; }
                    if (!latin) { report = "Z"; accept = false; }
                }

                accept ?
                result.accept (entry {std::move(title), std::move(topic)}, report, true):
                result.reject (entry {std::move(title), std::move(topic)}, report, true);
                title = "";
                topic = "";
            }
        }

    //  result.reject (tags); // file would be huge - about half of the original xml

        tags.clear ();
    }
};
