#pragma once
#include "4.h"
namespace pass4
{
	static std::map<str,str> placetype_aliases;
	static std::map<str,str> placename_aliases;

	static array<str> placetypes = {
		"administrative village", "administrative centre", "airport", "ancient capital", "ancient settlement", 
		"area", "atoll", "autonomous community", "autonomous oblast", "autonomous okrug", "autonomous region", 
		"autonomous republic", "bay", "beach", "borough", "borough seat", "canton", "capital city", "census area",
		"census-designated place", "city", "city-state", "civil parish", "collectivity", "colony", "commonwealth",
		"community development block", "constituent country", "continent", "council area", "country", "county",
		"county-administered city", "county borough", "county seat", "county town", "department", "dependency", 
		"dependent territory", "desert", "district", "district municipality", "division", "enclave", "exclave",
		"federal city", "federal subject", "fictional location", "forest", "ghost town", "governorate", "gulf",
		"headland", "hill", "historical capital", "historical county", "historical polity",
		"historical political subdivision", "historical region", "historical settlement", "island", "kibbutz",
		"krai", "lake", "largest city", "local government district", "London borough", "macroregion",
		"marginal sea", "metropolitan borough", "metropolitan city", "moor", "mountain", "mountain pass",
		"municipal district", "municipality", "mythological location", "neighborhood", "non-city capital",
		"oblast", "ocean", "okrug", "parish", "parish municipality", "parish seat", "park", "peninsula",
		"periphery", "planned community", "polity", "prefecture", "prefecture-level city", "province", "raion",
		"regency", "region", "regional district", "regional county municipality", "regional municipality",
		"regional unit", "republic", "river", "royal borough", "rural municipality", "satrapy", "sea",
		"special administrative region", "star", "state", "strait", "subdistrict", "subdivision",
		"subprefecture", "subprovince", "subprovincial city", "subregion", "suburb", "tehsil", "territory",
		"town", "township", "township municipality", "traditional region", "tributary", "unincorporated community",
		"union territory", "unitary authority", "unitary district", "united township municipality",
		"unrecognized country", "valley", "village", "village municipality", "voivodeship", "volcano", 
		"Welsh community",
		// + of
		"emirate",
		"capital",
		"capital and largest city",
		"shire town",
		"commune",
		"ward",
		// + in
		"community",
		"settlement",
		"place",
		"locale",
		"hamlet",
	};

	static array<str> placetypes_of;
	static array<str> placetypes_in = {
		"airport", "ancient settlement", "area", "atoll", "beach", "census area", "census-designated place", "city",
		"city-state", "community development block", "continent", "country", "county-administered city", "desert",
		"fictional location", "forest", "ghost town", "headland", "hill", "historical polity", "historical region",
		"historical settlement", "island", "kibbutz", "lake", "largest city", "moor", "mountain", "mountain pass",
		"mythological location", "ocean", "park", "peninsula", "planned community", "polity", "river", "sea", "star",
		"strait", "subprovincial city", "tehsil", "town", "township", "traditional region", "unincorporated community",
		"unitary authority", "unitary district", "united township municipality", "unrecognized country", "valley", 
		"village", "volcano",
		// +
		"community",
		"settlement",
		"place",
		"locale",
		"hamlet",
		"parish", // !
		"region", // !
	};

	static array<str> placenames_the = {
		"Cyclades",
		"Dodecanese",
		"Bronx",
		"Holy Roman Empire",
		"North Island",
		"South Island",
		"Balkans",
		"Russian Far East",
		"Caribbean",
		"Caucasus",
		"Middle East",
		"North Caucasus",
		"South Caucasus",
		"West Bank",
		"San Fernando Valley",
		// + 
		"Bahamas", "Comoros", "Gambia", "Maldives", "Netherlands", "Philippines", "United Arab Emirates", 
		"United Kingdom", "United States", "Soviet Union",
	};

	static array<str> placetypes_the = {
		"administrative centre",
		"ancient capital",
		"borough seat",
		"capital city",
		"county seat",
		"county town",
		"historical capital",
		"largest city",
		"non-city capital",
		"parish seat",
		// + 
		"capital", 
		"capital and largest city",
		"shire town",
	};

	static array<str> placetypes_a = {
		"union territory",
		"unitary authority",
		"unitary district",
		"united township municipality",
		// +
		"county",
		"plateau",
		"town",
		"census-designated place",
	};

	static array<str> placetypes_Suf = {
		"autonomous oblast",
		"autonomous okrug",
		"census area",
		"krai",
		"oblast",
		"okrug",
		"raion",
		"regional county municipality",
	};

	static array<str> placetypes_suf = {
		"canton",
		"civil parish",
		"community development block",
		"council area",
		"county borough",
		"department",
		"district",
		"district municipality",
		"governorate",
		"local government district",
		"London borough",
		"parish",
		"subdivision",
		"tehsil",
		"Welsh community",
		"prefecture", // +
		"borough", // +
	};

