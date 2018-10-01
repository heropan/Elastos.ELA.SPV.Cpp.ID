// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <nlohmann/json.hpp>
#include "ErrorChecker.h"

namespace Elastos {
	namespace DID {

		nlohmann::json ErrorChecker::mkErrorJson(Error::Code code, const std::string &msg) {
			nlohmann::json j;
			j["Code"] = code;
			j["Message"] = msg;
			return j;
		}

		void ErrorChecker::condition(bool condition, Elastos::DID::Error::Code code, const std::string &msg,
									 Elastos::DID::Exception::Type type) {
			if (condition) {
				nlohmann::json errJson = mkErrorJson(code, msg);

				if (type == Exception::LogicError) {
					throw std::logic_error(errJson.dump());
				} else if (type == Exception::InvalidArgument) {
					throw std::invalid_argument(errJson.dump());
				}
			}
		}

		void ErrorChecker::checkPassword(const std::string &password, const std::string &msg) {
			condition(password.size() < 8, Error::InvalidPasswd,
						   msg + " password invalid: less than 8",
						   Exception::LogicError);

			condition(password.size() > 128, Error::InvalidPasswd,
						   msg + " password invalid: more than 128",
						   Exception::LogicError);
		}

		void ErrorChecker::argumentNotEmpty(const std::string &argument, const std::string &msg) {
			condition(argument.empty(), Error::InvalidArgument, msg + " is empty");
		}

	}
}