// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "didManager.h"
#include "did.h"

#include "Core/BRTransaction.h"
#include "SDK/Common/Utils.h"
#include "SDK/Wrapper/Key.h"
#include "SDK/Implement/MasterWallet.h"
#include "SDK/Implement/SubWalletCallback.h"
#include "SDK/ELACoreExt/Payload/PayloadRegisterIdentification.h"
#include "SDK/ELACoreExt/ELATransaction.h"
#include "Common/DIDLog.h"
#include "Interface/IMasterWallet.h"
#include "MasterWallet.h"
#include <algorithm>
#include <SDK/Common/ErrorChecker.h>
#include "IDConfig.h"

#define SPV_DB_FILE_NAME "spv.db"
#define PEER_CONFIG_FILE "id_PeerConnection.json"
#define IDCACHE_DIR_NAME "IdCache"
#define IDCHAIN_NAME     "IdChain"

namespace fs = boost::filesystem;
using namespace Elastos::ElaWallet;



namespace Elastos {
	namespace DID {


		CDidManager::~CDidManager() {

			DidMap::iterator itor ;
			CDid* did = NULL;

			for (DIDSubWalletCallbackMap::iterator it = _didSubWalletCallbacks.begin(); it != _didSubWalletCallbacks.end(); ++it) {
				if (it->second != nullptr) {
					delete it->second;
				}
			}

			for (itor = _didMap.begin(); itor !=  _didMap.end(); ) {

				did = (CDid*)itor->second;
				delete did;
				_didMap.erase(itor++);

			}

		}

		CDidManager::CDidManager(IMasterWallet* masterWallet, const std::string &rootPath)
			: _pathRoot(rootPath) {

			DIDLog::debug("rootPath = {} masterWallet ={:p} begin", rootPath, (void*)masterWallet);

			ErrorChecker::condition(masterWallet == nullptr, Error::InvalidArgument, "Master wallet is null");

			_masterWallet = masterWallet;

			RegisterDIDSubWalletCallback();

			//_iidAgent     = (Elastos::ElaWallet::IIdAgent*)masterWallet;
			Elastos::ElaWallet::MasterWallet* pMasterWallet = (Elastos::ElaWallet::MasterWallet*)_masterWallet;
			_iidAgent     = dynamic_cast<Elastos::ElaWallet::IIdAgent*>(pMasterWallet);
			ErrorChecker::condition(_iidAgent == nullptr, Error::InvalidArgument, "Master wallet is not instance of ID agent");
			initIdCache();
			DIDLog::debug("CDidManager::CDidManager rootPath = {} end", rootPath.c_str());
		}

		void CDidManager::RegisterDIDSubWalletCallback() {
			std::vector<ISubWallet *> subWallets = _masterWallet->GetAllSubWallets();

			for (int i = 0; i < subWallets.size(); ++i) {
				if (subWallets[i]->GetChainId() == IDCHAIN_NAME){
					if (_didSubWalletCallbacks.find(_masterWallet->GetId()) == _didSubWalletCallbacks.end()) {
						DIDSubWalletCallback *callback = new DIDSubWalletCallback(this);
						_didSubWalletCallbacks[_masterWallet->GetId()] = callback;
						subWallets[i]->AddCallback(callback);
						break;
					}
				}
			}
		}

