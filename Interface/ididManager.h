// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_IDIDMANAGER_H__
#define __ELASTOS_SDK_IDIDMANAGER_H__

#include "idid.h"

namespace Elastos {
	namespace SDK {

		class IDIDManager{
		public:

			virtual std::string CreateDID() = 0;

			virtual IDID * GetDID(const std::string &didName) = 0;

			virtual nlohmann::json GetDIDList() const = 0;

			virtual void  DestoryDID(const std::string &didName) = 0;
		};

	}
}

#endif //__ELASTOS_SDK_IDMANAGERFACTORY_H__
