// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Core/BRAddress.h"

#include "IdManager.h"
#include "SDK/Wrapper/Key.h"
#include "SDK/Common/Utils.h"

namespace Elastos {
	namespace SDK {

		IdManager::IdManager() {

		}

		IdManager::~IdManager() {

		}

		nlohmann::json IdManager::GenerateId(std::string &id, std::string &privateKey) {
			Key key;
			UInt128 entropy = Utils::generateRandomSeed();

			CMBlock seedByte;
			seedByte.SetMemFixed(entropy.u8, sizeof(entropy));
			CMBlock privKey = Key::getAuthPrivKeyForAPI(seedByte);

			char *data = new char[privKey.GetSize()];
			memcpy(data, privKey, privKey.GetSize());
			std::string ret(data, privKey.GetSize());
			privateKey = ret;

			key.setPrivKey(ret);

			id = key.keyToAddress(ELA_IDCHAIN);

			nlohmann::json j;
			j["ID"] = id;
			j["PrivateKey"] = privateKey;
			return j;
		}

		nlohmann::json IdManager::getLastIdValue(const std::string &path) {
			return nlohmann::json();
		}

		nlohmann::json IdManager::getIdHistoryValues(const std::string &id, const std::string &path) {
			return nlohmann::json();
		}

		void IdManager::AddCallback(IIdManagerCallback *managerCallback) {
			if (std::find(_callbacks.begin(), _callbacks.end(), managerCallback) != _callbacks.end())
				return;
			_callbacks.push_back(managerCallback);
		}

		void IdManager::RemoveCallback(IIdManagerCallback *managerCallback) {
			_callbacks.erase(std::remove(_callbacks.begin(), _callbacks.end(), managerCallback), _callbacks.end());
		}
	}
}