		IDID * CDidManager::CreateDID(const std::string &password){

			DIDLog::info("CDidManager::CreateDID password = {} begin", password);
			//DIDLog::error("2222CDidManager::CreateDID password = {} begin", password);

			ErrorChecker::checkPassword(password, "ID");

			std::vector<std::string> allIdsVec = _iidAgent->GetAllIds();

			uint32_t index =  allIdsVec.size();

			std::string didNameStr = "";
			didNameStr = _iidAgent->DeriveIdAndKeyForPurpose(1 , index );//, password

			std::cout<<"new didNameStr " <<didNameStr <<std::endl;
			nlohmann::json defJson ={
				{"121", {{"datahash", "datahash1"}, {"proof", "hello proof1"}, {"sign", "hello sign1"}}},
				{"132", {{"datahash", "datahash2"}, {"proof", "hello proof2"}, {"sign", "hello sign2"}}},
				{"103", {{"datahash", "datahash3"}, {"proof", "hello proof3"}, {"sign", "hello sign3"}}}
			};
			//notice didNameStr must be store in leveldb before user setValue
			//so path is ""  is not user data
			_idCache->Put(didNameStr, "", defJson);

			DIDLog::error("ElastosID CreateDID ---------- ID str didNameStr {}", didNameStr);

			return NewDid(didNameStr);
		}

		IDID * CDidManager::GetDID(const std::string &didName)  {

			if (_didMap.find(didName) == _didMap.end())
				return NULL;

			return  _didMap[didName];
		}

		nlohmann::json CDidManager::GetDIDList() const {

//			nlohmann::json didJson;
//
//			DidMap::const_iterator itor  = _didMap.begin() ;
//			for (itor  =  _didMap.begin() ; itor != _didMap.end(); itor++) {
//
//				didJson.push_back(itor->first);
//			}
//			return didJson ;
			DIDLog::debug("GetDIDList begin");

			return _idCache->GetAllKey();
		}

		void CDidManager::DestoryDID(const std::string &didName){

			DIDLog::info("DestoryDID didName {} begin",  didName);
			DidMap::iterator itor ;
			itor = _didMap.find(didName);
			if (itor == _didMap.end())
				return;

			CDid * idID = (CDid *)itor->second;
			_didMap.erase(itor);
			delete idID;

			_idCache->Delete(didName);
			DIDLog::info("DestoryDID didName {} end",  didName);

		}

		void CDidManager::ClearLocal() {
			_idCache->DeleteAll();
		}

		nlohmann::json CDidManager::GetLastIdValue(const std::string &id, const std::string &path)  {
			ErrorChecker::argumentNotEmpty(id, "ID");
			ErrorChecker::argumentNotEmpty(path, "Path");

			CDid * idID  = (CDid *)GetDID(id);
			return idID->GetValue(path);

		}

		void CDidManager::updateDatabase(const std::string &id, const std::string &path, const nlohmann::json &value,
									   uint32_t blockHeight) {
			//todo consider block height equal INT_MAX(unconfirmed)
			//todo parse proof, data hash, sign from value


			ErrorChecker::argumentNotEmpty(id, "ID");
			ErrorChecker::argumentNotEmpty(path, "Path");

			CDid * idID  = (CDid *)GetDID(id);
			return idID->setValue(path, value, blockHeight);
		}

		void CDidManager::removeIdItem(const std::string &id, const std::string &path, uint32_t blockHeight) {
			ErrorChecker::argumentNotEmpty(id, "ID");
			ErrorChecker::argumentNotEmpty(path, "Path");

			CDid * idID  = (CDid *)GetDID(id);
			idID->DelValue(path, blockHeight);

		}

		ISubWallet * CDidManager::GetIDSubWallet(){
			DIDLog::info("GetIDSubWallet  begin");

			std::vector<ISubWallet *> subWalletVec;
			subWalletVec = _masterWallet->GetAllSubWallets();
			ISubWallet * subWallet = NULL;

			for (int i = 0; i < subWalletVec.size(); ++i) {
				subWallet = subWalletVec[i];
				if (subWallet->GetChainId() ==  IDCHAIN_NAME){
					break;
				}
				subWallet = NULL;
			}
			DIDLog::info("GetIDSubWallet subWallet int: {:p} end", ( void* )subWallet);//{0:x}
			return subWallet;
		}

