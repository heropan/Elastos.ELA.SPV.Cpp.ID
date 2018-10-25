// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef IDCHAIN_DIDSUBWALLETCALLBACK_H
#define IDCHAIN_DIDSUBWALLETCALLBACK_H

#include <Interface/ididManager.h>
#include "ISubWalletCallback.h"

namespace Elastos {
	namespace DID {


		class DIDSubWalletCallback : public ElaWallet::ISubWalletCallback {
		public:
			DIDSubWalletCallback(IDIDManager *manager);

			~DIDSubWalletCallback();
			/**
			 * Callback method fired when status of a transaction changed.
			 * @param txid indicate hash of the transaction.
			 * @param status can be "Added", "Deleted" or "Updated".
			 * @param desc is an detail description of transaction status.
			 * @param confirms is confirm count util this callback fired.
			 */
			virtual void OnTransactionStatusChanged(
				const std::string &txid,
				const std::string &status,
				const nlohmann::json &desc,
				uint32_t confirms);

			/**
			 * Callback method fired when block begin synchronizing with a peer. This callback could be used to show progress.
			 */
			virtual void OnBlockSyncStarted();

			/**
			 * Callback method fired when best block chain height increased. This callback could be used to show progress.
			 * @param currentBlockHeight is the of current block when callback fired.
			 * @param progress is current progress when block height increased.
			 */
			virtual void OnBlockHeightIncreased(uint32_t currentBlockHeight, int progress);

			/**
			 * Callback method fired when block end synchronizing with a peer. This callback could be used to show progress.
			 */
			virtual void OnBlockSyncStopped();

			virtual void OnBalanceChanged(uint64_t balance);

		private:
			IDIDManager *_DIDManager;
		};

	}
}
#endif //IDCHAIN_DIDSUBWALLETCALLBACK_H
