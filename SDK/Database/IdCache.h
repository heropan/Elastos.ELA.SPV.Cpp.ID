// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_IDCACHE_H
#define __ELASTOS_SDK_IDCACHE_H

#include <map>
#include <boost/filesystem/path.hpp>
#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <nlohmann/json.hpp>

namespace Elastos {
	namespace SDK {

		class IdCache {
		public:
			explicit IdCache(const boost::filesystem::path &path);
			IdCache();
			~IdCache();

			bool Put(const std::string &id, const nlohmann::json &idJson);
			bool Put(const std::string &id, const std::string &path, const nlohmann::json &pathJson);
			bool Put(const std::string &id, const std::string &path, uint32_t blockHeight, const nlohmann::json &blockHeighJson);

			nlohmann::json Get(const std::string &id) const;
			nlohmann::json Get(const std::string &id, const std::string &path) const;
			nlohmann::json Get(const std::string &id, const std::string &path, uint32_t blockHeight) const;

			bool Delete(const std::string &id);
			bool Delete(const std::string &id, const std::string &path);
			bool Delete(const std::string &id, const std::string &path, uint32_t blockHeight);

			bool DeleteAll();

			bool Initialized() const;

		private:
			leveldb::DB* _db;
		};

	}
}

#endif //__ELASTOS_SDK_IDCACHE_H
