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
		// +
		"capital",
		"capital and largest city",
		"community",
		"place", "places",
		"locale"
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
		"place", "places",
		"locale"
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
	};

	static array<str> placetypes_a = {
		"union territory",
		"unitary authority",
		"unitary district",
		"united township municipality",
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
		"prefecture", // +
		"subdivision",
		"tehsil",
		"Welsh community",
	};


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
					if (s2 == "") s2 = s1; else
					if (s1 == "dept") s2 += " department"; else
					if (s1 == "dist") s2 += " district"; else
					if (s1 == "riv:Suf") s2 += " River"; else
					if (s1 == "pen:suf") s2 += " peninsula"; else
					{}
					full.replace_by(s2);
				}
			};

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
				a.kind += " old"; if (body.contains("<<")) {
				a.kind += " new"; for (auto & s : a.unnamed) angles(s, a.kind); }

				auto map = [](const std::map<str,str> & m, const str & s) {
				auto it = m.find(s); return it != m.end() ? it->second : s;	};

				auto match = [](str s, str pattern) { return std::regex_match(s, std::regex(pattern)); };

				auto the = [match](str type, str name)
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

				str where = "";
				str kind1 = "";
				str kind2 = "";
				str kind3 = "";

				while (a.unnamed.size() >= 3)
				{
					a.kind += " 3";
					auto a3 = a.unnamed.back().split_by("/");
					if (a3.size() == 2)
					{
						str p1 = map(placetype_aliases, a3[0]);
						str p2 = map(placename_aliases, a3[1]);
						if (p1 == "city" and
							p2 == "New York")
							p2 = "New York City";

						if (p1 == "country"
						or  p1 == "constituent country"
						or  p1 == "county"
						or  p1 == "city"
						or  p1 == "council area"
						or  p1 == "province"
						or  p1 == "region"
						or  p1 == "state"
						or  false) {;}
						else
						if (placetypes.contains(p1))
							kind1 = "a"; else
							kind2 = "b";

						a.unnamed.back() = p2;
					}
					else kind3 = "c";

					str
					where_ =
						a[1].starts_with("in ") or
						a[1].starts_with("of ") ?
						" " : ", ";
					where_ += a.unnamed.back();
					where = where_ + where;
					a.unnamed.truncate();
				}

				str def = a.acquire("def");
				if (def != "")
				{
					a.kind += " def"; output = def;
				}
				else
				if (a.unnamed.size() == 2)
				{
					a.kind += " 2";
					auto a0 = a[0].split_by("/");
					auto a1 = a[1].split_by("/");

					if (a0.size() >= 5 and
					   (a0[1] == "and" or a0[1] == "or") and
					   (a0[3] == "and" or a0[3] == "or")) {
						a0[0] = map(placetype_aliases, a0[0]);
						a0[2] = map(placetype_aliases, a0[2]);
						a0[4] = map(placetype_aliases, a0[4]);
						a0[0] = a0[0] + " " + a0[1] + " " + a0[2] + " " + a0[3] + " " + a0[4];
						a0.erase(1);
						a0.erase(1);
						a0.erase(1);
						a0.erase(1);
						a.kind += "!!";
					}
					if (a0.size() >= 4 and
					   (a0[2] == "and" or a0[2] == "or")) {
						a0[0] = map(placetype_aliases, a0[0]);
						a0[1] = map(placetype_aliases, a0[1]);
						a0[3] = map(placetype_aliases, a0[3]);
						a0[0] += a0[1].starts_with("(") ? " " : ", ";
						a0[0] += placetypes.contains(a0[1]) ? str("aeiou").contains(a0[1][0]) ? "an " : "a " : "";
						a0[0] += a0[1] + " " + a0[2] + " " + a0[3];
						a0.erase(1);
						a0.erase(1);
						a0.erase(1);
						a.kind += "!!";
					}
					if (a0.size() >= 3 and
					   (a0[1] == "and" or a0[1] == "or")) {
						a0[0] = map(placetype_aliases, a0[0]);
						a0[2] = map(placetype_aliases, a0[2]);
						a0[0] = a0[0] + " " + a0[1] + " " + a0[2];
						a0.erase(1);
						a0.erase(1);
						a.kind += "!";
					}
					if (a0.size() == 2){
						a0[0] = map(placetype_aliases, a0[0]);
						a0[1] = map(placetype_aliases, a0[1]);
						if (placetypes_a  .contains(a0[1])) a0[1] = "a "   + a0[1];
						if (placetypes_the.contains(a0[1])) a0[1] = "the " + a0[1];
						a0[0] += a0[1].starts_with("(") ? " " : ", ";
						a0[0] += a0[1];
						a0.erase(1);
						a.kind += "!";
					}

					if (a0.size() == 1 and a1.size() == 1)
					{
						output = a0[0]; if (not a1[0].starts_with(","))
						output += " ";
						output += a1[0];
						a.kind += "1";
					}
					else
					if (a0.size() == 1 and a1.size() == 2)
					{
						a0[0] = map(placetype_aliases, a0[0]);
						str p0 = a0[0];
						str p1 = a1[0];
						str p2 = a1[1];

						p0 = p0.split_by(" (").front();
						auto pp = p0.split_by(" "); int n = pp.size();
						str pp4 = n < 4 ? "" : pp[n-4] + " " + pp[n-3] + " " + pp[n-2] + " " + pp[n-1];
						str pp3 = n < 3 ? "" : pp[n-3] + " " + pp[n-2] + " " + pp[n-1];
						str pp2 = n < 2 ? "" : pp[n-2] + " " + pp[n-1];
						str pp1 = n < 1 ? "" : pp[n-1];
						p0 = placetypes.contains(pp4) ? pp4:
							 placetypes.contains(pp3) ? pp3:
							 placetypes.contains(pp2) ? pp2:
							 placetypes.contains(pp1) ? pp1: "";

						if (p0 == "borough") seat = "Borough";

						str suf;
						if (p1.ends_with(":suf") or
							p1.ends_with(":Suf"))
							p1.split_by(":", p1, suf);

						p1 = map(placetype_aliases, p1);
						p2 = map(placename_aliases, p2);
						if (p1 == "city" and
							p2 == "New York")
							p2 = "New York City";

						if (not placetypes.contains(p0)) a.kind += "!";
						if (placetypes.contains(p2) or not
							placetypes.contains(p1))
							a.kind += "!";

						str article = the(p1, p2) ? "the " : "";
						if (article != "") a.kind += "!";

						if (placetypes_suf.contains(p1)) suf = "suf";
						if (placetypes_Suf.contains(p1)) suf = "Suf";
						if (suf == "suf") suf = p1;
						if (suf == "Suf") { suf = p1; suf[0] = str::ascii_toupper(suf[0]); }
						if (suf == "London borough") suf = "borough";
						if (suf == "Welsh community") suf = "community";
						if (suf == "local government district") suf = "district";
						if (suf != "") p2 += " " + suf;

						output  = a0[0];
						output += placetypes_of.contains(p0) ? " of " : " in ";
						output += article;
						output += p2;

						a.kind += kind1;
						a.kind += kind2;
						a.kind += kind3;
					}
					else a.kind += " quest";
				}
				else
				{
					//output = str(a.unnamed, " ");
					a.kind += " quest";
				}

				str A = a.acquire("a");

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
					output = A + " " + output + where;
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
            q = a.acquire("t1"          ); if (q != "") { output  = q + " (" + output + ")";            }
            q = a.acquire("t"           ); if (q != "") { output  = q + " (" + output + ")";            }

            if (not a.opt.empty()) a.kind += " opt";

			bool fix = false; str prefix = output;
			fix |= 0 < output.replace_all(" en:", " ");
			fix |= 0 < output.replace_all("..", ".");
			fix |= 0 < output.replace_all(".;", ";");
			fix |= 0 < output.replace_all(";;", ";");
			if (fix) result.report (esc + " " + title
				+ "\n" + report
				+ "\n" + prefix
				+ "\n" + output,
				"fix");

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
			"A city, the county seat of Whitman County, Washington; named for Schuyler Colfax"},
			{"", ""},
			{"", ""},
			{"", ""},
			{"", ""},
			{"", ""},
			{"", ""},
			{"", ""},
			{"", ""},
			{"", ""},
			{"", ""},
			{"", ""},
			{"", ""},
		};

		for (auto [key, value] : unittest)
		{
            bracketer b;
            b.proceed_template = [&](str s){ return templates_places_("", "", s, result); };
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
