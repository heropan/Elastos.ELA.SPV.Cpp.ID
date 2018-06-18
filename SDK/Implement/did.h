// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef IDCHAIN_DID_H
#define IDCHAIN_DID_H

#include <map>


#include "nlohmann/json.hpp"
#include "Interface/idid.h"



namespace Elastos {
	namespace DID {

		class CDidManager;

		class CDid : public IDID
		{
		public:
			virtual ~CDid();

			virtual std::string GetDIDName() ;

			virtual void SetValue(
				const std::string &keyPath,
				const nlohmann::json &valueJson) ;

			virtual nlohmann::json GetValue(
				const std::string &path) const ;

			virtual nlohmann::json GetHistoryValue(
				const std::string &keyPath) const ;
			//path
			virtual nlohmann::json GetAllKeys(
				uint32_t start,
				uint32_t count) const ;

			virtual std::string Sign(
				const std::string &message) ;

			virtual nlohmann::json CheckSign(
				const std::string &address,
				const std::string &message,
				const std::string &signature) ;

			virtual std::string GetPublicKey() ;

		protected:
				CDid(CDidManager* didManager, const std::string &id) ;

		protected:
			void setValue(	const std::string &path,
							const nlohmann::json &value,
							uint32_t blockHeight);

			void DelValue(
							const std::string &path,
							uint32_t blockHeight );

			void CheckInit() const ;
		protected:
			friend class CDidManager;

			std::string _didNameStr ;
			std::string _passWord;
			CDidManager* _didManger;

		};
	}
}
#endif //IDCHAIN_DID_H
