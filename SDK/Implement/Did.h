//
// Created by hp on 2018/6/16.
//

#ifndef IDCHAIN_DID_H
#define IDCHAIN_DID_H

#include <map>

#include "nlohmann/json.hpp"
#include "Interface/idid.h"


namespace Elastos {
	namespace SDK {

		class CDid : public IDID
		{
		public:
			virtual ~CDid();

			virtual std::string GetDIDName() = 0;

			virtual void SetValue(
				const std::string &keyPath,
				const nlohmann::json &valueJson) = 0;

			virtual nlohmann::json GetValue(
				const std::string &path) const = 0;

			virtual nlohmann::json GetHistoryValue(
				const std::string &keyPath) const = 0;

			virtual nlohmann::json GetAllKeys(
				uint32_t start,
				uint32_t count) const = 0;

			virtual std::string Sign(
				const std::string &message) = 0;

			virtual nlohmann::json CheckSign(
				const std::string &address,
				const std::string &message,
				const std::string &signature) = 0;

		protected:
				CDid();

		protected:
			std::string _didNameStr;
		};
	}
}
#endif //IDCHAIN_DID_H
