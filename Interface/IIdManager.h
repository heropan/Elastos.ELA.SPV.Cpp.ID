// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_IIDMANAGER_H__
#define __ELASTOS_SDK_IIDMANAGER_H__

#include "IIdChainSubWallet.h"
#include "IIdManagerCallback.h"

namespace Elastos {
	namespace DID {

		class IIdManager {
		public:

			virtual void RegisterId(
					const std::string &id,
					const std::string &key,
					const std::string &password) = 0;

			virtual void RecoverIds(
					const std::vector<std::string> &ids,
					const std::vector<std::string> &keys,
					const std::vector<std::string> &passwords) = 0;

			virtual nlohmann::json GetLastIdValue(
					const std::string &id,
					const std::string &path) const = 0;

			virtual nlohmann::json GetIdHistoryValues(
					const std::string &id,
					const std::string &path) const = 0;

			virtual std::string Sign(
					const std::string &id,
					const std::string &message,
					const std::string &password) = 0;

			virtual nlohmann::json CheckSign(
					const std::string &publicKey,
					const std::string &message,
					const std::string &signature) = 0;

			virtual nlohmann::json GenerateProgram(
					const std::string &id,
					const std::string &message,
					const std::string &password) = 0;

			virtual bool RegisterCallback(
					const std::string &id,
					IIdManagerCallback *callback) = 0;

			virtual bool UnregisterCallback(
					const std::string &id) = 0;
		};

	}
}

#endif //__ELASTOS_SDK_IIDMANAGER_H__
