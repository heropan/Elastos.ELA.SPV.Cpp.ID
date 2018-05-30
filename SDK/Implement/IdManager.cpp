// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//#include "Core/BRAddress.h"

#include "IdManager.h"
#include "SDK/Wrapper/Key.h"
#include "SDK/Common/Utils.h"
#include "SDK/Implement/IdChainSubWallet.h"
#include "SDK/ELACoreExt/Payload/PayloadRegisterIdentification.h"

namespace Elastos {
	namespace SDK {

		IdManager::IdManager(IIdChainSubWallet *subWallet) :
				_subWallet(subWallet) {
			tryInitialize();
		}

		IdManager::~IdManager() {

		}

		nlohmann::json IdManager::GenerateId(std::string &id, std::string &privateKey) const {
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

		nlohmann::json IdManager::GetLastIdValue(const std::string &id, const std::string &path) const {
			//todo get from database
			return nlohmann::json();
		}

		nlohmann::json IdManager::GetIdHistoryValues(const std::string &id, const std::string &path) {
			//todo get from database
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

		void IdManager::OnTransactionStatusChanged(const std::string &txid, const std::string &status,
												   const nlohmann::json &desc, uint32_t confirms) {
			std::string id = desc["ID"].get<std::string>();
			std::string path = desc["Path"].get<std::string>();
			nlohmann::json value;
			if (status == "Added" || status == "Updated") {
				value = desc["Value"];
			} else if (status == "Deleted") {
				value = GetLastIdValue(id, path);
			}

			//todo confirms really means block height, edit this in future
			updateDatabase(id, path, value, confirms);

			std::for_each(_callbacks.begin(), _callbacks.end(), [&id, &path, &value](IIdManagerCallback *callback) {
				callback->OnIdStatusChanged(id, path, value);
			});
		}

		void IdManager::tryInitialize() const {
			//todo get status from database
			bool hasInitialized = false;
			if (hasInitialized) return;

			IdChainSubWallet *subWallet = dynamic_cast<IdChainSubWallet *>(_subWallet);
			assert(subWallet != nullptr);

			SharedWrapperList<Transaction, BRTransaction *> transactions = subWallet->GetWalletManager()->getTransactions(
					[](const TransactionPtr &transaction) {
						return transaction->getTransactionType() == Transaction::RegisterIdentification;
					});
			std::for_each(transactions.begin(), transactions.end(),
						  [](const TransactionPtr &transaction) {
							  PayloadRegisterIdentification *payload =
									  static_cast<PayloadRegisterIdentification *>(transaction->getPayload().get());
							  uint32_t blockHeight = transaction->getBlockHeight();
							  //todo store in data base
						  });
		}

		void IdManager::updateDatabase(const std::string &id, const std::string &path, const nlohmann::json &value,
									   uint32_t blockHeight) {
			//todo consider block height equal INT_MAX(unconfirmed)
			//todo parse proof, data hash, sign from value
			//todo write to database
		}
	}
}