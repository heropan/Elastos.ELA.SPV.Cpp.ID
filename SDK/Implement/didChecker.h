// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef IDCHAIN_DIDCHECKER_H
#define IDCHAIN_DIDCHECKER_H
#include <map>


#include "nlohmann/json.hpp"
#include "Interface/ididChecker.h"
#include "didManager.h"

namespace Elastos {
	namespace DID {

		class didChecker : public IDIDChecker {
		public:

			didChecker(CDidManager* didManager) ;

			virtual ~didChecker();

			virtual nlohmann::json GetValue(
				const std::string &did ,
				const std::string &keyPath) ;

			virtual nlohmann::json GetHistoryValue(
				const std::string &did ,
				const std::string &keyPath);

			virtual nlohmann::json GetAllKeys(
				const std::string &did ,
				uint32_t start ,
				uint32_t count) ;

			virtual std::string GetPublicKey(const std::string &did
			);

			virtual nlohmann::json CheckSign(
				const std::string &did ,
				const std::string &publicKey ,
				const std::string &message ,
				const std::string &signature) ;

			virtual nlohmann::json CheckDID(
				const std::string &did ,
				const std::string &nonce ,
				const std::string &appid ,
				const std::string &declaration,
				const std::string &password
			) ;

		protected:
			CDidManager* _pDidManager;
		};
	}
}

#endif //IDCHAIN_DIDCHECKER_H
