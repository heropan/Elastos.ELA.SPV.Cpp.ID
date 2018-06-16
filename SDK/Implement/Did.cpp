//
// Created by hp on 2018/6/16.
//
#include "Did.h"

namespace Elastos {
	namespace SDK {

		CDid::CDid(){

		}

		CDid::~CDid(){

		}

		std::string CDid::GetDIDName(){

			return std::string();
		}

		void CDid::SetValue(
			const std::string &keyPath,
			const nlohmann::json &valueJson){


		}

		nlohmann::json CDid::GetValue(
			const std::string &path) const {
			return nlohmann::json() ;
		}

		nlohmann::json CDid::GetHistoryValue(
			const std::string &keyPath) const {
			return nlohmann::json();
		}

		nlohmann::json CDid::GetAllKeys(
			uint32_t start,
			uint32_t count) const {
			return nlohmann::json();
		}

		std::string CDid::Sign(
			const std::string &message){

			return std::string();
		}

		nlohmann::json CDid::CheckSign(
			const std::string &address,
			const std::string &message,
			const std::string &signature){

			return nlohmann::json();
		}


	}
}