// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Interface/IdManagerFactory.h"
#include "IdManager.h"

namespace Elastos {
	namespace SDK {

		IIdManager* IdManagerFactory::CreateIdManager(const std::vector<std::string> &initialAddresses) {
			return new IdManager(initialAddresses);
		}

	}
}