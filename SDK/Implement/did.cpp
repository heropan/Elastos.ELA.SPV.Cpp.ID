//
// Created by hp on 2018/6/16.
//
#include "did.h"

#include <boost/filesystem.hpp>
#include <SDK/Common/ParamChecker.h>
#include "didManager.h"


namespace fs = boost::filesystem;

using namespace Elastos::ElaWallet;
namespace Elastos {
	namespace DID {

		CDid::CDid(CDidManager* didManager, const std::string &id) {
			ParamChecker::checkNullPointer(didManager);
			ParamChecker::checkNotEmpty(id);

			_didManger = didManager;
			_didNameStr = id;
		}

		CDid::~CDid(){

		}

		std::string CDid::GetDIDName(){

			return _didNameStr;
		}

		void CDid::CheckInit() const {
			ParamChecker::checkNotEmpty(_didNameStr);
			//ParamChecker::checkNotEmpty(_passWord);
		}

		void CDid::DelValue(
			const std::string &path,
			uint32_t blockHeight ){

			CheckInit();
			ParamChecker::checkNotEmpty(path);


			_didManger->_idCache->Delete(_didNameStr, path, blockHeight);
		}

		void CDid::setValue(
					  const std::string &path,
					  const nlohmann::json &value,
					  uint32_t blockHeight){

			CheckInit();
			ParamChecker::checkNotEmpty(path);

			_didManger->_idCache->Put(_didNameStr,  path, blockHeight , value);
		}

		void CDid::SetValue(
			const std::string &keyPath ,
			const nlohmann::json &valueJson){

			CheckInit();
			ParamChecker::checkNotEmpty(keyPath);
			//检查是否json ???

			_didManger->_idCache->Put(_didNameStr,  keyPath, valueJson);

		}

		nlohmann::json CDid::GetValue(
			const std::string &path) const {

			CheckInit();
			ParamChecker::checkNotEmpty(path);

			nlohmann::json jsonGet;
			jsonGet =_didManger-> _idCache->Get(_didNameStr, path);
			if (jsonGet.empty()) {
				return nlohmann::json();
			}

			nlohmann::json lastIdValue;
			uint32_t blockHeight = 0;
			for (nlohmann::json::iterator it = jsonGet.begin(); it != jsonGet.end(); it++) {
				uint32_t getBlockHeight = (uint32_t)std::stoi(it.key(), nullptr, 10);
				if (getBlockHeight > blockHeight) {
					lastIdValue[it.key()] = it.value();
					blockHeight = getBlockHeight;
				}
			}

			return lastIdValue;

		}

		nlohmann::json CDid::GetHistoryValue(
			const std::string &keyPath) const {

			CheckInit();
			ParamChecker::checkNotEmpty(keyPath);


			return _didManger->_idCache->Get(_didNameStr, keyPath);
		}

		//start下标第一个为0
		nlohmann::json CDid::GetAllKeys(
			uint32_t start ,
			uint32_t count) const {

			CheckInit();

			nlohmann::json keysJson;

			if(count == 0){

				throw std::invalid_argument("count mut big than 0");
			}

			uint32_t addCount = 0;
			nlohmann::json jsonGet;
			jsonGet =_didManger-> _idCache->Get(_didNameStr);

			if(start >= jsonGet.size()){

				throw std::invalid_argument("start >= jsonGet.size()");
			}

			nlohmann::json::const_iterator it = jsonGet.begin();
			std::advance (it,start);
			for (; it != jsonGet.end(); it++) {
				keysJson.push_back(it.key());
				addCount++;

				if(addCount >= count){
					break;
				}
			}


			return keysJson;
		}

		nlohmann::json CDid::GenerateProgram(
			const std::string &message,
			const std::string &password){

			return _didManger->_masterWallet->GenerateProgram(_didNameStr, message, password);
		}

		//key password ？
		std::string CDid::Sign(
			const std::string &message , const std::string &password){

			ParamChecker::checkNotEmpty(password);
			CheckInit();

			return _didManger->_masterWallet->Sign(_didNameStr, message, password);
		}

		nlohmann::json CDid::CheckSign(
			const std::string &message ,
			const std::string &signature){

			CheckInit();


			CMBlock signatureData(signature.size());
			memcpy(signatureData, signature.c_str(), signature.size());

			UInt256 md;
			BRSHA256(&md, message.c_str(), message.size());


			bool r = Key::verifyByPublicKey(GetPublicKey(), md, signatureData);
			nlohmann::json jsonData;
			jsonData["Result"] = r;
			return jsonData;
		}

		std::string CDid::GetPublicKey() {
			CheckInit();
			return _didManger->_masterWallet->GetPublicKey(_didNameStr);
		}

	}
}