str proceed_link (str title, str header, str body, Result<entry> & result)
{
    str braced = "[[" + body + "]]";

    if (lexical_items.find(header) != lexical_items.end())
    {
        if (body.ascii_isalnum()) { result.report (braced, "link ascii"); return body; }
    }

    if (body.starts_with("File:")) { result.report (braced, "link File"); return ""; }

    result.report(braced, "[[");
    return braced;
}