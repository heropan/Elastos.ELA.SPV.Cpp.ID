// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_IDMANAGER_H__
#define __ELASTOS_SDK_IDMANAGER_H__

#include <vector>
#include <map>

#include "nlohmann/json.hpp"

#include "Interface/IIdManager.h"
#include "Interface/IIdChainSubWallet.h"
#include "Interface/IMasterWallet.h"
#include "SDK/Common/CMemBlock.h"
#include "SDK/Wrapper/Key.h"

namespace Elastos {
	namespace SDK {

		class IdManager : public IIdManager {
		public:
			IdManager();

			virtual ~IdManager();

			virtual bool InitIdCache(
					IIdChainSubWallet *subWallet);

			virtual bool RegisterId(
					const std::string &id,
					const std::string &key,
					const std::string &password);

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
					IIdManagerCallback *callback,
					IIdChainSubWallet *subWallet);

			virtual bool UnregisterCallback(
					const std::string &id);

		protected:

			void tryInitializeIdCache() const;

			void updateDatabase(const std::string &id,
								const std::string &path,
								const nlohmann::json &value,
								uint32_t blockHeight);

			KeyPtr deriveKey(const std::string &id,
							 const std::string &password);

		protected:
			class SubWalletListener : public ISubWalletCallback {
			public:
				SubWalletListener(IdManager *manager);

				void AddCallback(IIdManagerCallback *managerCallback);

				void RemoveCallback(IIdManagerCallback *managerCallback);

				virtual void OnTransactionStatusChanged(
						const std::string &txid,
						const std::string &status,
						const nlohmann::json &desc,
						uint32_t confirms);

			private:
				IdManager *_manager;
				std::vector<IIdManagerCallback *> _callbacks;
			};

		protected:

			typedef std::map<std::string, CMBlock> IdKeyMap;

			typedef boost::shared_ptr<SubWalletListener> ListenerPtr;
			typedef std::map<std::string, ListenerPtr> IdListenerMap;

			IdKeyMap _idKeyMap;

			IdListenerMap _idListenerMap;
		};

	}
}

#endif //__ELASTOS_SDK_IDMANAGER_H__
