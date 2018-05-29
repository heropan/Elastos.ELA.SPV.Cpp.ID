// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_IDMANAGER_H__
#define __ELASTOS_SDK_IDMANAGER_H__

#include <vector>

#include "nlohmann/json.hpp"

#include "Interface/IIdManager.h"

namespace Elastos {
	namespace SDK {

		class IdManager : public IIdManager {
		public:
			IdManager();

			virtual ~IdManager();

			virtual nlohmann::json GenerateId(std::string &id, std::string &privateKey);

			virtual nlohmann::json getLastIdValue(const std::string &path);

			virtual nlohmann::json getIdHistoryValues(const std::string &id, const std::string &path);

			virtual void AddCallback(IIdManagerCallback *managerCallback);

			virtual void RemoveCallback(IIdManagerCallback *managerCallback);

		private:

			std::vector<IIdManagerCallback *> _callbacks;
		};

	}
}

#endif //__ELASTOS_SDK_IDMANAGER_H__