		bool CDidManager::initIdCache() {

			DIDLog::info("initIdCache  begin");
			if (_idCache != NULL){
				DIDLog::debug("initIdCache  _idCache != NULL end");
				return false;
			}


			fs::path idCachePath = _pathRoot;
			idCachePath /= IDCACHE_DIR_NAME;

			_idCache = IdCachePtr(new IdCache(idCachePath));

			nlohmann::json jsonRet = _idCache->GetAllKey();

			for (nlohmann::json::const_iterator it = jsonRet.begin(); it != jsonRet.end(); it++) {
				NewDid(it.value());
			}
			DIDLog::info("initIdCache  end");
			return true;
		}


		bool CDidManager::RegisterCallback(const std::string &id, IIdManagerCallback *callback) {
			DIDLog::info("RegisterCallback  begin id {} callback {:p}", id, ( void* )callback);


			if (_idListenerMap.find(id) == _idListenerMap.end()) {
				_idListenerMap[id] = ListenerPtr(new SubWalletListener(this));
			}

			_idListenerMap[id]->AddCallback(callback);
			DIDLog::info("RegisterCallback  end id {} callback {:p}", id, ( void* )callback);

			return true;
		}

		bool CDidManager::UnregisterCallback(const std::string &id) {
			DIDLog::info("UnregisterCallback  begin id {} ", id);

			if (_idListenerMap.find(id) == _idListenerMap.end())
				return false;
			_idListenerMap.erase(id);
			DIDLog::info("UnregisterCallback  end id {} ", id);

			return true;
		}

		IDID *  CDidManager::NewDid(const std::string didNameStr) {
			DIDLog::debug("NewDid  begin didNameStr {} ", didNameStr);

			CDid * idID = new  CDid(this, didNameStr);
			_didMap[didNameStr] =  idID;
			RegisterId(didNameStr);
			DIDLog::info("NewDid  end didNameStr {} ", didNameStr);

			return idID;
		}

		void CDidManager::RegisterId(const std::string &id) {

			//WalletManager future use
			//AddressRegisteringWallet *wallet = static_cast<AddressRegisteringWallet *>(_walletManager->getWallet().get());
			//wallet->RegisterAddress(id);


		}

		void CDidManager::RecoverIds(const std::vector<std::string> &ids, const std::vector<std::string> &keys,
								   const std::vector<std::string> &passwords) {
			//WalletManager future use
//			for (int i = 0; i < ids.size(); ++i) {
//				RegisterId(ids[i]);
//			}
//			_walletManager->getPeerManager()->rescan();
		}


		CDidManager::SubWalletListener::SubWalletListener(CDidManager *manager) : _manager(manager) {

		}
		void CDidManager::SubWalletListener::FireCallbacks(const std::string &id, const std::string &path,
														 const nlohmann::json &value) {
			DIDLog::info("FireCallbacks  begin didNameStr id {} path{} value{}", id , path, value.dump());

			std::for_each(_callbacks.begin(), _callbacks.end(), [&id, &path, &value](IIdManagerCallback *callback) {
				callback->OnIdStatusChanged(id, path, value);
			});
			DIDLog::info("FireCallbacks  end didNameStr id {} path{} value{}", id , path, value.dump());
		}

		void CDidManager::SubWalletListener::AddCallback(IIdManagerCallback *managerCallback) {
			DIDLog::info("AddCallback  begin didNameStr {:p} ", ( void* )managerCallback);

			if (std::find(_callbacks.begin(), _callbacks.end(), managerCallback) != _callbacks.end())
				return;
			_callbacks.push_back(managerCallback);
			DIDLog::info("AddCallback  end didNameStr {:p} ", ( void* )managerCallback);

		}

		void CDidManager::SubWalletListener::RemoveCallback(IIdManagerCallback *managerCallback) {
			DIDLog::info("RemoveCallback  begin didNameStr {:p} ", ( void* )managerCallback);

			_callbacks.erase(std::remove(_callbacks.begin(), _callbacks.end(), managerCallback), _callbacks.end());

			DIDLog::info("RemoveCallback  end didNameStr {:p} ", ( void* )managerCallback);

		}
	}

}