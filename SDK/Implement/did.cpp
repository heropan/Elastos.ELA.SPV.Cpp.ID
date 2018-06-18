//
// Created by hp on 2018/6/16.
//
#include "did.h"

#include <boost/filesystem.hpp>
#include <SDK/Common/ParamChecker.h>
#include "didManager.h"


namespace fs = boost::filesystem;

using namespace Elastos::SDK;
namespace Elastos {
	namespace DID {

		CDid::CDid(CDidManager* didManager) {
			ParamChecker::checkNullPointer(didManager);
			_didManger = didManager;
		}

		CDid::~CDid(){

		}

		std::string CDid::GetDIDName(const std::string &password){

			ParamChecker::checkPassword(password);

			uint32_t index =  _didManger->_masterWallet->GetAllIds().size();

			_passWord = password;
			_didNameStr = _didManger->_masterWallet->DeriveIdAndKeyForPurpose(1 , index , password);
			return _didNameStr;
		}

		void CDid::CheckInit() const {
			ParamChecker::checkNotEmpty(_didNameStr);
			ParamChecker::checkNotEmpty(_passWord);
		}

		void CDid::DelValue(
			const std::string &path,
			uint32_t blockHeight ){

			CheckInit();
			ParamChecker::checkNotEmpty(path);


			_didManger->_idCache.Delete(_didNameStr, path, blockHeight);
		}

		void CDid::setValue(
					  const std::string &path,
					  const nlohmann::json &value,
					  uint32_t blockHeight){

			CheckInit();
			ParamChecker::checkNotEmpty(path);

			_didManger->_idCache.Put(_didNameStr,  path, blockHeight , value);
		}

		void CDid::SetValue(
			const std::string &keyPath ,
			const nlohmann::json &valueJson){

			CheckInit();
			ParamChecker::checkNotEmpty(keyPath);
			//检查是否json ???

			_didManger->_idCache.Put(_didNameStr,  keyPath, valueJson);

		}

		nlohmann::json CDid::GetValue(
			const std::string &path) const {

			CheckInit();
			ParamChecker::checkNotEmpty(path);

			nlohmann::json jsonGet;
			jsonGet =_didManger-> _idCache.Get(_didNameStr, path);
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


			return _didManger->_idCache.Get(_didNameStr, keyPath);
		}

		nlohmann::json CDid::GetAllKeys(
			uint32_t start ,
			uint32_t count) const {

			CheckInit();

			return nlohmann::json();
		}

		//key password ？
		std::string CDid::Sign(
			const std::string &message){

			CheckInit();

			return _didManger->_masterWallet->Sign(_didNameStr, message, _passWord);
		}

		nlohmann::json CDid::CheckSign(
			const std::string &address ,
			const std::string &message ,
			const std::string &signature){

			CheckInit();


			CMBlock signatureData(signature.size());
			memcpy(signatureData, signature.c_str(), signature.size());

			UInt256 md;
			BRSHA256(&md, message.c_str(), message.size());

			bool r = Key::verifyByPublicKey(address, md, signatureData);
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