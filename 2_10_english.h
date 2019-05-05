Pass <pass1::entry, entry> english = [](auto & input, auto & output)
{
    Result result {__FILE__, output};

    std::map<str,int> smap, stmap, semap, somap; //headers;
    std::map<int,str> imap; 

    for (auto [title, topic_string] : input)
    {
        static int64_t nn = 0; if (++nn % 10'000 == 0) print("english ", nn, " entries ", input.cargo, " cargo ");

        array<str> topic = topic_string.split_by ("\n");

        for (str & s : topic)
        {
            if (s.found ( "==" ))
            {
                if (title.starts_with ("Template:")) { stmap [s]++; continue; }

                if (s.starts_with ( "==" ))
                {
                    if (s.found ( "English" )) semap [s]++; else smap [s]++; continue;
                }

                somap [s]++;

                

                //str header = s;
                //str l; header.spit_by (str::one_of{"="}, l, header, false, 1 );  sl.strip ( " \t" );  int level = sl.size ();
                //str r; header.spit_by (str::one_of{"="}, header, r, false,-1 );  sr.strip ( " \t" );  he.strip ( "[ ]\t" ); 
            }
        }
    }

    for (auto [h, n] : smap) imap [n] = h;

    for (auto [h, n] : smap) result.report (h + " " + std::to_string(n) + "\n", "map str to int");
    for (auto [n, h] : imap) result.report (h + " " + std::to_string(n) + "\n", "map int to str");

    for (auto [h, n] : semap) result.report (h + " " + std::to_string(n) + "\n", "english");
    for (auto [h, n] : stmap) result.report (h + " " + std::to_string(n) + "\n", "templates");
    for (auto [h, n] : somap) result.report (h + " " + std::to_string(n) + "\n", "others");

};


