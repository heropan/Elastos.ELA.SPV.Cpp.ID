// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "nlohmann/json.hpp"

#ifndef __ELASTOS_DID_ERRORCHECKER_H__
#define __ELASTOS_DID_ERRORCHECKER_H__

namespace Elastos {
	namespace DID {

		namespace Error {
			typedef enum {
				InvalidArgument = 30000,
				DIDNotFound,
				InvalidPasswd,
			} Code;
		}

		namespace Exception {
			typedef enum {
				LogicError,
				InvalidArgument,
			} Type;
		}

		class ErrorChecker {
		public:
			static nlohmann::json mkErrorJson(Error::Code code, const std::string &msg);

			static void condition(bool condition, Error::Code code, const std::string &msg,
								  Exception::Type type = Exception::LogicError);

			static void checkPassword(const std::string &password, const std::string &msg);

			static void argumentNotEmpty(const std::string &argument, const std::string &msg);

		};

	}
}

#endif // __ELASTOS_DID_ERRORCHECKER_H__