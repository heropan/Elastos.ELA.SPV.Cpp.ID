// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#define CATCH_CONFIG_MAIN

#include <sstream>
#include <boost/filesystem.hpp>

#include "Log.h"
#include "IdCache.h"
#include "catch.hpp"

using namespace Elastos::SDK;

const std::string IdCachePath = "IdCacheTestDB";

TEST_CASE( "IdCache test", "[IdCache]" )
{

	nlohmann::json id1Json = {
		{"path1", {
					  {"101", {{"datahash", "datahash1"}, {"proof", "hello proof1"}, {"sign", "hello sign1"}}},
					  {"102", {{"datahash", "datahash2"}, {"proof", "hello proof2"}, {"sign", "hello sign2"}}},
					  {"103", {{"datahash", "datahash3"}, {"proof", "hello proof3"}, {"sign", "hello sign3"}}}
				  }},
		{"path2", {
					  {"104", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
					  {"105", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
					  {"106", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
				  }},
		{"path3", {
					  {"107", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
					  {"108", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
					  {"109", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
				  }}
	};

	nlohmann::json id2Json = {
		{"path1", {
					  {"111", {{"datahash", "datahash1"}, {"proof", "hello proof1"}, {"sign", "hello sign1"}}},
					  {"112", {{"datahash", "datahash2"}, {"proof", "hello proof2"}, {"sign", "hello sign2"}}},
					  {"113", {{"datahash", "datahash3"}, {"proof", "hello proof3"}, {"sign", "hello sign3"}}}
				  }},
		{"path2", {
					  {"114", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
					  {"115", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
					  {"116", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
				  }},
		{"path3", {
					  {"117", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
					  {"118", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
					  {"119", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
				  }}
	};

	nlohmann::json id3Json = {
		{"path1", {
					  {"121", {{"datahash", "datahash1"}, {"proof", "hello proof1"}, {"sign", "hello sign1"}}},
					  {"132", {{"datahash", "datahash2"}, {"proof", "hello proof2"}, {"sign", "hello sign2"}}},
					  {"103", {{"datahash", "datahash3"}, {"proof", "hello proof3"}, {"sign", "hello sign3"}}}
				  }},
		{"path2", {
					  {"104", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
					  {"105", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
					  {"106", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
				  }},
		{"path3", {
					  {"107", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
					  {"108", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
					  {"109", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
				  }}
	};

	SECTION("Prepare to test") {
		if (boost::filesystem::exists(IdCachePath) && boost::filesystem::is_directory(IdCachePath)) {
			boost::filesystem::remove_all(IdCachePath);
		}
	}

	SECTION("IdCache test by id") {
		SECTION("IdCache put by id") {
			IdCache idCache(IdCachePath);

			REQUIRE(idCache.Put("id1", id1Json));
			REQUIRE(idCache.Put("id2", id2Json));
			REQUIRE(idCache.Put("id3", id3Json));
		}

		SECTION("IdCache get by id") {
			IdCache idCache(IdCachePath);
			nlohmann::json jsonGet;

			jsonGet = idCache.Get("idNotFound");
			REQUIRE(jsonGet.empty());

			jsonGet = idCache.Get("id1");
			REQUIRE(jsonGet.dump() == id1Json.dump());

			jsonGet = idCache.Get("id2");
			REQUIRE(jsonGet.dump() == id2Json.dump());

			jsonGet = idCache.Get("id3");
			REQUIRE(jsonGet.dump() == id3Json.dump());
		}

		SECTION("IdCache delete by id") {
			IdCache idCache(IdCachePath);
			nlohmann::json jsonGet;

			REQUIRE(idCache.Delete("id1"));
			jsonGet = idCache.Get("id1");
			REQUIRE(jsonGet.empty());

			REQUIRE(idCache.Delete("id2"));
			jsonGet = idCache.Get("id2");
			REQUIRE(jsonGet.empty());

			REQUIRE(idCache.Delete("id3"));
			jsonGet = idCache.Get("id3");
			REQUIRE(jsonGet.empty());
		}
	}

	SECTION("IdCache test by id and path") {
		SECTION("IdCache put by id and path") {
			IdCache idCache(IdCachePath);
			nlohmann::json jsonGet;

			for (nlohmann::json::iterator it = id1Json.begin(); it != id1Json.end(); it++) {
				std::string tagPath = it.key();
				REQUIRE(idCache.Put("id1", tagPath, it.value()));
			}

			for (nlohmann::json::iterator it = id2Json.begin(); it != id2Json.end(); it++) {
				std::string tagPath = it.key();
				REQUIRE(idCache.Put("id2", tagPath, it.value()));
			}

			for (nlohmann::json::iterator it = id3Json.begin(); it != id3Json.end(); it++) {
				std::string tagPath = it.key();
				REQUIRE(idCache.Put("id3", tagPath, it.value()));
			}
		}

		SECTION("IdCache get by id and path") {
			IdCache idCache(IdCachePath);
			nlohmann::json getJson;

			for (nlohmann::json::iterator it = id1Json.begin(); it != id1Json.end(); it++) {
				std::string tagPath = it.key();
				nlohmann::json pathJson = it.value();
				nlohmann::json jsonGet = idCache.Get("id1", tagPath);
				REQUIRE(!jsonGet.empty());
				REQUIRE(jsonGet.dump() == pathJson.dump());
			}

			for (nlohmann::json::iterator it = id2Json.begin(); it != id2Json.end(); it++) {
				std::string tagPath = it.key();
				nlohmann::json pathJson = it.value();
				nlohmann::json jsonGet = idCache.Get("id2", tagPath);
				REQUIRE(!jsonGet.empty());
				REQUIRE(jsonGet.dump() == pathJson.dump());
			}

			for (nlohmann::json::iterator it = id3Json.begin(); it != id3Json.end(); it++) {
				std::string tagPath = it.key();
				nlohmann::json pathJson = it.value();
				nlohmann::json jsonGet = idCache.Get("id3", tagPath);
				REQUIRE(!jsonGet.empty());
				REQUIRE(jsonGet.dump() == pathJson.dump());
			}
		}

		SECTION("IdCache delete by id and path") {
			IdCache idCache(IdCachePath);
			nlohmann::json getJson;

			for (nlohmann::json::iterator it = id1Json.begin(); it != id1Json.end(); it++) {
				std::string tagPath = it.key();
				REQUIRE(idCache.Delete("id1", tagPath));
				getJson = idCache.Get("id1", tagPath);
				REQUIRE(getJson.empty());
			}

			for (nlohmann::json::iterator it = id2Json.begin(); it != id2Json.end(); it++) {
				std::string tagPath = it.key();
				REQUIRE(idCache.Delete("id2", tagPath));
				getJson = idCache.Get("id2", tagPath);
				REQUIRE(getJson.empty());
			}

			for (nlohmann::json::iterator it = id3Json.begin(); it != id3Json.end(); it++) {
				std::string tagPath = it.key();
				REQUIRE(idCache.Delete("id3", tagPath));
				getJson = idCache.Get("id3", tagPath);
				REQUIRE(getJson.empty());
			}
		}
	}

	SECTION("IdCache test by id and path and blockHeight") {
		SECTION("IdCache put by id and path and blockHeight") {
			IdCache idCache(IdCachePath);

			for (nlohmann::json::iterator it = id1Json.begin(); it != id1Json.end(); it++) {
				std::string tagPath = it.key();
				nlohmann::json pathJson = it.value();

				for (nlohmann::json::iterator iter = pathJson.begin(); iter != pathJson.end(); iter++) {
					uint32_t blockHeight = std::stoi(iter.key(), nullptr, 10);
					REQUIRE(idCache.Put("id1", tagPath, blockHeight, iter.value()));
				}
			}

			for (nlohmann::json::iterator it = id2Json.begin(); it != id2Json.end(); it++) {
				std::string tagPath = it.key();
				nlohmann::json pathJson = it.value();

				for (nlohmann::json::iterator iter = pathJson.begin(); iter != pathJson.end(); iter++) {
					uint32_t blockHeight = std::stoi(iter.key(), nullptr, 10);
					REQUIRE(idCache.Put("id2", tagPath, blockHeight, iter.value()));
				}
			}

			for (nlohmann::json::iterator it = id3Json.begin(); it != id3Json.end(); it++) {
				std::string tagPath = it.key();
				nlohmann::json pathJson = it.value();

				for (nlohmann::json::iterator iter = pathJson.begin(); iter != pathJson.end(); iter++) {
					uint32_t blockHeight = std::stoi(iter.key(), nullptr, 10);
					REQUIRE(idCache.Put("id3", tagPath, blockHeight, iter.value()));
				}
			}
		}

		SECTION("IdCache get by id and path and blockHeight") {
			IdCache idCache(IdCachePath);

			for (nlohmann::json::iterator it = id1Json.begin(); it != id1Json.end(); it++) {
				std::string tagPath = it.key();
				nlohmann::json pathJson = it.value();

				for (nlohmann::json::iterator iter = pathJson.begin(); iter != pathJson.end(); iter++) {
					uint32_t blockHeight = std::stoi(iter.key(), nullptr, 10);
					nlohmann::json blockHeightJson = iter.value();
					nlohmann::json jsonGet = idCache.Get("id1", tagPath, blockHeight);
					REQUIRE(blockHeightJson.dump() == jsonGet.dump());
				}
			}

			for (nlohmann::json::iterator it = id2Json.begin(); it != id2Json.end(); it++) {
				std::string tagPath = it.key();
				nlohmann::json pathJson = it.value();

				for (nlohmann::json::iterator iter = pathJson.begin(); iter != pathJson.end(); iter++) {
					uint32_t blockHeight = (uint32_t)std::stoi(iter.key(), nullptr, 10);
					nlohmann::json blockHeightJson = iter.value();
					nlohmann::json jsonGet = idCache.Get("id2", tagPath, blockHeight);
					REQUIRE(blockHeightJson.dump() == jsonGet.dump());
				}
			}

			for (nlohmann::json::iterator it = id3Json.begin(); it != id3Json.end(); it++) {
				std::string tagPath = it.key();
				nlohmann::json pathJson = it.value();

				for (nlohmann::json::iterator iter = pathJson.begin(); iter != pathJson.end(); iter++) {
					uint32_t blockHeight = (uint32_t)std::stoi(iter.key(), nullptr, 10);
					nlohmann::json blockHeightJson = iter.value();
					nlohmann::json jsonGet = idCache.Get("id3", tagPath, blockHeight);
					REQUIRE(blockHeightJson.dump() == jsonGet.dump());
				}
			}
		}

		SECTION("IdCache delete by id and path and blockHeight") {
			IdCache idCache(IdCachePath);

			for (nlohmann::json::iterator it = id1Json.begin(); it != id1Json.end(); it++) {
				std::string tagPath = it.key();
				nlohmann::json pathJson = it.value();

				for (nlohmann::json::iterator iter = pathJson.begin(); iter != pathJson.end(); iter++) {
					uint32_t blockHeight = std::stoi(iter.key(), nullptr, 10);

					REQUIRE(idCache.Delete("id1", tagPath, blockHeight));
					nlohmann::json jsonGet = idCache.Get("id1", tagPath, blockHeight);
					REQUIRE(jsonGet.empty());
				}
			}

			for (nlohmann::json::iterator it = id2Json.begin(); it != id2Json.end(); it++) {
				std::string tagPath = it.key();
				nlohmann::json pathJson = it.value();

				for (nlohmann::json::iterator iter = pathJson.begin(); iter != pathJson.end(); iter++) {
					uint32_t blockHeight = std::stoi(iter.key(), nullptr, 10);

					REQUIRE(idCache.Delete("id2", tagPath, blockHeight));
					nlohmann::json jsonGet = idCache.Get("id2", tagPath, blockHeight);
					REQUIRE(jsonGet.empty());
				}
			}

			for (nlohmann::json::iterator it = id3Json.begin(); it != id3Json.end(); it++) {
				std::string tagPath = it.key();
				nlohmann::json pathJson = it.value();

				for (nlohmann::json::iterator iter = pathJson.begin(); iter != pathJson.end(); iter++) {
					uint32_t blockHeight = std::stoi(iter.key(), nullptr, 10);

					REQUIRE(idCache.Delete("id3", tagPath, blockHeight));
					nlohmann::json jsonGet = idCache.Get("id3", tagPath, blockHeight);
					REQUIRE(jsonGet.empty());
				}
			}
		}
	}

	SECTION("IdCache delete all") {
	}

}