	str lcfirst (str s) { if (s != "") s[0] = str::ascii_tolower(s[0]); return s; }
	str ucfirst (str s) { if (s != "") s[0] = str::ascii_toupper(s[0]); return s; }

	auto map (const std::map<str,str> & m, const str & s) {
		auto it = m.find(s); return it != m.end() ? it->second : s;	};

	auto match (str s, str pattern) {
		return std::regex_match(s, std::regex(pattern)); };

	auto the (str type, str name)
	{
		return
		placenames_the.contains(name)
		or  type == "gulf"
		or  type == "ocean"
		or  type == "river"
		or  type == "sea"
		or  type == "voivodeship"
		or (type == "bay"      and match(name, "^Bay of "))
		or (type == "lake"     and match(name, "^Lake of "))
		or (type == "country"  and match(name, "^Republic of "))
		or (type == "country"  and match(name, " Republic of ")) // +
		or (type == "country"  and match(name, " States of ")) // +
		or (type == "country"  and match(name, "^State of ")) // +
		or (type == "country"  and match(name, " Republic$"))
		or (type == "republic" and match(name, "^Republic of "))
		or (type == "republic" and match(name, " Republic$"))
		or (type == "region"   and match(name, " [Rr]egion$"))
		or (type == "river"    and match(name, " River$"))
		or (type == "county"   and match(name, "^Shire of "))
		or (type == "local government area"      and match(name, "^Shire of "))
		or (type == "Indian reservation"         and match(name, " Reservation"))
		or (type == "Indian reservation"         and match(name, " Nation"))
		or (type == "tribal jurisdictional area" and match(name, " Reservation"))
		or (type == "tribal jurisdictional area" and match(name, " Nation"))
		or  match(name, "^Isle of ")
		or  match(name, " Islands$")
		or  match(name, " Mountains$")
		or  match(name, " Empire$")
		or  match(name, " Country$")
		or  match(name, " Region$")
		or  match(name, " District$")
		or  match(name, "^City of ")
		or  match(name, " Territory$") // +
		or  match(name, " Territories$") // +
		;
	};

	auto naked (str type) -> str
	{
		if (type == "") return "";
		type = type.split_by(":").front();
		type = type.split_by(" (").front();
		type = map(placetype_aliases, type);
		auto pp = type.split_by(" "); int n = pp.size();
		str pp4 = n < 4 ? "" : pp[n-4] + " " + pp[n-3] + " " + pp[n-2] + " " + pp[n-1];
		str pp3 = n < 3 ? "" : pp[n-3] + " " + pp[n-2] + " " + pp[n-1];
		str pp2 = n < 2 ? "" : pp[n-2] + " " + pp[n-1];
		str pp1 = n < 1 ? "" : pp[n-1];
		return
			placetypes.contains(pp4) ? pp4:
			placetypes.contains(pp3) ? pp3:
			placetypes.contains(pp2) ? pp2:
			placetypes.contains(pp1) ? pp1: "";
	};

	auto affixed (str p1, str p2, bool articled = false)
	{
		str suf;
		if (p1.ends_with(":suf") or
			p1.ends_with(":Suf"))
			p1.split_by(":", p1, suf);

		str pref;
		if (p1.ends_with(":pref") or
			p1.ends_with(":Pref"))
			p1.split_by(":", p1, pref);

		p1 = map(placetype_aliases, p1);
		p2 = map(placename_aliases, p2);
		if (p1 == "city" and
			p2 == "New York")
			p2 = "New York City";

		str article = articled and the(p1, p2) ? "the" : "";

		if (suf == "" and not
			p2.ascii_lowercased().ends_with(p1)) {
			if (placetypes_suf.contains(p1)) suf = "suf";
			if (placetypes_Suf.contains(p1)) suf = "Suf";
		}

		if (suf == "suf") suf = p1;
		if (suf == "Suf") suf = ucfirst(p1);
		if (suf == "London borough") suf = "borough";
		if (suf == "Welsh community") suf = "community";
		if (suf == "local government district") suf = "district";
		if (suf != "") p2 += " " + suf;

		if (pref == "pref") pref = p1;
		if (pref == "Pref") pref = ucfirst(p1);
		if (pref != "") p2 = "the " +
			pref + " of " + p2;

		if (article != "") p2 =
			article + " " + p2;
		return p2;
	};

