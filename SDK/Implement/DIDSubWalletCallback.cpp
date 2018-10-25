// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "DIDSubWalletCallback.h"
#include "Common/DIDLog.h"
#include "didManager.h"

namespace Elastos {
	namespace DID {

		DIDSubWalletCallback::DIDSubWalletCallback(IDIDManager *manager)
			: _DIDManager(manager) {

		}

		DIDSubWalletCallback::~DIDSubWalletCallback() {

		}

		void DIDSubWalletCallback::OnTransactionStatusChanged(
			const std::string &txid,
			const std::string &status,
			const nlohmann::json &desc,
			uint32_t confirms) {

		}

		void DIDSubWalletCallback::OnBlockSyncStarted() {
			DIDLog::debug("OnBlockSyncStarted");
		}

		void DIDSubWalletCallback::OnBlockHeightIncreased(uint32_t currentBlockHeight, int progress) {
			DIDLog::debug("OnBlockHeightIncreased: height {}, progress", currentBlockHeight, progress);
		}

		void DIDSubWalletCallback::OnBlockSyncStopped() {
			DIDLog::debug("OnBlockSyncStopped");
		}

		void DIDSubWalletCallback::OnBalanceChanged(uint64_t balance) {
			DIDLog::debug("OnBalanceChanged: {}", balance);
		}
	}
}