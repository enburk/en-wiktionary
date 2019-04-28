template<class type> struct array : public std::vector<type>
{
    using base = std::vector<type>;

    int size () const { return (int) base::size (); }

    void operator += (const type  & e) { base::push_back (e); }
    void operator += (const array & a) { base::insert (base::end (), a.begin (), a.end ()); }

    friend array operator + (const array & a, const type  & b) { array tt; tt += a; tt+= b; return tt; }
    friend array operator + (const type  & a, const array & b) { array tt; tt += a; tt+= b; return tt; }
};

struct str : public std::string
{
    using base = std::string;

    str (               ) : base ( ) {}
    str (const base & s ) : base (s) {}
    str (      base &&s ) : base (std::move(s)) {}
    str (const char * s ) : base (s) {}
    str (char c, int n=1) { if (n > 0) *this = base (n,c); }
    str (const str &  s ) = default;
    str (      str && s ) = default;

    int size () const { return (int) base::size (); }

    str & operator  = (const str & s){ base::operator  = (s); return *this; }
    str & operator += (const str & s){ base::operator += (s); return *this; }

    static const int max = std::numeric_limits<int>::max ();
    static const int nope = -1;

    str substr (int pos, int num) const { if (pos < 0) num += pos, pos = 0; num = std::min (num, size () - pos); return num > 0 ? base::substr (pos, num) : base (); }

    str from (int pos) const { return substr (pos, max); }
    str head (int num) const { return substr (0, num); }
    str tail (int num) const { return substr (size () - num, max); }

    bool headed (str s) const { if (size () < s.size ()) return false; for (int i=0; i<s.size (); i++) if ((*this) [          i] != s [            i]) return false; return true; }
    bool tailed (str s) const { if (size () < s.size ()) return false; for (int i=0; i<s.size (); i++) if ((*this) [size ()-1-i] != s [s.size ()-1-i]) return false; return true; }

    void append    (                  str s){ *this += s; }
    void insert    (int pos,          str s){ *this = head (pos) + s + from (pos); }
    void overwrite (int pos,          str s){ erase (pos, s.size ()); insert (pos, s); }
    void replace   (int pos, int num, str s){ erase (pos, num      ); insert (pos, s); }
    void erase     (int pos, int num = 1   ){ if (pos < 0) num += pos, pos = 0; num = std::min (num, size () - pos); if (num > 0) base::erase (pos, num); }
    void truncate  (int pos                ){ erase (pos, max); }
    void truncate  (                       ){ erase (size () - 1); }
    void strip     (const str & set = " "  ){ trimr (set); triml (set); }
    void triml     (const str & set = " "  ){ size_type n = find_first_not_of (set); *this = n == npos ? str () : from ((int) n  ); }
    void trimr     (const str & set = " "  ){ size_type n = find_last_not_of  (set); *this = n == npos ? str () : head ((int) n+1); }

    struct bool_{ bool b; bool_(bool b) : b(b) {}; }; // prevent { bool b = "ABC"; }

    int  find      (const str & s, int start_pos = 0) const { size_type rc = base::find  (s, start_pos); return rc == npos ? nope : (int) rc; }
    int  findr     (const str & s, int start_pos = 0) const { size_type rc = base::rfind (s, start_pos); return rc == npos ? nope : (int) rc; }
    bool found     (const str & s, int start_pos = 0) const { return find (s, start_pos) != nope; }

