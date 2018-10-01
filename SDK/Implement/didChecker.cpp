// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <SDK/Common/ErrorChecker.h>
#include "didChecker.h"

using namespace Elastos::ElaWallet;

namespace Elastos {
	namespace DID {

		didChecker::didChecker(CDidManager* didManager) {

			ErrorChecker::condition(didManager == nullptr, Error::InvalidArgument, "DID manager is null");
			_pDidManager = didManager;
		}

		didChecker::~didChecker(){

		}

		nlohmann::json didChecker::GetValue(
			const std::string &did,
			const std::string &keyPath) {

			ErrorChecker::argumentNotEmpty(did, "DID");
			ErrorChecker::argumentNotEmpty(keyPath, "Key path");


			IDID *idid = _pDidManager->GetDID(did);
			ErrorChecker::condition(idid == nullptr, Error::DIDNotFound, "DID not found");
			return idid->GetValue(keyPath);
		}

		nlohmann::json didChecker::GetHistoryValue(
			const std::string &did,
			const std::string &keyPath){

			ErrorChecker::argumentNotEmpty(did, "DID");
			ErrorChecker::argumentNotEmpty(keyPath, "Key path");

			IDID * idid = _pDidManager->GetDID(did);
			ErrorChecker::condition(idid == nullptr, Error::DIDNotFound, "DID not found");

			return  idid->GetHistoryValue(keyPath);
		}

		nlohmann::json didChecker::GetAllKeys(const std::string &did, uint32_t start, uint32_t count) {
			ErrorChecker::argumentNotEmpty(did, "DID");

			IDID * idid = _pDidManager->GetDID(did);
			ErrorChecker::condition(idid == nullptr, Error::DIDNotFound, "DID not found");

			return  idid->GetAllKeys(start, count);
		}

		nlohmann::json didChecker::CheckSign(
			const std::string &did,
			const std::string &publicKey,
			const std::string &message,
			const std::string &signature) {

			ErrorChecker::argumentNotEmpty(did, "DID");
			ErrorChecker::argumentNotEmpty(publicKey, "Pubkey");
			ErrorChecker::argumentNotEmpty(message, "Message");


			IDID * idid = _pDidManager->GetDID(did);
			ErrorChecker::condition(idid == nullptr, Error::DIDNotFound, "DID not found");

			//check did and publicKey???????????
			return  idid->CheckSign(message, signature);
		}

		std::string didChecker::GetPublicKey(const std::string &did) {
			ErrorChecker::argumentNotEmpty(did, "DID");
			IDID * idid = _pDidManager->GetDID(did);
			ErrorChecker::condition(idid == nullptr, Error::DIDNotFound, "DID not found");

			return idid->GetPublicKey();
		}

		nlohmann::json didChecker::CheckDID(
			const std::string &did ,
			const std::string &nonce ,
			const std::string &appid ,
			const std::string &declaration,
			const std::string &password) {
//
//			//首先检查DID是否存在
//			ParamChecker::checkNotEmpty(did);
//			IDID * idid = _pDidManager->GetDID(did);
//
//			nlohmann::json retJson;
//			if(idid == NULL){
//
//				//json字符串 设置为返回值false
//				//sign 这个字段为空字符串
//
//				retJson["Result"] = false;
//				retJson["sign"] = "";
//			}
//			ParamChecker::checkNullPointer(idid);
//
//
//			retJson["Result"] = true;
//			retJson["sign"] = ;
//			//pass word
//			//msg
//			idid->Sign( , password);
			return nlohmann::json();
		}
	}
}