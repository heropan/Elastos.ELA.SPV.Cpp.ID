// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#define CATCH_CONFIG_MAIN

#include <sstream>
#include <boost/filesystem.hpp>

#include "Log.h"
#include "IDCache.h"
#include "catch.hpp"

using namespace Elastos::SDK;

const std::string IDCachePath = "IDCacheTestDB";

const std::string tagKeyIDPrefix   = "id";
const std::string tagKeyPathPrefix = "path";

const std::string tagDataHash    = "DataHash";
const std::string tagProof       = "Proof";
const std::string tagSign        = "Sign";
const std::string tagBlockHeight = "BlockHeight";

const int howManyID = 3;
const int howManyPath = 4;

TEST_CASE( "IDCache test", "[IDCache]" )
{
	SECTION("Prepare to test") {
		if (boost::filesystem::exists(IDCachePath) && boost::filesystem::is_directory(IDCachePath)) {
			boost::filesystem::remove_all(IDCachePath);
		}
	}

	SECTION("IDCache put map indexed by id") {
#if 0
		put some json like this:
			"id1":[
				{"path1": {"DataHash": "DataHash1", "Proof": "hello proof1", "Sign": "hello sign1", "BlockHeight": 1}},
				{"path2": {"DataHash": "DataHash2", "Proof": "hello proof2", "Sign": "hello sign2", "BlockHeight": 2}},
				{"path3": {"DataHash": "DataHash3", "Proof": "hello proof3", "Sign": "hello sign3", "BlockHeight": 3}}
				]

			"id2":[
				{"path1": {"DataHash": "DataHash4", "Proof": "hello proof4", "Sign": "hello sign7", "BlockHeight": 4}},
				{"path2": {"DataHash": "DataHash5", "Proof": "hello proof5", "Sign": "hello sign5", "BlockHeight": 5}},
				{"path3": {"DataHash": "DataHash6", "Proof": "hello proof6", "Sign": "hello sign6", "BlockHeight": 6}}
				]

			"id3":[
				{"path1": {"DataHash": "DataHash7", "Proof": "hello proof7", "Sign": "hello sign7", "BlockHeight": 7}},
				{"path2": {"DataHash": "DataHash8", "Proof": "hello proof8", "Sign": "hello sign8", "BlockHeight": 8}},
				{"path3": {"DataHash": "DataHash9", "Proof": "hello proof9", "Sign": "hello sign9", "BlockHeight": 9}}
				]
#endif
		IDCache idCache(IDCachePath);

		IDPathMap pathMap;
		std::ostringstream ss;
		for (int id = 1; id <= howManyID; ++id) {
			pathMap.clear();
			for (int path = 1; path <= howManyPath; ++path) {
				nlohmann::json putJson;

				ss.str("");
				ss << ((id - 1) * howManyPath + path);
				putJson[tagDataHash]    = tagDataHash + ss.str();
				putJson[tagProof]       = "hello proof" + ss.str();
				putJson[tagSign]        = "hello sign" + ss.str();
				putJson[tagBlockHeight] = (id - 1) * howManyPath + path;

				ss.str("");
				ss << path;
				pathMap[tagKeyPathPrefix + ss.str()] = putJson;
			}
			ss.str("");
			ss << id;
			REQUIRE(idCache.Put(tagKeyIDPrefix + ss.str(), pathMap));
		}
	}

	SECTION("IDCache get map indexed by id") {
		IDCache idCache(IDCachePath);
		IDPathMap pathMap;
		std::ostringstream ss;

		for (int id = 1; id <= howManyID; ++id) {
			pathMap.clear();
			ss.str("");
			ss << id;
			REQUIRE(idCache.Get(tagKeyIDPrefix + ss.str(), pathMap));
			REQUIRE(howManyPath == pathMap.size());
//			Log::getLogger()->info("{}", tagKeyIDPrefix + ss.str());
			for (int path = 1; path <= howManyPath; ++path) {
				ss.str("");
				ss << path;
				bool found = pathMap.find(tagKeyPathPrefix + ss.str()) != pathMap.end();
				REQUIRE(found);
				nlohmann::json getJson;
				getJson = pathMap[tagKeyPathPrefix + ss.str()];
//				Log::getLogger()->info("\"{}\": {}", tagKeyPathPrefix + ss.str(), getJson.dump());

				ss.str("");
				ss << ((id - 1) * howManyPath + path);
				REQUIRE(getJson[tagDataHash].get<std::string>() == tagDataHash + ss.str());
				REQUIRE(getJson[tagProof].get<std::string>() == "hello proof" + ss.str());
				REQUIRE(getJson[tagSign].get<std::string>() == "hello sign" + ss.str());
				REQUIRE(getJson[tagBlockHeight].get<uint32_t>() == ((id - 1) * howManyPath + path));
			}
		}

		REQUIRE(!idCache.Get("idNotFound", pathMap));
	}

	SECTION("IDCache put a json indexed by id and path") {
		IDCache idCache(IDCachePath);

		nlohmann::json putJson;
		putJson[tagDataHash]    = "DataHash+1";
		putJson[tagProof]       = "Proof+1";
		putJson[tagSign]        = "Sign+1";
		putJson[tagBlockHeight] = 110;
		REQUIRE(idCache.Put("id1", "path4", putJson));

		putJson[tagDataHash]    = "DataHash+2";
		putJson[tagProof]       = "Proof+2";
		putJson[tagSign]        = "Sign+2";
		putJson[tagBlockHeight] = 120;
		REQUIRE(idCache.Put("id2", "path5", putJson));

		putJson[tagDataHash]    = "DataHash+3";
		putJson[tagProof]       = "Proof+3";
		putJson[tagSign]        = "Sign+3";
		putJson[tagBlockHeight] = 130;
		REQUIRE(idCache.Put("id3", "path2", putJson));
	}

	SECTION("IDCache get a json indexed by id and path") {
		IDCache idCache(IDCachePath);
		nlohmann::json getJson;

		REQUIRE(idCache.Get("id1", "path4", getJson));
//		Log::getLogger()->info("json: {}", getJson.dump());
		REQUIRE(getJson[tagDataHash].get<std::string>() == "DataHash+1");
		REQUIRE(getJson[tagProof].get<std::string>() == "Proof+1");
		REQUIRE(getJson[tagSign].get<std::string>() == "Sign+1");
		REQUIRE(getJson[tagBlockHeight].get<uint32_t>() == 110);
		REQUIRE(!idCache.Get("id1", "pathNotFound", getJson));

		REQUIRE(idCache.Get("id2", "path5", getJson));
		REQUIRE(getJson[tagDataHash].get<std::string>() == "DataHash+2");
		REQUIRE(getJson[tagProof].get<std::string>() == "Proof+2");
		REQUIRE(getJson[tagSign].get<std::string>() == "Sign+2");
		REQUIRE(getJson[tagBlockHeight].get<uint32_t>() == 120);
		REQUIRE(!idCache.Get("id2", "pathNotFound", getJson));

		REQUIRE(idCache.Get("id3", "path2", getJson));
		REQUIRE(getJson[tagDataHash].get<std::string>() == "DataHash+3");
		REQUIRE(getJson[tagProof].get<std::string>() == "Proof+3");
		REQUIRE(getJson[tagSign].get<std::string>() == "Sign+3");
		REQUIRE(getJson[tagBlockHeight].get<uint32_t>() == 130);
		REQUIRE(!idCache.Get("id3", "pathNotFound", getJson));
	}

	SECTION("IDCache move a josn indexed by id and path") {
		IDCache idCache(IDCachePath);

		IDPathMap pathMapFrom, pathMapTo, pathMapVerify;
		nlohmann::json jsonMoved, jsonTmp;
		REQUIRE(idCache.Get("id1", pathMapFrom));
		jsonMoved = pathMapFrom["path1"];
		REQUIRE(idCache.Get("id2", pathMapTo));
		REQUIRE(idCache.Move("id1", "path1", "id2", "path1"));
		for (IDPathMap::iterator it = pathMapFrom.begin(); it != pathMapFrom.end(); it++) {
			jsonTmp.clear();
			if (it->first == "path1") {
				REQUIRE(!idCache.Get("id1", it->first, jsonTmp));
			} else {
				REQUIRE(idCache.Get("id1", it->first, jsonTmp));
				REQUIRE(jsonTmp == it->second);
			}
		}

		for (IDPathMap::iterator it = pathMapTo.begin(); it != pathMapTo.end(); it++) {
			jsonTmp.clear();
			REQUIRE(idCache.Get("id2", it->first, jsonTmp));
			if (it->first == "path1") {
				REQUIRE(jsonTmp == jsonMoved);
			} else {
				REQUIRE(pathMapTo[it->first] == jsonTmp);
			}
		}
	}

	SECTION("IDCache delete a json indexed by id and path") {
		IDCache idCache(IDCachePath);
		IDPathMap pathMap;
		nlohmann::json getJson;

		REQUIRE(idCache.Get("id1", pathMap));
		REQUIRE(idCache.Delete("id1", "path1"));
		for (IDPathMap::iterator it = pathMap.begin(); it != pathMap.end(); it++) {
			if (it->first == "path1") {
				REQUIRE(!idCache.Get("id1", it->first, getJson));
			} else {
				REQUIRE(idCache.Get("id1", it->first, getJson));
				REQUIRE(it->second == getJson);
			}
		}

		pathMap.clear();
		getJson.clear();
		REQUIRE(idCache.Get("id2", pathMap));
		REQUIRE(idCache.Delete("id2", "path3"));
		for (IDPathMap::iterator it = pathMap.begin(); it != pathMap.end(); it++) {
			if (it->first == "path3") {
				REQUIRE(!idCache.Get("id2", it->first, getJson));
			} else {
				REQUIRE(idCache.Get("id2", it->first, getJson));
				REQUIRE(it->second == getJson);
			}
		}

		pathMap.clear();
		getJson.clear();
		REQUIRE(idCache.Get("id3", pathMap));
		REQUIRE(idCache.Delete("id3", "pathNotFound"));
		for (IDPathMap::iterator it = pathMap.begin(); it != pathMap.end(); it++) {
			if (it->first == "pathNotFound") {
				REQUIRE(!idCache.Get("id3", it->first, getJson));
			} else {
				REQUIRE(idCache.Get("id3", it->first, getJson));
				REQUIRE(it->second == getJson);
			}
		}
	}

	SECTION("IDCache move a map indexed by id") {
		IDCache idCache(IDCachePath);
		IDPathMap pathMapFrom, pathMapTo, pathMaptmp;

		REQUIRE(idCache.Get("id1", pathMapFrom));
		REQUIRE(idCache.Move("id1", "id2"));
		REQUIRE(!idCache.Get("id1", pathMaptmp));
		REQUIRE(idCache.Get("id2", pathMapTo));
		REQUIRE(pathMapFrom.size() == pathMapTo.size());
		for (IDPathMap::iterator it = pathMapFrom.begin(); it != pathMapFrom.end(); it++) {
			REQUIRE(pathMapFrom[it->first] == pathMapTo[it->first]);
		}
	}

	SECTION("IDCache delete a map indexed by id") {
		IDCache idCache(IDCachePath);
		IDPathMap pathMap;

		REQUIRE(idCache.Delete("id2"));
		REQUIRE(!idCache.Get("id2", pathMap));
		REQUIRE(idCache.Delete("id2"));
	}

	SECTION("IDCache delete all") {
		IDCache idCache(IDCachePath);
		IDPathMap pathMap;

		REQUIRE(idCache.DeleteAll());
		REQUIRE(!idCache.Get("id1", pathMap));
		REQUIRE(!idCache.Get("id2", pathMap));
		REQUIRE(!idCache.Get("id3", pathMap));
		REQUIRE(!idCache.Get("id4", pathMap));
		REQUIRE(!idCache.Get("id5", pathMap));
	}

}