    int replace_all (str from, str to){ int pos = 0, nn = 0; while ((pos = find (from, pos)) != nope){ replace (pos, from.size (), to); pos += to.size (); nn++; }; return nn; }

};
/*
int     findf_set       ( const str8 & set,  bool_ in = true ) const { size_type rc = in.b ? base::find_first_of (set) : base::find_first_not_of (set); return rc == npos ? -1 : (int) rc; }
int     findr_set       ( const str8 & set,  bool_ in = true ) const { size_type rc = in.b ? base::find_last_of  (set) : base::find_last_not_of  (set); return rc == npos ? -1 : (int) rc; }

int     findf           ( int start_pos, const str8 & s, bool_ cases = true ) const { if( cases.b ){ size_type rc = base::find  ( s, start_pos ); return rc == npos ? -1 : (int) rc; }; return as_lower ().findf ( start_pos, s.as_lower (), true ); }
int     findr           ( int start_pos, const str8 & s, bool_ cases = true ) const { if( cases.b ){ size_type rc = base::rfind ( s, start_pos ); return rc == npos ? -1 : (int) rc; }; return as_lower ().findr ( start_pos, s.as_lower (), true ); }

int     findf_set       ( int start_pos, const str8 & set,  bool_ in = true ) const { size_type rc = in.b ? base::find_first_of ( set, start_pos ) : base::find_first_not_of ( set, start_pos ); return rc == npos ? -1 : (int) rc; }
int     findr_set       ( int start_pos, const str8 & set,  bool_ in = true ) const { size_type rc = in.b ? base::find_last_of  ( set, start_pos ) : base::find_last_not_of  ( set, start_pos ); return rc == npos ? -1 : (int) rc; }

void canonize  ()
    {
        int d = 0, s = 0; char cc = ' '; while (s < size ())
        {
            char c = at(s++);
            if( c == '\t' || c  == '\r' || c == '\n' )  c = ' ';
            if( c != ' '  || cc != ' '  ) at (d++) = c; cc = c;
        }
        if (cc == ' ' && d > 0) d--; truncate (d);
    }
    // P A R S I N G
    // sepn_case :
    // -1 - separator appends  to left string
    //  0 - separator excludes from both
    //  1 - separator prepends to right string

    bool    parse           ( const str8 & sept, str8 & a1, str8 & a2, bool cases = true, int sepn_case = 0 ) const { str8 aa = *this; int m = sept.size (); int n = aa.findf(sept,cases); a1 = n==-1 ? aa    : aa.head (n + (sepn_case==-1 ? m : 0)); a2 = n==-1 ? str8() : aa.from (n+m - (sepn_case== 1 ? m : 0)); return n != -1; }
    bool    parsr           ( const str8 & sept, str8 & a2, str8 & a1, bool cases = true, int sepn_case = 0 ) const { str8 aa = *this; int m = sept.size (); int n = aa.findr(sept,cases); a2 = n==-1 ? str8() : aa.head (n + (sepn_case==-1 ? m : 0)); a1 = n==-1 ? aa    : aa.from (n+m - (sepn_case== 1 ? m : 0)); return n != -1; }
    char    parse_set       ( const str8 & set,  str8 & a1, str8 & a2, bool in    = true, int sepn_case = 0 ) const { str8 aa = *this; int n = aa.findf_set(set,in); char t = n ==-1 ? L'\0' : aa[n]; a1 = n==-1 ? aa    :  aa.head (n + (sepn_case==-1 ? 1 : 0)); a2 = n==-1 ? str8() : aa.from (n+1 - (sepn_case== 1 ? 1 : 0)); return t; }
    char    parsr_set       ( const str8 & set,  str8 & a2, str8 & a1, bool in    = true, int sepn_case = 0 ) const { str8 aa = *this; int n = aa.findr_set(set,in); char t = n ==-1 ? L'\0' : aa[n]; a2 = n==-1 ? str8() :  aa.head (n + (sepn_case==-1 ? 1 : 0)); a1 = n==-1 ? aa    : aa.from (n+1 - (sepn_case== 1 ? 1 : 0)); return t; }

    std::vector<str8> parse ( const str8 & sept, bool cases = true ) const { std::vector<str8> aa;  str8 s, ss = *this;  while ( ss != "" ){ ss.parse ( sept, s, ss, cases ); aa.push_back (s); };  return aa; }
*/
