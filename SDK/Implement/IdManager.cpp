// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/filesystem.hpp>

#include "IdManager.h"
#include "Core/BRTransaction.h"
#include "SDK/Common/Utils.h"
#include "SDK/Wrapper/Key.h"
#include "SDK/Wrapper/AddressRegisteringWallet.h"
#include "SDK/Implement/IdChainSubWallet.h"
#include "SDK/Implement/MasterWallet.h"
#include "SDK/Implement/SubWalletCallback.h"
#include "SDK/ELACoreExt/Payload/PayloadRegisterIdentification.h"
#include "SDK/ELACoreExt/ELABRTransaction.h"

#define SPV_DB_FILE_NAME "spv.db"
#define PEER_CONFIG_FILE "id_PeerConnection.json"

namespace fs = boost::filesystem;

namespace Elastos {
	namespace SDK {


		class SpvListener : public Wallet::Listener {
		public:
			SpvListener(IdManager *manager) : _manager(manager) {
			}

			virtual void balanceChanged(uint64_t balance) {

			}

			virtual void onTxAdded(const TransactionPtr &transaction) {

				if (transaction->getTransactionType() != Transaction::RegisterIdentification)
					return;

				fireTransactionStatusChanged(
						static_cast<PayloadRegisterIdentification *>(transaction->getPayload().get()),
						SubWalletCallback::Added, transaction->getBlockHeight());
			}

			virtual void onTxUpdated(const std::string &hash, uint32_t blockHeight, uint32_t timeStamp) {
				BRTransaction *transaction = BRWalletTransactionForHash(
						_manager->_walletManager->getWallet()->getRaw(), Utils::UInt256FromString(hash));
				if (transaction == nullptr ||
					((ELABRTransaction *) transaction)->type != Transaction::RegisterIdentification)
					return;

				Transaction wrapperTx(transaction);
				PayloadRegisterIdentification *payload = static_cast<PayloadRegisterIdentification *>(
						wrapperTx.getPayload().get());
				fireTransactionStatusChanged(payload, SubWalletCallback::Updated, blockHeight);
			}

			virtual void onTxDeleted(const std::string &hash, bool notifyUser, bool recommendRescan) {
				BRTransaction *transaction = BRWalletTransactionForHash(
						_manager->_walletManager->getWallet()->getRaw(), Utils::UInt256FromString(hash));
				if (transaction == nullptr ||
					((ELABRTransaction *) transaction)->type != Transaction::RegisterIdentification)
					return;

				Transaction wrapperTx(transaction);
				PayloadRegisterIdentification *payload = static_cast<PayloadRegisterIdentification *>(
						wrapperTx.getPayload().get());
				fireTransactionStatusChanged(payload, SubWalletCallback::Deleted, transaction->blockHeight);
			}

		private:
			void fireTransactionStatusChanged(PayloadRegisterIdentification *payload,
											  SubWalletCallback::TransactionStatus status, uint32_t blockHeight) {
				nlohmann::json j = payload->toJson();
				if (j.find("ID") == j.end())
					return;

				_manager->OnTransactionStatusChanged(j["ID"].get<std::string>(), SubWalletCallback::convertToString(
						status), j, blockHeight);
			}

			IdManager *_manager;
		};

		IdManager::IdManager(const std::vector<std::string> &initialAddresses) :
				_pathRoot("Data") {
			initSpvModule(initialAddresses);
			initIdCache();
		}

		IdManager::~IdManager() {

		}

		void IdManager::RegisterId(const std::string &id, const std::string &key, const std::string &password) {
			if (_idKeyMap.find(id) != _idKeyMap.end())
				return;

			CMBlock secretKey;
			secretKey.SetMemFixed((const uint8_t *) key.c_str(), key.size());

			_idKeyMap[id] = Utils::encrypt(secretKey, password);

			AddressRegisteringWallet *wallet = static_cast<AddressRegisteringWallet *>(_walletManager->getWallet().get());
			wallet->RegisterAddress(id);
		}

		void IdManager::RecoverIds(const std::vector<std::string> &ids, const std::vector<std::string> &keys,
								   const std::vector<std::string> &passwords) {
			for (int i = 0; i < ids.size(); ++i) {
				RegisterId(ids[i], keys[i], passwords[i]);
			}
			_walletManager->getPeerManager()->rescan();
		}

		nlohmann::json IdManager::GetLastIdValue(const std::string &id, const std::string &path) const {
			//todo get from database
			return nlohmann::json();
		}

		nlohmann::json IdManager::GetIdHistoryValues(const std::string &id, const std::string &path) const {
			//todo get from database
			return nlohmann::json();
		}

		bool IdManager::initIdCache() {
			//todo get status from database
			bool hasInitialized = false;
			if (hasInitialized) return true;


			SharedWrapperList<Transaction, BRTransaction *> transactions =
					_walletManager->getTransactions(
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
			return true;
		}

		void IdManager::updateDatabase(const std::string &id, const std::string &path, const nlohmann::json &value,
									   uint32_t blockHeight) {
			//todo consider block height equal INT_MAX(unconfirmed)
			//todo parse proof, data hash, sign from value
			//todo write to database
		}

		void IdManager::removeIdItem(const std::string &id, const std::string &path, uint32_t blockHeight) {
			//todo remove id value with specified path and block height
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
		IdManager::RegisterCallback(const std::string &id, IIdManagerCallback *callback) {
			if (_idListenerMap.find(id) == _idListenerMap.end()) {
				_idListenerMap[id] = ListenerPtr(new SubWalletListener(this));
			}

			_idListenerMap[id]->AddCallback(callback);
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

		void IdManager::initSpvModule(const std::vector<std::string> &initialAddresses) {

			fs::path dbPath = _pathRoot;
			dbPath /= SPV_DB_FILE_NAME;
			fs::path peerConfigPath = _pathRoot;
			peerConfigPath /= PEER_CONFIG_FILE;
			_walletManager = WalletManagerPtr(
					new WalletManager(dbPath, peerConfigPath, 0, 0, initialAddresses, ChainParams::mainNet()));

			_walletManager->registerWalletListener(_spvListener.get());
		}

		void
		IdManager::OnTransactionStatusChanged(const std::string &id, const std::string &status,
											  const nlohmann::json &desc, uint32_t blockHeight) {

			std::string path = desc["Path"].get<std::string>();
			nlohmann::json value;
			if (status == "Added" || status == "Updated") {
				value.push_back(desc["DataHash"]);
				value.push_back(desc["Proof"]);
				value.push_back(desc["Sign"]);

				updateDatabase(id, path, value, blockHeight);
			} else if (status == "Deleted") {
				value = GetLastIdValue(id, path);

				removeIdItem(id, path, blockHeight);
			}

			if (_idListenerMap.find(id) != _idListenerMap.end())
				_idListenerMap[id]->FireCallbacks(id, status, value);
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

		void IdManager::SubWalletListener::FireCallbacks(const std::string &id, const std::string &path,
														 const nlohmann::json &value) {
			std::for_each(_callbacks.begin(), _callbacks.end(), [&id, &path, &value](IIdManagerCallback *callback) {
				callback->OnIdStatusChanged(id, path, value);
			});
		}

	}
}