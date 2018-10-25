// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef IDCHAIN_DIDLOG_H
#define IDCHAIN_DIDLOG_H

#include "spdlog/spdlog.h"

namespace Elastos {
	namespace DID {

		class DIDLog {
		public:
			template<typename Arg1, typename... Args>
			static void trace(const std::string &fmt, const Arg1 &arg1, const Args &... args) {
				_didConsoleLog->trace(fmt.c_str(), arg1, args...);
			}

			template<typename Arg1, typename... Args>
			static void debug(const std::string &fmt, const Arg1 &arg1, const Args &... args) {
				_didConsoleLog->debug(fmt.c_str(), arg1, args...);
			}

			template<typename Arg1, typename... Args>
			static void info(const std::string &fmt, const Arg1 &arg1, const Args &... args) {
				_didConsoleLog->info(fmt.c_str(), arg1, args...);
			}

			template<typename Arg1, typename... Args>
			static void warn(const std::string &fmt, const Arg1 &arg1, const Args &... args) {
				_didConsoleLog->warn(fmt.c_str(), arg1, args...);
			}

			template<typename Arg1, typename... Args>
			static void error(const std::string &fmt, const Arg1 &arg1, const Args &... args) {
				_didConsoleLog->error(fmt.c_str(), arg1, args...);
			}

			template<typename Arg1, typename... Args>
			static void critical(const std::string &fmt, const Arg1 &arg1, const Args &... args) {
				_didConsoleLog->critical(fmt.c_str(), arg1, args...);
			}

			template<typename T>
			static void trace(const T &msg) {
				_didConsoleLog->trace(msg);
			}

			template<typename T>
			static void debug(const T &msg) {
				_didConsoleLog->trace(msg);
			}

			template<typename T>
			static void info(const T &msg) {
				_didConsoleLog->trace(msg);
			}

			template<typename T>
			static void warn(const T &msg) {
				_didConsoleLog->trace(msg);
			}

			template<typename T>
			static void error(const T &msg) {
				_didConsoleLog->trace(msg);
			}

			template<typename T>
			static void critical(const T &msg) {
				_didConsoleLog->trace(msg);
			}

			static void setLevel(spdlog::level::level_enum level) {
				_didConsoleLog->set_level(level);
			}

		private:
			static std::shared_ptr<spdlog::logger> _didConsoleLog;
		};

	}
}

#endif //IDCHAIN_DIDLOG_H
