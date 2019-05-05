struct Report
{
    static inline const char esc [] =
    "#############################################################################################################################";
    static inline const char sep [] =
    "##############################################";

    struct entry { string title; array<string> topic; };

    sys::file f;  PATH dir; str8 name;

    Report ( PATH dir, str8 name ) : dir (dir), name (name) {}

    void write ( str8 title, array<str8> topic0, array<str8> topic1 = array<str8>(), array<str8> complex = array<str8>() )
    {
        if( ! f.opened () )
        {
            RC rc = f.create ( dir / name + ".txt" );  if( ! rc.ok ) throw std::logic_error (rc.msg);
        }

            while (!topic.empty() && topic.front() == "" ) topic.erase  (0);
            while (!topic.empty() && topic.back () == "" ) topic.truncate();

        for   ( int n=0; n<topic0.size (); n++ ) topic0 [n].strip ();
        while ( topic0.size () > 0 && topic0 [0]     == "" ) topic0.erase (0);
        while ( topic0.size () > 0 && topic0.last () == "" ) topic0.trunc ();
        for   ( int n=1; n<topic0.size (); n++ ) if( topic0 [n] == "" && topic0 [n-1] == "" ){ topic0.erase (n); n--; }

        for   ( int n=0; n<topic1.size (); n++ ) topic1 [n].strip ();
        while ( topic1.size () > 0 && topic1 [0]     == "" ) topic1.erase (0);
        while ( topic1.size () > 0 && topic1.last () == "" ) topic1.trunc ();
        for   ( int n=1; n<topic1.size (); n++ ) if( topic1 [n] == "" && topic1 [n-1] == "" ){ topic1.erase (n); n--; }

        if( true )
        {
            f.write_str8 ( esc );
            f.write_str8 ( title );
            f.write_str8 ( esc );
            f.write_str8 ( "" );
            f.write_str8 ( topic0 );
            f.write_str8 ( "" );
        }
        if( complex.size () > 0 )
        {
            f.write_str8 ( sep );
            f.write_str8 ( "" );
            f.write_str8 ( complex );
            f.write_str8 ( "" );
        }
        if( complex.size () > 0 || topic1.size () > 0 )
        {
            f.write_str8 ( sep );
            f.write_str8 ( "" );
            f.write_str8 ( topic1 );
            f.write_str8 ( "" );
        }
    }

    void write ( str8 title )
    {
        if( ! f.opened () )
        {
            RC rc = f.create ( dir / name + ".txt" );  if( ! rc.ok ) throw std::logic_error (rc.msg);
        }
        if( true )
        {
            f.write_str8 ( title );
        }
    }
    void write ( array<str8> topic )
    {
        for( int i=0; i<topic.size (); i++ )  write ( topic [i] );
    }
};
