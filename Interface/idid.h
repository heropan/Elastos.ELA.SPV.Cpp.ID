// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_IDID_H__
#define __ELASTOS_SDK_IDID_H__


namespace Elastos {
	namespace SDK {

		class IDID {
		public:

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

		};

	}
}

#endif //__ELASTOS_SDK_IDMANAGERFACTORY_H__
