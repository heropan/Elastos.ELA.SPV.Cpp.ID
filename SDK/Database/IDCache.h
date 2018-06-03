// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_IDCACHE_H
#define __ELASTOS_SDK_IDCACHE_H

#include <map>
#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <nlohmann/json.hpp>

namespace Elastos {
	namespace SDK {

		typedef std::map<std::string, nlohmann::json> IDPathMap;

		class IDCache {
		public:
			explicit IDCache(const std::string &filename);
			IDCache();
			~IDCache();

			bool Put(const std::string &id, const IDPathMap &map);
			bool Put(const std::string &id, const std::string &path, const nlohmann::json &j);

			bool Get(const std::string &id, IDPathMap &map) const;
			bool Get(const std::string &id, const std::string &path, nlohmann::json &j) const;

			bool Delete(const std::string &id);
			bool Delete(const std::string &id, const std::string &path);

			bool Move(const std::string &idFrom, const std::string &idTo);
			bool Move(const std::string &idFrom, const std::string &pathFrom, const std::string &idTo, const std::string &pathTo);

			bool DeleteAll();
		private:
			bool isValid() const;

		private:
			leveldb::DB* _db;
		};

	}
}

#endif //__ELASTOS_SDK_IDCACHE_H
