// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include "didChecker.h"

#include <SDK/Common/ParamChecker.h>
//#include "didManager.h"

using namespace Elastos::SDK;

namespace Elastos {
	namespace DID {

		didChecker::didChecker(CDidManager* didManager) {

			ParamChecker::checkNullPointer(didManager);
			_pDidManager = didManager;
		}

		didChecker::~didChecker(){

		}

		nlohmann::json didChecker::GetValue(
			const std::string &did,
			const std::string &keyPath) {

			ParamChecker::checkNullPointer(_pDidManager);
			ParamChecker::checkNotEmpty(did);
			ParamChecker::checkNotEmpty(keyPath);


			IDID * idid = _pDidManager->GetDID(did);
			ParamChecker::checkNullPointer(idid);
			return  idid->GetValue(keyPath);
		}

		nlohmann::json didChecker::GetHistoryValue(
			const std::string &did,
			const std::string &keyPath){

			ParamChecker::checkNullPointer(_pDidManager);
			ParamChecker::checkNotEmpty(did);
			ParamChecker::checkNotEmpty(keyPath);


			IDID * idid = _pDidManager->GetDID(did);
			ParamChecker::checkNullPointer(idid);
			return  idid->GetHistoryValue(keyPath);
		}

		nlohmann::json didChecker::GetAllKeys(
			const std::string &did,
			uint32_t start,
			uint32_t count){
			ParamChecker::checkNullPointer(_pDidManager);
			ParamChecker::checkNotEmpty(did);

			IDID * idid = _pDidManager->GetDID(did);
			ParamChecker::checkNullPointer(idid);
			return  idid->GetAllKeys(start, count);
		}

		nlohmann::json didChecker::CheckSign(
			const std::string &did,
			const std::string &publicKey,
			const std::string &message,
			const std::string &signature){

			ParamChecker::checkNullPointer(_pDidManager);
			ParamChecker::checkNotEmpty(did);
			ParamChecker::checkNotEmpty(publicKey);
			ParamChecker::checkNotEmpty(message);


			IDID * idid = _pDidManager->GetDID(did);
			ParamChecker::checkNullPointer(idid);

			//check did and publicKey???????????
			return  idid->CheckSign(message, signature);
		}

		std::string didChecker::GetPublicKey(const std::string &did) {
			ParamChecker::checkNotEmpty(did);
			IDID * idid = _pDidManager->GetDID(did);
			ParamChecker::checkNullPointer(idid);

			return idid->GetPublicKey();
		}

		nlohmann::json didChecker::CheckDID(
			const std::string &did ,
			const std::string &nonce ,
			const std::string &appid ,
			const std::string &declaration,
			const std::string &password){
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