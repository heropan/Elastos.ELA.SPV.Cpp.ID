// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "Interface/DIDManagerSupervisor.h"
#include "didManager.h"
#include "Log.h"
#include "IDConfig.h"

namespace Elastos {
	namespace DID {

		DIDManagerSupervisor::DIDManagerSupervisor(const std::string &rootPath) :
			_rootPath(rootPath) {

		}

		IDIDManager *
		DIDManagerSupervisor::CreateDIDManager(ElaWallet::IMasterWallet *masterWallet, const std::string &rootPath) {
			ElaWallet::Log::getLogger()->critical("libidchain.so version {}", IDCHAIN_VERSION_MESSAGE);
			return new CDidManager(masterWallet, rootPath);
		}

		void DIDManagerSupervisor::DestroyDIDManager(IDIDManager *manager) {
			CDidManager *IDManager = static_cast<CDidManager*>(manager);
			IDManager->ClearLocal();
			delete IDManager;
		}

	}
}