    str templates_places__(array<str> unnamed, str & kind, str & seat)
    {
		auto uu =
		unnamed.front().split_by("/");
		unnamed.erase(0);

		kind +=
		" "	+ std::to_string(min(3,uu.size())) +
		" "	+ std::to_string(min(3,unnamed.size()));

		str output;
		str state;
		str type;

		for (str u : uu)
		{
			u = map(placetype_aliases, u);

			str t = naked(u); if (t != "") type = t;

			if (placetypes.contains(type))
			{
				if (output.ends_with(" and") or
					output.ends_with(" or"))
					output += " ";
				else
				if (output != "")
				{
					output +=
						u == "or"  or
						u == "and" or
						u.starts_with("or " ) or
						u.starts_with("and ") or
						u.starts_with("("   ) ?
						" " : ", ";
					output += 
						placetypes_the.contains(u) ? "the ":
						placetypes_a  .contains(u) ? "a ":
						"";
				}
			}
			else
			if (output != "")
				output += " ";

			output += u;
		}

		if (type == "borough")
			seat =  "Borough";

		for (str u : unnamed)
		{
			str u1, u2; u.split_by("/", u1, u2);

			if (u2 == "")
			{
				output +=
					u.starts_with("or " ) or u == "or"  or
					u.starts_with("and ") or u == "and" or
					u.starts_with("("   ) ? " " :
					u.starts_with("in " ) or u == "in"  or
					u.starts_with("on " ) or u == "on"  or
					u.starts_with("of " ) or u == "of"  or
					false ? state != "" ?
					", " : " " :
					", ";
				output += u;
				state = "unplaced";
			}
			else
			{
				if (state == "")
					output += placetypes_of.contains(type) ?
						" of" : " in";

				if (u2.contains("/")) kind += " quest";

				output += state == "placed" ?
				", " + affixed(u1, u2, false):
				" "  + affixed(u1, u2, state == "");
				state = "placed";
			}
		}

		return output;
	}

    str templates_places_(str title, str header, str body, Result<entry> & result)
    {
        args args (body); str name = args.name; auto & a = args;
        args.languaged();

        str output = "{{" + body + "}}";
        str report = "{{" + body + "}}";
        args.kind  = "{{" + name + "}}";

        if (name == "place")
        {
			str seat = "County";

			auto angles = [](str & text, str & kind)
			{
				for (int e, b = 0; ; )
				{
					auto rb = text.find("<<", str::start_from(b)); b = rb.offset; if (!rb) break;
					auto re = text.find(">>", str::start_from(b)); e = re.offset; if (!re) { kind += "-broken";
					auto re = text.find(">" , str::start_from(b)); e = re.offset; if (!re) { kind += "-broken"; break; } }

					auto part = text.from(b+2).size(e-b-2);
					auto full = text.from(b+0).size(e-b+2);
					str s1, s2; str(part).split_by("/", s1, s2);
					if (s2 == "") s2 = s1; else s2 = affixed(s1, s2);
					if (s1.contains(":")) kind += "..";
					full.replace_by(s2);
				}
			};

			a.unnamed.erase_all("");

			str def = a.acquire("def");
			if (def != "")
			{
				a.kind += " def"; output = def;
			}
			else
			if (body.contains("<<") and a.unnamed.size() == 1)
			{
				a.kind += " new";
				a.ignore("a");
				output = a[0];
				angles(output, a.kind);
				result.report (esc + " " + title
					+ "\n" + report
					+ "\n" + output,
					a.kind + ".lines");
			}
			else
			{
				output = "";
				auto & u = a.unnamed;

				a.kind += " old"; if (body.contains("<<")) {
				a.kind += " new"; for (auto & s : u) angles(s, a.kind); }

				if (body.contains("pref:") or
					body.contains("//") or
					body.contains("|;") or
					false)
					result.report(report,
						"quest");

				while (true)
				{
					bool parted = false;

					for (int i=0; i<u.size(); i++)
					{
						if (u[i] == ";") {
							str kind; // ignore it
							output += templates_places__(u.upto(i), kind, seat);
							output += "; ";
							a.kind += ";";
							u.upto(i+1).erase();
							parted = true;
							break;
						}
					}

					if (not parted) break;
				}
				output += templates_places__(u, a.kind, seat);

				str A = a.acquire("a");

				if (A == "")
					for (str s : placetypes_the)
						if (output.starts_with(s + " of "))
							A = "The";

				if (A == "" and output != ""
					and not output.starts_with("a ")
					and not output.starts_with("A ")
					and not output.starts_with("an ")
					and not output.starts_with("An ")
					and not output.starts_with("the ")
					and not output.starts_with("The "))
					A = str("AEIOUaeiou").contains(output[0]) ? "An" : "A";

				if (A != "" and not output.starts_with("{{"))
					output = A + " " + output;
			}

			a.ignore("cat");

			array<str> qq;
            qq = a.acquire_all("capital");
			if  (qq.size() > 0) output += ". Capital";
			if  (qq.size() > 1) output += "s";
			if  (qq.size() > 0) output += ": "
				+ str::list(qq, ", ", " and ");

            qq = a.acquire_all("official");
			if  (qq.size() > 0) output += ". Official name";
			if  (qq.size() > 1) output += "s";
			if  (qq.size() > 0) output += ": "
				+ str::list(qq, ", ", " and ");

            qq = a.acquire_all("seat");
			if  (qq.size() > 0) output += ". " + seat + " seat";
			if  (qq.size() > 1) output += "s";
			if  (qq.size() > 0) output += ": "
				+ str::list(qq, ", ", " and ");

            qq = a.acquire_all("shire town");
			if  (qq.size() > 0) output += ". Shire town";
			if  (qq.size() > 1) output += "s";
			if  (qq.size() > 0) output += ": "
				+ str::list(qq, ", ", " and ");

            str q;
            q = a.acquire("caplc"       ); if (q != "") { output += ". Capital and largest city: " + q; }
            q = a.acquire("largest city"); if (q != "") { output += ". Largest city: "             + q; }
            q = a.acquire("modern"      ); if (q != "") { output += "; modern: "                   + q; }
            q = a.acquire("t1"          ); if (q != "") { output  = q + " (" + lcfirst(output) + ")"; }
            q = a.acquire("t"           ); if (q != "") { output  = q + " (" + lcfirst(output) + ")"; }

            if (not a.opt.empty()) a.kind += " opt";

			for (int i=0; i<3; i++)
			{
				bool fix = false; str prefix = output;
				fix |= 0 < output.replace_all(" en:", " ");
				fix |= 0 < output.replace_all("  ", " ");
				fix |= 0 < output.replace_all("..", ".");
				fix |= 0 < output.replace_all(".,", ",");
				fix |= 0 < output.replace_all(".;", ";");
				fix |= 0 < output.replace_all(";;", ";");
				fix |= 0 < output.replace_all(",,", ",");
				fix |= 0 < output.replace_all(",;", ",");
				fix |= 0 < output.replace_all(";,", ";");
				fix |= 0 < output.replace_all(" ;", ";");
				fix |= 0 < output.replace_all(" ,", ",");
				fix |= 0 < output.replace_all(" .", ".");
				fix |= 0 < output.replace_all(" in in ", " in ");
				fix |= 0 < output.replace_all(" of of ", " of ");
				fix |= 0 < output.replace_all(" on on ", " on ");
				fix |= 0 < output.replace_all(" the the ", " the ");
				fix |= 0 < output.replace_all(" an an ", " an ");
				fix |= 0 < output.replace_all(" a a ", " a ");
				fix |= 0 < output.replace_all(" in, the, ", " in the ");
				fix |= 0 < output.replace_all(" on, the, ", " on the ");
				fix |= 0 < output.replace_all(" of, the, ", " of the ");
				fix |= 0 < output.replace_all(" the, ", " the ");
				fix |= 0 < output.replace_all(" an, ", " an ");
				fix |= 0 < output.replace_all(" a, ", " a ");
				fix |= 0 < output.replace_all(" in, ", " in ");
				fix |= 0 < output.replace_all(" on, ", " on ");
				fix |= 0 < output.replace_all(" of, ", " of ");
				fix |= 0 < output.replace_all(" or, ", " or ");
				fix |= 0 < output.replace_all(", or ", " or ");
				fix |= 0 < output.replace_all(" and, ", " and ");
				if (fix) result.report (esc + " " + title
					+ "\n" + report
					+ "\n" + prefix
					+ "\n" + output,
					"fix" + std::to_string(i));
			}
        }
        else
        {
            a.kind = "{{}}"; templates_statistics [__FILE__][name]++;
        }

        if (a.kind.contains(" quest")) a.kind += " !!!!!";
        if (output.contains("\n")) a.kind +=  " #br#";
        if (output.contains("\n")) report = "==== "+title+" ==== "+header+" ==== "+"\n\n" + report;
        if (output.contains("\n")) output.replace_all("\n", " ");
        if (a.kind != "{{}}") result.report (report + " => " + output + " == " + title, a.kind);
        return output;
    }

