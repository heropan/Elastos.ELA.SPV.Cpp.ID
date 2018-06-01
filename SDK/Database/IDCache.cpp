// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/filesystem/path.hpp>
#include <SDK/Common/Log.h>
#include "IDCache.h"

namespace Elastos {
	namespace SDK {

		IDCache::IDCache(const std::string &filename) {
			leveldb::Options opt;
			opt.create_if_missing = true;
//			opt.filter_policy = NewBloomFilterPolicy(10);
			leveldb::Status s = leveldb::DB::Open(opt, filename, &_db);
			if (!s.ok()) {
				Log::getLogger()->error("leveldb open file {} error: {}", filename, s.ToString());
				_db = nullptr;
			}
		}

		IDCache::IDCache() :
			IDCache("IDCache") {
		}

		IDCache::~IDCache() {
			if (_db) {
				delete _db;
				_db = nullptr;
			}
		}

		bool IDCache::Put(const std::string &id, const IDPathMap &map) {
			if (!isValid()) {
				return false;
			}

			nlohmann::json jsonToSave;
			std::string value;
			leveldb::Status s = _db->Get(leveldb::ReadOptions(), id, &value);
			if (!s.ok()) {
				if (!s.IsNotFound()) {
					Log::getLogger()->error("IDCache get id={} error: {}", id, s.ToString());
					return false;
				}
			} else {
				jsonToSave = nlohmann::json::parse(value);
			}

			for (IDPathMap::const_iterator iter = map.begin(); iter != map.end(); iter++) {
				jsonToSave[iter->first] = iter->second;
			}

			s = _db->Put(leveldb::WriteOptions(), id, jsonToSave.dump());
			if (!s.ok()) {
				Log::getLogger()->error("IDCache put id={} error: {}", id, s.ToString());
				return false;
			}

			return true;
		}

		bool IDCache::Put(const std::string &id, const std::string &path, const nlohmann::json &j) {
			if (!isValid()) {
				return false;
			}

			nlohmann::json jsonToSave;
			std::string value;
			leveldb::Status s = _db->Get(leveldb::ReadOptions(), id, &value);
			if (!s.ok()) {
				if (!s.IsNotFound()) {
					Log::getLogger()->error("IDCache get id={} error: {}", id, s.ToString());
					return false;
				}
			} else {
				jsonToSave = nlohmann::json::parse(value);
			}

			jsonToSave[path] = j;

			s = _db->Put(leveldb::WriteOptions(), id, jsonToSave.dump());
			if (!s.ok()) {
				Log::getLogger()->error("IDCache put id={} error: {}", id, s.ToString());
				return false;
			}

			return true;
		}

		bool IDCache::Get(const std::string &id, IDPathMap &map) const {
			if (!isValid()) {
				return false;
			}

			nlohmann::json jsonGet;
			std::string value;
			leveldb::Status s = _db->Get(leveldb::ReadOptions(), id, &value);
			if (!s.ok()) {
				Log::getLogger()->error("IDCache get id={} error: {}", id, s.ToString());
				return false;
			}

			jsonGet = nlohmann::json::parse(value);

			for (nlohmann::json::const_iterator it = jsonGet.cbegin(); it != jsonGet.cend(); ++it) {
				map[it.key()] = it.value();
			}

			return true;
		}

		bool IDCache::Get(const std::string &id, const std::string &path, nlohmann::json &j) const {
			if (!isValid()) {
				return false;
			}

			std::string value;
			leveldb::Status s = _db->Get(leveldb::ReadOptions(), id, &value);
			if (!s.ok()) {
				Log::getLogger()->error("IDCache get id={} error: {}", id, s.ToString());
				return false;
			}

			j = nlohmann::json::parse(value);

			return true;
		}

		bool IDCache::Delete(const std::string &id) {
			if (!isValid()) {
				return false;
			}

			leveldb::Status s = _db->Delete(leveldb::WriteOptions(), id);
			if (!s.ok() && !s.IsNotFound()) {
				Log::getLogger()->error("IDCache delete id={} error: {}", id, s.ToString());
				return false;
			}

			return true;
		}

		bool IDCache::Delete(const std::string &id, const std::string &path) {
			if (!isValid()) {
				return false;
			}

			nlohmann::json jsonToSave;
			std::string value;
			leveldb::Status s = _db->Get(leveldb::ReadOptions(), id, &value);
			if (!s.ok()) {
				if (s.IsNotFound()) {
					return true;
				}
				Log::getLogger()->error("IDCache get id={} error: {}", id, s.ToString());
				return false;
			}

			jsonToSave = nlohmann::json::parse(value);
			jsonToSave.erase(path);

			s = _db->Put(leveldb::WriteOptions(), id, jsonToSave.dump());
			if (!s.ok()) {
				Log::getLogger()->error("IDCache put id={} error: {}", id, s.ToString());
				return false;
			}

			return true;
		}

		bool IDCache::Move(const std::string &idFrom, const std::string &idTo) {
			if (!isValid()) {
				return false;
			}

			leveldb::WriteBatch batch;

			std::string value;
			leveldb::Status s = _db->Get(leveldb::ReadOptions(), idFrom, &value);
			if (!s.ok()) {
				Log::getLogger()->error("IDCache get id={} error: {}", idFrom, s.ToString());
				return false;
			}

			batch.Delete(idFrom);
			batch.Put(idTo, value);

			s = _db->Write(leveldb::WriteOptions(), &batch);
			if (!s.ok()) {
				Log::getLogger()->error("batch write error: {}", s.ToString());
				return false;
			}

			return true;
		}

		bool IDCache::Move(const std::string &idFrom, const std::string &pathFrom, const std::string &idTo, const std::string &pathTo) {
			if (!isValid()) {
				return false;
			}

			leveldb::WriteBatch batch;
			nlohmann::json jsonGet, jsonToSave, jsonToMove;

			std::string value;
			leveldb::Status s = _db->Get(leveldb::ReadOptions(), idFrom, &value);
			if (!s.ok()) {
				Log::getLogger()->error("IDCache get id={} error: {}", idFrom, s.ToString());
				return false;
			}

			jsonGet = nlohmann::json::parse(value);
			jsonToMove = jsonGet[pathFrom];

			jsonGet.erase(pathFrom);
			if (jsonGet.empty()) {
				batch.Delete(idFrom);
			} else {
				batch.Put(idFrom, jsonGet.dump());
			}

			s = _db->Get(leveldb::ReadOptions(), idTo, &value);
			if (!s.ok()) {
				if (!s.IsNotFound()) {
					Log::getLogger()->error("IDCache get id={} error: {}", idTo, s.ToString());
					return false;
				}
			} else {
				jsonToSave = nlohmann::json::parse(value);
			}

			jsonToSave[pathTo] = jsonToMove;

			batch.Put(idTo, jsonToSave.dump());

			s = _db->Write(leveldb::WriteOptions(), &batch);
			if (!s.ok()) {
				Log::getLogger()->error("batch write error: {}", s.ToString());
				return false;
			}

			return true;
		}

		bool IDCache::isValid() const {
			if (_db == nullptr) {
				Log::getLogger()->error("IDCache invalid; _db is nullptr");
				return false;
			}

			return true;
		}

		bool IDCache::DeleteAll() {
			return false;
		}

	}
}