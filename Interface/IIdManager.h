// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_IIDMANAGER_H__
#define __ELASTOS_SDK_IIDMANAGER_H__

#include "IIdManagerCallback.h"

namespace Elastos {
	namespace SDK {

		class IIdManager {
		public:

			virtual nlohmann::json GenerateId(std::string &id, std::string &privateKey) const = 0;

			virtual nlohmann::json GetLastIdValue(const std::string &id, const std::string &path) const = 0;

			virtual nlohmann::json GetIdHistoryValues(const std::string &id, const std::string &path) = 0;

			virtual void AddCallback(IIdManagerCallback *managerCallback) = 0;

			virtual void RemoveCallback(IIdManagerCallback *managerCallback) = 0;
		};

	}
}

#endif //__ELASTOS_SDK_IIDMANAGER_H__
