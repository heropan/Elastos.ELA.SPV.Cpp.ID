// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Interface/IdManagerFactory.h"
#include "didManager.h"
#include "Log.h"
#include "IDConfig.h"

namespace Elastos {
	namespace DID {

		IDIDManager* IdManagerFactory::CreateIdManager(ElaWallet::IMasterWallet* masterWallet, const std::string &rootPath) {
			ElaWallet::Log::getLogger()->critical("libidchain.so version {}", IDCHAIN_VERSION_MESSAGE);
			return new CDidManager(masterWallet, rootPath);
		}

	}
}