// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Interface/IdManagerFactory.h"
//#include "IdManager.h"
#include "didManager.h"

namespace Elastos {
	namespace DID {

		IDIDManager* IdManagerFactory::CreateIdManager(SDK::IMasterWallet* masterWallet, const std::vector<std::string> &initialAddresses) {
			return new CDidManager(masterWallet, initialAddresses);
		}

	}
}