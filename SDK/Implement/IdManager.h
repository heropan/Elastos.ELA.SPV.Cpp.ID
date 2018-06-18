// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_IDMANAGER_H__
#define __ELASTOS_SDK_IDMANAGER_H__

#include <vector>
#include <map>

#include "nlohmann/json.hpp"

#include "Interface/IIdManager.h"
#include "SDK/Common/CMemBlock.h"
#include "SDK/Wrapper/Key.h"
#include "SDK/SpvService/WalletManager.h"
#include "IdCache.h"

namespace Elastos {
	namespace DID {

		class SpvListener;

		class IdManager : public IIdManager {
		public:
			IdManager(const std::vector<std::string> &initialAddresses);

			virtual ~IdManager();

			virtual void RegisterId(
					const std::string &id,
					const std::string &key,
					const std::string &password);

			virtual void RecoverIds(
					const std::vector<std::string> &ids,
					const std::vector<std::string> &keys,
					const std::vector<std::string> &passwords);

			virtual nlohmann::json GetLastIdValue(
					const std::string &id,
					const std::string &path) const;

			virtual nlohmann::json GetIdHistoryValues(
					const std::string &id,
					const std::string &path) const;

			virtual std::string Sign(
					const std::string &id,
					const std::string &message,
					const std::string &password);

			virtual nlohmann::json CheckSign(
					const std::string &publicKey,
					const std::string &message,
					const std::string &signature);

			virtual nlohmann::json GenerateProgram(
					const std::string &id,
					const std::string &message,
					const std::string &password);

			virtual bool RegisterCallback(
					const std::string &id,
					IIdManagerCallback *callback);

			virtual bool UnregisterCallback(
					const std::string &id);

		protected:

			friend class SpvListener;

			bool initIdCache();

			void initSpvModule(const std::vector<std::string> &initialAddresses);

			void updateDatabase(const std::string &id,
								const std::string &path,
								const nlohmann::json &value,
								uint32_t blockHeight);

			void removeIdItem(const std::string &id,
							  const std::string &path,
							  uint32_t blockHeight);

			SDK::KeyPtr deriveKey(const std::string &id,
							 const std::string &password);

			virtual void OnTransactionStatusChanged(
					const std::string &id,
					const std::string &status,
					const nlohmann::json &desc,
					uint32_t blockHeight);

		protected:
			class SubWalletListener {
			public:
				SubWalletListener(IdManager *manager);

				void AddCallback(IIdManagerCallback *managerCallback);

				void RemoveCallback(IIdManagerCallback *managerCallback);

				void FireCallbacks(const std::string &id,
								   const std::string &path,
								   const nlohmann::json &value);

			private:
				IdManager *_manager;
				std::vector<IIdManagerCallback *> _callbacks;
			};

		protected:

			typedef std::map<std::string, CMBlock> IdKeyMap;

			typedef boost::shared_ptr<SubWalletListener> ListenerPtr;
			typedef std::map<std::string, ListenerPtr> IdListenerMap;

			typedef boost::shared_ptr<SDK::WalletManager> WalletManagerPtr;

			typedef boost::shared_ptr<SpvListener> SpvListenerPtr;

			IdKeyMap _idKeyMap;

			IdListenerMap _idListenerMap;

			WalletManagerPtr _walletManager;

			SpvListenerPtr _spvListener;

			std::string _pathRoot;
			SDK::IdCache _idCache;
		};

	}
}

#endif //__ELASTOS_SDK_IDMANAGER_H__