    Pass <entry, entry> templates_places = [](auto & input, auto & output)
    {
        Result result {__FILE__, output, true};

		auto & a = placetype_aliases;
		a["c"] = "country";
		a["cc"] = "constituent country";
		a["p"] = "province";
		a["ap"] = "autonomous province";
		a["r"] = "region";
		a["ar"] = "autonomous region";
		a["adr"] = "administrative region";
		a["sar"] = "special administrative region";
		a["s"] = "state";
		a["arch"] = "archipelago";
		a["bor"] = "borough";
		a["can"] = "canton";
		a["caplc"] = "capital and largest city", // +
		a["carea"] = "council area";
		a["cdblock"] = "community development block";
		a["cdep"] = "Crown dependency";
		a["cdp"] = "census-designated place";
		a["CDP"] = "census-designated place";
		a["co"] = "county";
		a["cobor"] = "county borough";
		a["colcity"] = "county-level city";
		a["coll"] = "collectivity";
		a["comm"] = "community";
		a["acomm"] = "autonomous community";
		a["cont"] = "continent";
		a["contregion"] = "continental region"; // +
		a["cpar"] = "civil parish";
		a["dep"] = "dependency";
		a["dept"] = "department";
		a["dist"] = "district";
		a["distmun"] = "district municipality";
		a["div"] = "division";
		a["gov"] = "governorate";
		a["govnat"] = "governorate";
		a["ires"] = "Indian reservation";
		a["isl"] = "island";
		a["lbor"] = "London borough";
		a["lgarea"] = "local government area";
		a["lgdist"] = "local government district";
		a["metbor"] = "metropolitan borough";
		a["metcity"] = "metropolitan city";
		a["mtn"] = "mountain";
		a["mun"] = "municipality";
		a["mundist"] = "municipal district";
		a["non-city capital"] = "capital"; // +
		a["obl"] = "oblast";
		a["aobl"] = "autonomous oblast";
		a["okr"] = "okrug";
		a["aokr"] = "autonomous okrug";
		a["par"] = "parish";
		a["parmun"] = "parish municipality";
		a["pen"] = "peninsula";
		a["populated place"] = "locality";
		a["pref"] = "prefecture";
		a["preflcity"] = "prefecture-level city";
		a["apref"] = "autonomous prefecture";
		a["rep"] = "republic";
		a["arep"] = "autonomous republic";
		a["riv"] = "river";
		a["rcomun"] = "regional county municipality";
		a["rdist"] = "regional district";
		a["rmun"] = "regional municipality";
		a["robor"] = "royal borough";
		a["runit"] = "regional unit";
		a["rurmun"] = "rural municipality";
		a["terrauth"] = "territorial authority";
		a["terr"] = "territory";
		a["aterr"] = "autonomous territory";
		a["uterr"] = "union territory";
		a["tjarea"] = "tribal jurisdictional area";
		a["twp"] = "township";
		a["twpmun"] = "township municipality";
		a["utwpmun"] = "united township municipality";
		a["val"] = "valley";
		a["voi"] = "voivodeship";
		a["wcomm"] = "Welsh community";
		a["range"] = "mountain range";
		a["departmental capital"] = "department capital";
		a["home-rule city"] = "home rule city";
		a["home-rule municipality"] = "home rule municipality";
		a["sub-provincial city"] = "subprovincial city";
		a["sub-prefecture-level city"] = "sub-prefectural city";
		a["nonmetropolitan county"] = "non-metropolitan county";
		a["inner-city area"] = "inner city area";
		a["Welsh community"] = "community";

		auto & b = placename_aliases;
		b["Valencian Community"] = "Valencia";
		b["Washington; DC"] = "Washington; D.C.";
		b["Washington D.C."] = "Washington; D.C.";
		b["Washington DC"] = "Washington; D.C.";
		b["US"] = "United States";
		b["U.S."] = "United States";
		b["USA"] = "United States";
		b["U.S.A."] = "United States";
		b["United States of America"] = "United States";
		b["UK"] = "United Kingdom";
		b["UAE"] = "United Arab Emirates";
		b["Macedonia"] = "North Macedonia";
		b["Republic of North Macedonia"] = "North Macedonia";
		b["Republic of Macedonia"] = "North Macedonia";
		b["Republic of Ireland"] = "Ireland";
		b["Republic of Armenia"] = "Armenia";
		b["Côte d'Ivoire"] = "Ivory Coast";
		b["Czechia"] = "Czech Republic";
		b["Vatican"] = "Vatican City";
		b["Bosnia and Hercegovina"] = "Bosnia and Herzegovina";
		b["Northern Ostrobothnia"] = "North Ostrobothnia";
		b["Southern Ostrobothnia"] = "South Ostrobothnia";
		b["North Savo"] = "Northern Savonia";
		b["South Savo"] = "Southern Savonia";
		b["Päijät-Häme"] = "Päijänne Tavastia";
		b["Kanta-Häme"] = "Tavastia Proper";
		b["Åland"] = "Åland Islands";
		b["Kabardino-Balkarian Republic"] = "Kabardino-Balkar Republic";
		b["Tyva Republic"] = "Tuva Republic";
		b["Mecklenburg-Western Pomerania"] = "Mecklenburg-Vorpommern";
		b["Mexico"] = "State of Mexico";
		b["U.S. Virgin Islands"] = "United States Virgin Islands";
		b["US Virgin Islands"] = "United States Virgin Islands";

		for (str s : placetypes)
			if (not	placetypes_in.contains(s))
				placetypes_of += s;

		std::map<str,str> unittest = 
		{
			{"{{place|en|a=a|river|c/USA}}", "a river in the United States"},
			{"{{place|en|council area|cc/Scotland}}", "A council area of Scotland"},
			{"{{place|en|state|c/US|capital=Montgomery|largest city=Birmingham}}",
			"A state of the United States. Capital: Montgomery. Largest city: Birmingham"},
			{"{{place|en|hamlet/and/census-designated place|s/New York}}", "A hamlet and census-designated place in New York"},
			{"{{place|en|town/and/municipality/capital|c/Liechtenstein}}", "A town and municipality, the capital of Liechtenstein"},
			{"{{place|en|region/(one of 20)|c/Finland}}", "A region (one of 20) of Finland"},
			{"{{place|en|city/and/county/caplc|cc/Wales}}", "A city and county, the capital and largest city of Wales"},
			{"{{place|en|former transitional government|c/USA}}", "A former transitional government in the United States"},	// in ???
			{"{{place|en|a=The|barony|which contains these islands.}}", "The barony which contains these islands."},
			{"{{place|en|zoo|, Whipsnade Zoo, near the village}}", "A zoo, Whipsnade Zoo, near the village"},
			{"{{place|en|city/county/and/unitary authority|in southwest|cc/England}}", "A city, a county and unitary authority in southwest England"},
			{"{{place|en|capital city|emirate:suf/Ajman|c/United Arab Emirates}}", "The capital city of Ajman emirate, United Arab Emirates"},
			{"{{place|en|borough|s/Alaska|c/USA|seat=Sand Point}}", "A borough of Alaska, United States. Borough seat: Sand Point"},
			{"{{place|en|village/or/hamlet|co/Down|p/Northern Ireland}}", "A village or hamlet in County Down, Northern Ireland"},
			{"{{place|en|special ward|pref/Tokyo|c/Japan}}", "A special ward of Tokyo prefecture, Japan"},
			{"{{place|en|city/and/municipality/and/capital|p/Drenthe|c/Netherlands}}", "A city and municipality and capital of Drenthe, Netherlands"},
			{"{{place|en|town/and/commune/non-city capital|dept/Orne|c/France}}", "A town and commune, the capital of Orne department, France"},
			{"{{place|en|city/county/and/unitary authority|in southwest|cc/England}}", "A city, a county and unitary authority in southwest England"},
			{"{{place|en|a=A French-speaking|region|of northwestern|c/Italy|caplc=en:Aosta}}",
			"A French-speaking region of northwestern Italy. Capital and largest city: Aosta"},
			{"{{place|en|village|lbor/Croydon|co/Greater London}}", "A village in Croydon borough, Greater London"},
			{"{{place|en|place|c/US|named after Montana Territory}}", "A place in the United States, named after Montana Territory"},
			{"{{place|en|province|in|the Ethiopian Empire}}", "A province in the Ethiopian Empire"},
			{"{{place|en|island|in the Florida Keys|the largest significant cay in the archipelago}}",
			"An island in the Florida Keys the largest significant cay in the archipelago"}, // (sic!) no comma
			{"{{place|en|Virginia Beach|;|Lynnhaven section of Virginia Beach, VA}}", "A Virginia Beach; Lynnhaven section of Virginia Beach, VA"},
			{"{{place|en|townland|bar/Ormond Lower|co/Tipperary|c/Ireland}}", "A townland in Ormond Lower, County Tipperary, Ireland"},
			{"{{place|en|village|dist/West Berkshire|co/Berkshire|cc/England}}", "A village in West Berkshire district, Berkshire, England"},
			{"{{place|en|city|co/Emanuel County/and/Johnson County|s/Georgia|c/USA}}",
			"A city in Emanuel County/and/Johnson County, Georgia, United States"}, // (sic!)
			{"{{place|en|village|township/Verulam|city/Kawartha Lakes|county/Victoria|p/Ontario|c/Canada}}",
			"A village in Verulam, Kawartha Lakes, Victoria, Ontario, Canada"},
			{"{{place|en|mountain|region/Descartes Highlands|moon/Moon}}", "A mountain in Descartes Highlands, Moon"},
			{"{{place|mul|asteroid|region/Asteroid Belt|starsystem/Solar System|t1=Mohorovicic}}",
			"Mohorovicic (an asteroid in Asteroid Belt, Solar System)"},
			{"{{place|en|major river|in Scotland, flowing from|carea/South Lanarkshire|past|carea/North Lanarkshire|through|carea/Glasgow|and past|carea/Renfrewshire|and|carea/West Dunbartonshire|to the|place/Firth of Clyde}}",
			"A major river in Scotland, flowing from South Lanarkshire council area, past North Lanarkshire council area, through Glasgow council area, and past Renfrewshire council area and West Dunbartonshire council area, to the Firth of Clyde"},
			{"{{place|en|town|s/Louisiana|;|the parish seat|of|parish/Grant Parish|;|named for Schuyler Colfax}}",
			"A town in Louisiana; the parish seat of Grant Parish; named for Schuyler Colfax"},
			{"{{place|en|capital city|dept/Gironde|c/France|;|capital city|r:pref/Nouvelle-Aquitaine}}",
			"The capital city of Gironde department, France; capital city of the region of Nouvelle-Aquitaine"},
			{"{{place|en|emirate|c/United Arab Emirates|on the southern|gulf/Persian Gulf}}",
			"An emirate of the United Arab Emirates, on the southern Persian Gulf"},
			{"{{place|en|city/and/seaport|c/Israel|on the|sea/Mediterranean Sea}}", "A city and seaport in Israel, on the Mediterranean Sea"},
			{"{{place|en|The <<capital city>> of <<s/Texas>>, <<cc/USA>> and the <<county seat>> of <<co/Travis County>>|;|named for American empresario Stephen F. Austin}}",
			"The capital city of Texas, USA and the county seat of Travis County; named for American empresario Stephen F. Austin"},
			{"{{place|en|country|r/Central Europe|a member state of the European Union|official=Republic of Austria}}",
			"A country in Central Europe, a member state of the European Union. Official name: Republic of Austria"},
			{"{{place|en|city/shire town|co/Chittenden County|s/Vermont|;|the largest city in Vermont|;|perhaps named for Richard Boyle, 3rd Earl of Burlington, or the prominent Burling family of New York}}",
			"A city, the shire town of Chittenden County, Vermont; the largest city in Vermont; perhaps named for Richard Boyle, 3rd Earl of Burlington, or the prominent Burling family of New York"},
			{"{{place|en|river|in|England|, forming the boundary between|co/Derbyshire|and|co/Staffordshire}}",
			"A river in England, forming the boundary between Derbyshire and Staffordshire"},
			{"{{place|en|city/and/municipality|p/North Holland|c/Netherlands|;|capital city|c/Netherlands}}",
			"A city and municipality of North Holland, Netherlands; capital city of the Netherlands"},
			{"{{place|en|municipality/capital city|c/Brazil|;|state capital|s/Distrito Federal|c/Brazil}}",
			"A municipality, the capital city of Brazil; state capital of Distrito Federal, Brazil"},
			{"{{place|en|capital city|c/Switzerland|;|capital city|can/Bern}}", "The capital city of Switzerland; capital city of Bern canton"},
			{"{{place|en|city|in southern|c/Poland|;|capital|voivodeship/Silesian Voivodeship}}",
			"A city in southern Poland; capital of the Silesian Voivodeship"},
			{"{{place|en|capital city|dept/Bouches-du-Rhône|c/France|;|capital city|r:pref/Provence-Alpes-Côte d'Azur}}",
			"The capital city of Bouches-du-Rhône department, France; capital city of the region of Provence-Alpes-Côte d'Azur"},
			{"{{place|en|town/county seat|co/Tyrrell County||s/North Carolina}}", "A town, the county seat of Tyrrell County, North Carolina"},
			{"{{place|en|country| and the largest <<island>> in the <<r/Caribbean>>|caplc=Havana}}",
			"A country and the largest island in the Caribbean. Capital and largest city: Havana"},
			{"{{place|en|township|city:pref/Mandalay|div:suf/Mandalay|c/Burma|;, and|former capital|c/Burma}}",
			"A township in the city of Mandalay, Mandalay division, Burma, and former capital of Burma"},
			{"{{place|en|geographic region|region/Horn of Africa}}", "A geographic region in Horn of Africa"},
			{"{{place|en|village|council area/Fife|cc/Scotland}}", "A village in Fife council area, Scotland"},
			{"{{place|en|highlands|region/Southern France|c/France|t1=Massif Central}}", "Massif Central (a highlands in Southern France, France)"},
			{"{{place|en|settlement|isl/Saint Thomas|terr/United States Virgin Islands}}", "A settlement in Saint Thomas, United States Virgin Islands"},
			{"{{place|en|former headland|isl/Hong Kong Island|sar/Hong Kong}}", "A former headland in Hong Kong Island, Hong Kong"},
			{"{{place|en|development|s/Tarlac|the New Clark City}}", "A development in Tarlac, the New Clark City"},
			{"{{place|en|country|region/Oceania|comprising over 300 islands}}", "A country in Oceania, comprising over 300 islands"},
			{"{{place|en|city/county seat/(one of two)|co/St. Clair County|s/Alabama|c/USA}}",
			"A city, the county seat (one of two) of St. Clair County, Alabama, United States"},
			{"{{place|en|unincorporated community/census-designated place/county seat|co/Oscoda County|s/Michigan|c/USA}}",
			"An unincorporated community, a census-designated place, the county seat of Oscoda County, Michigan, United States"},
			{"{{place|en|mountain/plateau/town/and/locality|lgarea/City of Gold Coast|s/Queensland|c/Australia}}",
			"A mountain, a plateau, a town and locality in the City of Gold Coast, Queensland, Australia"},
			{"{{place|en|unincorporated community/census-designated place/county seat|co/Echols County|s/Georgia|c/USA}}",
			"An unincorporated community, a census-designated place, the county seat of Echols County, Georgia, United States"},
			{"{{place|en|modern municipality|runit/Xanthi|in the|adr:pref/Thrace|c/Greece}}",
			"A modern municipality of Xanthi, in the administrative region of Thrace, Greece"},
			{"{{place|en|city/administrative centre|obl/Chernivtsi|in the|r:suf/Bukovyna|c/Ukraine}}",
			"A city, the administrative centre of Chernivtsi Oblast, in the Bukovyna region, Ukraine"},
			{"{{place|en|coastal village|co/Ceredigion|cc/Wales|north of|vill/Borth}}", "A coastal village in Ceredigion, Wales, north of Borth"},
			{"{{place|en|small village|wcomm/Llanwddyn|co/Powys|cc/Wales|historically in|place/Montgomeryshire}}",
			"A small village in Llanwddyn community, Powys, Wales, historically in Montgomeryshire"},
			{"{{place|en|city|co/Tarrant County|s/Texas|c/USA|previously named|town/Bransford}}",
			"A city in Tarrant County, Texas, United States, previously named Bransford"},
			{"{{place|en|hamlet|par/Savernake|south of|town/Marlborough|co/Wiltshire|cc/England}}",
			"A hamlet in Savernake parish, south of Marlborough, Wiltshire, England"},
			{"{{place|en|CDP/and/town/county seat|co/Suffolk County|on|isl/Long Island|s/New York|c/USA}}",
			"A census-designated place and town, the county seat of Suffolk County, on Long Island, New York, United States"},
			{"{{place|en||village/and/civil parish|with a group parish council in|dist/West Lindsey|co/Lincolnshire|cc/England}}",
			"A village and civil parish with a group parish council in West Lindsey district, Lincolnshire, England"},
			{"{{place|en|municipal unit|of|the|municipality|of|municipality/Lezhë|in|the|county|of|county/Lezhë|c/Albania}}",
			"A municipal unit of the municipality of Lezhë in the county of Lezhë, Albania"},
			{"{{place|en|mountain|between|s/Alaska|c/USA|and|terr/Yukon|c/Canada|in|the|range/Saint Elias Mountains}}",
			"A mountain between Alaska, United States and Yukon, Canada in the Saint Elias Mountains"},
			{"{{place|en|suburb|of|city/London|in|lbor/Haringey|lbor/Camden|and|lbor/Islington|co/Greater London|cc/England}}",
			"A suburb of London in Haringey borough, Camden borough and Islington borough, Greater London, England"},
			{"{{place|en|unincorporated community|co/Athens County|s/Ohio|and a|state park|in|co/Morgan County|and|co/Athens County}}",
			"An unincorporated community in Athens County, Ohio, and a state park in Morgan County and Athens County"},
			{"{{place|en|unincorporated community|par/St. John the Baptish Parish||s/Louisiana}}",
			"An unincorporated community in St. John the Baptish Parish, Louisiana"},
			{"{{place|en|community|twp/Admaston/Bromley|co/Renfrew County|p/Ontario|c/Canada}}",
			"A community in Admaston/Bromley, Renfrew County, Ontario, Canada"},
			{"{{place|en|legendary city/ancient capital|kingdom:suf/Kosala}}", "A legendary city, the ancient capital of Kosala kingdom"},
			{"{{place|en|prefecture/capital city|c/Japan}}", "A prefecture, the capital city of Japan"},
			{"{{place|en|city/and/port|c/Denmark}}", "A city and port in Denmark"},
			{"{{place|en|town/and/commune|p:pref/Trapani|r/Sicily|c/Italy}}", "A town and commune of the province of Trapani, Sicily, Italy"},
			{"{{place|en|civil parish/and/small settlement|dist/Maldon|co/Essex|cc/England}}",
			"A civil parish and small settlement in Maldon district, Essex, England"},
			{"{{place|en|hamlet/and/civil parish|in|bor/Allerdale|co/Cumbria|cc/England}}",
			"A hamlet and civil parish in Allerdale borough, Cumbria, England"},
			{"{{place|en|a <<river>> downstate in <<s:Suf/New York>>}}", "a river downstate in New York State"},
			{"{{place|en|country|in northeastern|cont/Africa|on the|sea/Red Sea|, having <<capital/Asmara>> as its capital|modern=State of Eritrea (since the 1990s)}}",
			"A country in northeastern Africa, on the Red Sea, having Asmara as its capital; modern State of Eritrea (since the 1990s)"},
			{"", ""},
			{"", ""},
			{"", ""},
		};

		for (auto [key, value] : unittest)
		{
            bracketer b;
            b.proceed_template = [&](str s){ return templates_places_("unittest", "", s, result); };
            b.proceed(key);
            if (b.output != value)
				result.report(esc
					+ "\n" + key
					+ "\n" + value
					+ "\n" + b.output,
					"{{place}} unittest");
		}

        for (auto && [title, topic] : input)
        {
            static int64_t nn = 0; if (++nn % 100'000 == 0)
                logout("templates3", nn, input.cargo);

            for (auto & [header, forms, content] : topic)
            {
                auto t = title;
                auto h = header;

                for (auto & line : content)
                {
					bool place_complex = 
						line.contains("{{place|") and ((
						not line.starts_with("## {{")  and
						not line.starts_with ("# {{")) or (
						not line.ends_with     ("}}.") and
						not line.ends_with     ("}}")) or
						str(line.from(4)).contains("{{"));

					if (place_complex) result.report (esc + " " + title + "\n" + line, "{{place}} &&&");

                    bracketer b;
                    b.proceed_template = [&](str s){ return templates_places_(t, h, s, result); };
                    b.proceed(line);
                    line = b.output;

					if (place_complex) result.report (line, "{{place}} &&&");
                }
            }

            output.push(entry{
                std::move(title),
                std::move(topic)});
        }
    };
}
