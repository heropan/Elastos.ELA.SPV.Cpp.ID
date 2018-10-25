// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "DIDLog.h"

namespace Elastos {
	namespace DID {
#if defined(__ANDROID__)
		std::shared_ptr<spdlog::logger> DIDLog::_didConsoleLog = spdlog::android_logger("Elastos", "DID");
#else
		std::shared_ptr<spdlog::logger> DIDLog::_didConsoleLog = spdlog::stdout_color_mt("console");
#endif

	}
}