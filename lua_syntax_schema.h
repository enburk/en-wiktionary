            if (t.kind == "literal"
            or  t.kind == "literall")
            {
                t.text.upto(1).erase();
                t.text.truncate();
                t.kind = "literal";
            }
            if (t.kind == "literalll")
            {
                t.text = std::move(t.kind);
                t.text.upto(1).erase();
                t.text.truncate();
                t.text.strip("=");
                t.text.upto(1).erase();
                t.text.truncate();
                t.kind = "literal";
            }
