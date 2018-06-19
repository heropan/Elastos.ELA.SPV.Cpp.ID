// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef IDCHAIN_DIDMANAGER_H
#define IDCHAIN_DIDMANAGER_H

#include <map>
#include <Interface/IIdManagerCallback.h>

#include "nlohmann/json.hpp"
#include "Interface/ididManager.h"
#include "SDK/SpvService/WalletManager.h"
#include "IdCache.h"
#include <SDK/IdAgent/IdAgentImpl.h>
#include "MasterWallet.h"



namespace Elastos {
	namespace DID {
		class SpvListener;


		class CDidManager : public IDIDManager
		{
		public:

			CDidManager(SDK::IMasterWallet* masterWallet) ;

			virtual ~CDidManager();

			virtual IDID * CreateDID(const std::string &password);

			virtual IDID * GetDID(const std::string &didName) ;
			//all id
			virtual nlohmann::json GetDIDList() const;

			virtual void  DestoryDID(const std::string &didName);

		protected:
			friend class SpvListener;


			virtual void OnTransactionStatusChanged(
				const std::string &id,
				const std::string &status,
				const nlohmann::json &desc,
				uint32_t blockHeight);

			void updateDatabase(const std::string &id,
								const std::string &path,
								const nlohmann::json &value,
								uint32_t blockHeight);

			virtual nlohmann::json GetLastIdValue(
				const std::string &id,
				const std::string &path) ;

			void removeIdItem(const std::string &id,
							  const std::string &path,
							  uint32_t blockHeight);

			void initSpvModule();

			bool initIdCache();

			bool  RegisterCallback(const std::string &id, IIdManagerCallback *callback);
			bool  UnregisterCallback(const std::string &id);
			void  RecoverIds(const std::vector<std::string> &ids, const std::vector<std::string> &keys,
									   const std::vector<std::string> &passwords);

			void  RegisterId(const std::string &id);

			IDID *  NewDid(const std::string didNameStr);

		protected:
			class SubWalletListener {
			public:
				SubWalletListener(CDidManager *manager);

				void AddCallback(IIdManagerCallback *managerCallback);

				void RemoveCallback(IIdManagerCallback *managerCallback);

				void FireCallbacks(const std::string &id,
								   const std::string &path,
								   const nlohmann::json &value);

			private:
				CDidManager *_manager;
				std::vector<IIdManagerCallback *> _callbacks;
			};

		protected:
			typedef std::map<std::string, IDID *> DidMap;


			typedef std::map<std::string, CMBlock> IdKeyMap;

			typedef boost::shared_ptr<SubWalletListener> ListenerPtr;
			typedef std::map<std::string, ListenerPtr> IdListenerMap;

			typedef boost::shared_ptr<SDK::WalletManager> WalletManagerPtr;

			typedef boost::shared_ptr<SpvListener> SpvListenerPtr;

			typedef boost::shared_ptr<SDK::IdCache> IdCachePtr;
			friend class CDid;
			DidMap	_didMap;


			IdListenerMap _idListenerMap;

			WalletManagerPtr _walletManager;

			SpvListenerPtr _spvListener;
			std::string _pathRoot;
			IdCachePtr _idCache;

			Elastos::SDK::MasterWallet*	_masterWallet;
		};
	}
}

#endif //IDCHAIN_DIDMANAGER_H
