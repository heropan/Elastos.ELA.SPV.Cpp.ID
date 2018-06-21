//
// Created by baidu on 2018/6/16.
//
#include <map>
#include "nlohmann/json.hpp"

#ifndef IDCHAIN_IDIDCHECKER_H
#define IDCHAIN_IDIDCHECKER_H

namespace Elastos {
	namespace DID {

		class IDIDChecker{
		public:

			virtual nlohmann::json GetValue(
				const std::string &did ,
				const std::string &keyPath)  = 0;

			virtual nlohmann::json GetHistoryValue(
				const std::string &did ,
				const std::string &keyPath)  = 0;

			virtual nlohmann::json GetAllKeys(
				const std::string &did ,
				uint32_t start ,
				uint32_t count)   = 0;

			virtual std::string GetPublicKey(
				const std::string &did) = 0;

			virtual nlohmann::json CheckSign(
				const std::string &did ,
				const std::string &publicKey ,
				const std::string &message ,
				const std::string &signature) = 0;

			virtual nlohmann::json CheckDID(
				const std::string &did ,
				const std::string &nonce ,
				const std::string &appid ,
				const std::string &declaration,
				const std::string &password ) = 0;
		};

	}
}

#endif //IDCHAIN_IDIDCHECKER_H
