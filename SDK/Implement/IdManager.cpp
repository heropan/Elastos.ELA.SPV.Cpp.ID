// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//#include "Core/BRAddress.h"

#include "IdManager.h"
#include "SDK/Wrapper/Key.h"
#include "SDK/Common/Utils.h"
#include "SDK/Implement/IdChainSubWallet.h"
#include "SDK/Implement/MasterWallet.h"
#include "SDK/ELACoreExt/Payload/PayloadRegisterIdentification.h"

namespace Elastos {
	namespace SDK {

		IdManager::IdManager() {

		}

		IdManager::~IdManager() {

		}

		nlohmann::json IdManager::GetLastIdValue(const std::string &id, const std::string &path) const {
			//todo get from database
			return nlohmann::json();
		}

		nlohmann::json IdManager::GetIdHistoryValues(const std::string &id, const std::string &path) const {
			//todo get from database
			return nlohmann::json();
		}

		bool IdManager::InitIdCache(IIdChainSubWallet *subWallet) {
			//todo get status from database
			bool hasInitialized = false;
			if (hasInitialized) return true;

			IdChainSubWallet *subWalletInner = dynamic_cast<IdChainSubWallet *>(subWallet);
			assert(subWalletInner != nullptr);

			SharedWrapperList<Transaction, BRTransaction *> transactions =
					subWalletInner->GetWalletManager()->getTransactions(
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

		std::string IdManager::Sign(const std::string &id, const std::string &message, const std::string &password) {
			if (_idKeyMap.find(id) == _idKeyMap.end())
				return "";

			CMBlock keyData = Utils::decrypt(_idKeyMap[id], password);
			KeyPtr key = deriveKey(id, password);

			UInt256 md;
			BRSHA256(&md, message.c_str(), message.size());

			CMBlock signedData = key->sign(md);

			char *data = new char[signedData.GetSize()];
			memcpy(data, signedData, signedData.GetSize());
			std::string singedMsg(data, signedData.GetSize());
			return singedMsg;
		}

		bool IdManager::RegisterId(const std::string &id, const std::string &key, const std::string &password) {
			if (_idKeyMap.find(id) != _idKeyMap.end())
				return false;

			CMBlock secretKey;
			secretKey.SetMemFixed((const uint8_t *) key.c_str(), key.size());

			_idKeyMap[id] = Utils::encrypt(secretKey, password);
			return true;
		}

		nlohmann::json
		IdManager::GenerateProgram(const std::string &id, const std::string &message, const std::string &password) {
			nlohmann::json j;
			std::string signedMsg = Sign(id, message, password);
			if (signedMsg.empty()) return j;

			KeyPtr key = deriveKey(id, password);

			j["parameter"] = signedMsg;
			j["code"] = key->keyToRedeemScript(ELA_IDCHAIN);
			return j;
		}

		KeyPtr IdManager::deriveKey(const std::string &id, const std::string &password) {
			CMBlock keyData = Utils::decrypt(_idKeyMap[id], password);
			char *stmp = new char[keyData.GetSize()];
			memcpy(stmp, keyData, keyData.GetSize());
			std::string secret(stmp, keyData.GetSize());

			KeyPtr key(new Key);
			key->setPrivKey(secret);
			return key;
		}

		bool
		IdManager::RegisterCallback(const std::string &id, IIdManagerCallback *callback, IIdChainSubWallet *subWallet) {
			if (_idListenerMap.find(id) == _idListenerMap.end()) {
				_idListenerMap[id] = ListenerPtr(new SubWalletListener(this));
			}

			_idListenerMap[id]->AddCallback(callback);
			subWallet->AddCallback(_idListenerMap[id].get());
			return true;
		}

		bool IdManager::UnregisterCallback(const std::string &id) {
			if (_idListenerMap.find(id) == _idListenerMap.end())
				return false;

			_idListenerMap.erase(id);
			return true;
		}

		nlohmann::json
		IdManager::CheckSign(const std::string &publicKey, const std::string &message, const std::string &signature) {
			CMBlock signatureData(signature.size());
			memcpy(signatureData, signature.c_str(), signature.size());

			UInt256 md;
			BRSHA256(&md, message.c_str(), message.size());

			bool r = Key::verifyByPublicKey(publicKey, md, signatureData);
			nlohmann::json jsonData;
			jsonData["Result"] = r;
			return jsonData;
		}

		IdManager::SubWalletListener::SubWalletListener(IdManager *manager) : _manager(manager) {
		}

		void IdManager::SubWalletListener::AddCallback(IIdManagerCallback *managerCallback) {
			if (std::find(_callbacks.begin(), _callbacks.end(), managerCallback) != _callbacks.end())
				return;
			_callbacks.push_back(managerCallback);
		}

		void IdManager::SubWalletListener::RemoveCallback(IIdManagerCallback *managerCallback) {
			_callbacks.erase(std::remove(_callbacks.begin(), _callbacks.end(), managerCallback), _callbacks.end());
		}

		void
		IdManager::SubWalletListener::OnTransactionStatusChanged(const std::string &txid, const std::string &status,
																 const nlohmann::json &desc, uint32_t confirms) {
			if (desc.find("ID") == desc.end() || desc.find("Path") == desc.end())
				return;

			std::string id = desc["ID"].get<std::string>();
			std::string path = desc["Path"].get<std::string>();
			nlohmann::json value;
			if (status == "Added" || status == "Updated") {
				value.push_back(desc["DataHash"]);
				value.push_back(desc["Proof"]);
				value.push_back(desc["Sign"]);
			} else if (status == "Deleted") {
				value = _manager->GetLastIdValue(id, path);
			}

			//todo confirms really means block height, edit this in future
			_manager->updateDatabase(id, path, value, confirms);

			std::for_each(_callbacks.begin(), _callbacks.end(), [&id, &path, &value](IIdManagerCallback *callback) {
				callback->OnIdStatusChanged(id, path, value);
			});
		}

	}
}