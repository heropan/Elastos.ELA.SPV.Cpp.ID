// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_IDMANAGER_H__
#define __ELASTOS_SDK_IDMANAGER_H__

#include <vector>

#include "nlohmann/json.hpp"

#include "Interface/IIdManager.h"
#include "Interface/IIdChainSubWallet.h"

namespace Elastos {
	namespace SDK {

		class IdManager : public IIdManager, public ISubWalletCallback {
		public:
			IdManager(IIdChainSubWallet *subWallet);

			virtual ~IdManager();

			virtual nlohmann::json GenerateId(std::string &id, std::string &privateKey) const;

			virtual nlohmann::json GetLastIdValue(const std::string &id, const std::string &path) const;

			virtual nlohmann::json GetIdHistoryValues(const std::string &id, const std::string &path);

			virtual void AddCallback(IIdManagerCallback *managerCallback);

			virtual void RemoveCallback(IIdManagerCallback *managerCallback);

			virtual void OnTransactionStatusChanged(
					const std::string &txid,
					const std::string &status,
					const nlohmann::json &desc,
					uint32_t confirms);

		protected:
			void tryInitialize() const;

			void updateDatabase(const std::string &id,
								const std::string &path,
								const nlohmann::json &value,
								uint32_t blockHeight);

		protected:

			std::vector<IIdManagerCallback *> _callbacks;
			IIdChainSubWallet *_subWallet;
		};

	}
}

#endif //__ELASTOS_SDK_IDMANAGER_H__
