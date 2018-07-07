// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#define CATCH_CONFIG_MAIN

#include <sstream>
#include <boost/filesystem.hpp>

#include "Log.h"
#include "IdCache.h"
#include "catch.hpp"

#include <boost/scoped_ptr.hpp>

#include "MasterWalletManager.h"
#include "Interface/ididManager.h"
#include "Interface/idid.h"
#include "Implement/didManager.h"
#include "did.h"
#include "SDK/Implement/MasterWallet.h"


using namespace Elastos::ElaWallet;
using namespace Elastos::DID;




class CDidManagerTest : public  CDidManager {
public:
	CDidManagerTest(IMasterWallet *masterWallet) : CDidManager(masterWallet, "Data"){

	}

	virtual IDID * CreateDID(const std::string &password){
		std::string didNameStr = "";
		return NewDid(didNameStr);
	}

};



class TestMasterWalletManager : public MasterWalletManager {
public:
	TestMasterWalletManager() : MasterWalletManager("Data") {
	}
};

class TestMasterWallet : public MasterWallet {
public:
	TestMasterWallet(const MasterWallet &wallet) : MasterWallet(wallet) {
	}

protected:
	virtual void startPeerManager(SubWallet *wallet) {
	}

	virtual void stopPeerManager(SubWallet *wallet) {
	}
};


const std::string IdCachePath = "IdCacheTestDB";
IMasterWallet *masterWallet = nullptr;
std::string payPassword = "idPassword";
boost::scoped_ptr<TestMasterWalletManager> masterWalletManager(new TestMasterWalletManager());

void initMasterWallet() {


	std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
	std::string phrasePassword = "";

	std::string masterWalletId = "MasterWalletId";
	masterWallet = masterWalletManager->ImportWalletWithMnemonic(masterWalletId, mnemonic, phrasePassword, payPassword);

	//masterWallet = masterWalletManager->CreateMasterWallet("123456", "english");
	masterWallet = masterWalletManager->CreateMasterWallet("123456", mnemonic , phrasePassword, payPassword , "english");
//	masterWalletManager->InitializeMasterWallet(masterWallet->GetId(), mnemonic, phrasePassword,
//												payPassword);
}

void DestroyMasterWallet(){
	if (masterWallet){
		masterWalletManager->DestroyWallet(masterWallet->GetId());
	}

}

TEST_CASE( "CDid constructor  test", "[didTest]" )
{
	//Enviroment::InitializeRootPath("Data");
	initMasterWallet();
	CDidManager* didManager = NULL;

	SECTION("CDid constructor didNameStr empty str") {
		CDidManagerTest* didManager = NULL;
		didManager = new CDidManagerTest(masterWallet);
		REQUIRE(didManager != nullptr);
		CHECK_THROWS_AS(didManager->CreateDID(payPassword), std::invalid_argument);
		delete didManager;
	}
	SECTION("CDid constructor didNameStr empty str") {
		CDidManagerTest* didManager = NULL;
		didManager = new CDidManagerTest(masterWallet);
		REQUIRE(didManager != nullptr);
		CHECK_THROWS_AS(didManager->CreateDID(payPassword), std::invalid_argument);
		delete didManager;
	}
	DestroyMasterWallet();
}

TEST_CASE( "GetDIDName  test", "[didTest]" )
{
	//Enviroment::InitializeRootPath("Data");
	initMasterWallet();
	CDidManager* didManager = NULL;

	SECTION("GetDIDName didNameStr empty str") {

		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		IDID * idID = didManager->CreateDID(payPassword);
		REQUIRE(idID != nullptr);

		std::string dIDName = "";
		dIDName = idID->GetDIDName();

		REQUIRE(dIDName != "");

		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}


TEST_CASE( "CreateDID  test", "[didTest]" )
{
	//Enviroment::InitializeRootPath("Data");
	initMasterWallet();
	CDidManager* didManager = NULL;

	SECTION("GetDIDName didNameStr empty str") {

		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		IDID * idID = didManager->CreateDID(payPassword);
		REQUIRE(idID != nullptr);

		std::string dIDName1 = "";
		dIDName1 = idID->GetDIDName();
		std::cout << "dIDName1 " << dIDName1  << std::endl;
		REQUIRE(dIDName1 != "");


		//////////////////////
		IDID * idID2 = didManager->CreateDID(payPassword);
		REQUIRE(idID2 != nullptr);

		std::string dIDName2 = "";
		dIDName2 = idID2->GetDIDName();

		REQUIRE(dIDName2 != "");
		std::cout << "dIDName2 " << dIDName2  << std::endl;
		//////////////////////
		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}


TEST_CASE( "SetValue  test", "[didTest]" )
{
	//Enviroment::InitializeRootPath("Data");
	nlohmann::json id1Json = {
		{"path1", {
					  {"101", {{"datahash", "datahash1"}, {"proof", "hello proof1"}, {"sign", "hello sign1"}}},
					  {"102", {{"datahash", "datahash2"}, {"proof", "hello proof2"}, {"sign", "hello sign2"}}},
					  {"103", {{"datahash", "datahash3"}, {"proof", "hello proof3"}, {"sign", "hello sign3"}}}
				  }},
		{"path2", {
					  {"104", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
					  {"105", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
					  {"106", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
				  }},
		{"path3", {
					  {"107", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
					  {"108", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
					  {"109", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
				  }}
	};

	initMasterWallet();
	CDidManager* didManager = NULL;

	SECTION("SetValue didNameStr empty str") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		IDID * idID = didManager->CreateDID(payPassword);
		REQUIRE(idID != nullptr);

		nlohmann::json jsonSet = {"117", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}};

		for (nlohmann::json::iterator it = id1Json.begin(); it != id1Json.end(); it++) {
			std::string tagPath = it.key();
			nlohmann::json tagValue = it.value();
			idID->SetValue(tagPath, it.value());
			std::cout << "----- " << tagValue << " ----- " << std::endl;
		}
		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}

TEST_CASE( "GetValue  test", "[didTest]" )
{
	//::InitializeRootPath("Data");
	nlohmann::json id1Json = {
		{"path1", {
					  {"101", {{"datahash", "datahash1"}, {"proof", "hello proof1"}, {"sign", "hello sign1"}}},
					  {"102", {{"datahash", "datahash2"}, {"proof", "hello proof2"}, {"sign", "hello sign2"}}},
					  {"103", {{"datahash", "datahash3"}, {"proof", "hello proof3"}, {"sign", "hello sign3"}}}
				  }},
		{"path2", {
					  {"104", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
					  {"105", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
					  {"106", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
				  }},
		{"path3", {
					  {"107", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
					  {"108", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
					  {"109", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
				  }}
	};

	initMasterWallet();
	CDidManager* didManager = NULL;

	SECTION("GetValue didNameStr empty str") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		IDID * idID = didManager->CreateDID(payPassword);
		REQUIRE(idID != nullptr);

		nlohmann::json jsonSet = {"117", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}};

		for (nlohmann::json::iterator it = id1Json.begin(); it != id1Json.end(); it++) {
			std::string tagPath = it.key();
			idID->SetValue(tagPath, it.value());
		}
		nlohmann::json path1Json = {
			{"101", {{"datahash", "datahash1"}, {"proof", "hello proof1"}, {"sign", "hello sign1"}}},
			{"102", {{"datahash", "datahash2"}, {"proof", "hello proof2"}, {"sign", "hello sign2"}}},
			{"103", {{"datahash", "datahash3"}, {"proof", "hello proof3"}, {"sign", "hello sign3"}}}
		};

		nlohmann::json jsonRet;
		jsonRet = idID->GetValue("path1");

		REQUIRE(jsonRet == path1Json);

		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}

TEST_CASE( "GetAllKeys  test", "[didTest]" )
{
	//Enviroment::InitializeRootPath("Data");
	initMasterWallet();
	CDidManager* didManager = NULL;

	//SECTION("GetDIDName didNameStr empty str") {

		nlohmann::json id1Json = {
			{"path1", {
						  {"101", {{"datahash", "datahash1"}, {"proof", "hello proof1"}, {"sign", "hello sign1"}}},
						  {"102", {{"datahash", "datahash2"}, {"proof", "hello proof2"}, {"sign", "hello sign2"}}},
						  {"103", {{"datahash", "datahash3"}, {"proof", "hello proof3"}, {"sign", "hello sign3"}}}
					  }},
			{"path2", {
						  {"104", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
						  {"105", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
						  {"106", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
					  }},
			{"path3", {
						  {"107", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
						  {"108", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
						  {"109", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
					  }}
		};

//		initMasterWallet();
//		CDidManager* didManager = NULL;

		SECTION("GetAllKeys str") {
			didManager = new CDidManager(masterWallet, "Data");
			REQUIRE(didManager != nullptr);

			IDID * idID = didManager->CreateDID(payPassword);
			REQUIRE(idID != nullptr);


			for (nlohmann::json::iterator it = id1Json.begin(); it != id1Json.end(); it++) {
				std::string tagPath = it.key();
				idID->SetValue(tagPath, it.value());
			}


			nlohmann::json jsonRet;
			jsonRet = idID->GetAllKeys(0,  100);

			jsonRet = idID->GetAllKeys(0,  100);

			nlohmann::json path1Json = {
				"",
				"path1",
				"path2",
				"path3"
			};
			std::cout << jsonRet << std::endl;
			REQUIRE(jsonRet == path1Json);

			delete didManager;
			didManager = NULL;
	}

	SECTION("didManager didNameStr empty str") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		IDID * idID = didManager->CreateDID(payPassword);
		REQUIRE(idID != nullptr);


		for (nlohmann::json::iterator it = id1Json.begin(); it != id1Json.end(); it++) {
			std::string tagPath = it.key();
			idID->SetValue(tagPath, it.value());
		}


		nlohmann::json jsonRet;
		jsonRet = idID->GetAllKeys(0,  100);



		nlohmann::json path1Json = {
			"",
			"path1",
			"path2",
			"path3"
		};
		std::cout << jsonRet << std::endl;
		REQUIRE(jsonRet == path1Json);

		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}

//TEST_CASE( "GetAllKeys  test", "[didTest]" )
//{
	//Enviroment::InitializeRootPath("Data");
//	SECTION("didManager didNameStr empty str") {
//
//		nlohmann::json id1Json = {
//			{"path1", {
//						  {"101", {{"datahash", "datahash1"}, {"proof", "hello proof1"}, {"sign", "hello sign1"}}},
//						  {"102", {{"datahash", "datahash2"}, {"proof", "hello proof2"}, {"sign", "hello sign2"}}},
//						  {"103", {{"datahash", "datahash3"}, {"proof", "hello proof3"}, {"sign", "hello sign3"}}}
//					  }},
//			{"path2", {
//						  {"104", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
//						  {"105", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
//						  {"106", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
//					  }},
//			{"path3", {
//						  {"107", {{"DataHash", "DataHash1"}, {"Proof", "hello proof1"}, {"Sign", "hello sign1"}}},
//						  {"108", {{"DataHash", "DataHash2"}, {"Proof", "hello proof2"}, {"Sign", "hello sign2"}}},
//						  {"109", {{"DataHash", "DataHash3"}, {"Proof", "hello proof3"}, {"Sign", "hello sign3"}}}
//					  }}
//		};
//
//		initMasterWallet();
//		CDidManager* didManager = NULL;
//
//		SECTION("GetAllKeys str") {
//			didManager = new CDidManager(masterWallet, "Data");
//			REQUIRE(didManager != nullptr);
//
//			IDID * idID = didManager->CreateDID(payPassword);
//			REQUIRE(idID != nullptr);
//
//
//			for (nlohmann::json::iterator it = id1Json.begin(); it != id1Json.end(); it++) {
//				std::string tagPath = it.key();
//				idID->SetValue(tagPath, it.value());
//			}
//
//
//			nlohmann::json jsonRet;
//			jsonRet = idID->GetAllKeys(0,  100);
//
//
//
//			nlohmann::json path1Json = {
//				"path1",
//				"path2",
//				"path3"
//			};
//			std::cout << jsonRet << std::endl;
//			REQUIRE(jsonRet == path1Json);
//
//			delete didManager;
//			didManager = NULL;
//		}
//
//		SECTION("didManager didNameStr empty str") {
//			didManager = new CDidManager(masterWallet, "Data");
//			REQUIRE(didManager != nullptr);
//
//			IDID * idID = didManager->CreateDID(payPassword);
//			REQUIRE(idID != nullptr);
//
//
//			for (nlohmann::json::iterator it = id1Json.begin(); it != id1Json.end(); it++) {
//				std::string tagPath = it.key();
//				idID->SetValue(tagPath, it.value());
//			}
//
//
//			nlohmann::json jsonRet;
//			jsonRet = idID->GetAllKeys(0,  100);
//
//
//
//			nlohmann::json path1Json = {
//				"path1",
//				"path2",
//				"path3"
//			};
//			std::cout << jsonRet << std::endl;
//			REQUIRE(jsonRet == path1Json);
//
//			delete didManager;
//			didManager = NULL;
//		}
//	}
//	DestroyMasterWallet();
//
//}


TEST_CASE( "GetPublicKey test", "[didTest]" )
{
	//Enviroment::InitializeRootPath("Data");
	initMasterWallet();
	CDidManager* didManager = NULL;
	SECTION("GetPublicKey normal ok") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		IDID * idID = didManager->CreateDID(payPassword);
		REQUIRE(idID != nullptr);

		std::string dIDName = "";
		dIDName = idID->GetDIDName();

		REQUIRE(dIDName != "");

		std::string strPubKey = "";
		strPubKey = idID->GetPublicKey();

		std::cout <<" strPubKey  "<<strPubKey << std::endl;
		REQUIRE(strPubKey != "");

		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}




TEST_CASE( "Sign test", "[didTest]" )
{
	//Enviroment::InitializeRootPath("Data");
	initMasterWallet();
	CDidManager* didManager = NULL;

	SECTION("Sign normal ok") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		IDID * idID = didManager->CreateDID(payPassword);
		REQUIRE(idID != nullptr);

		std::string dIDName = "";
		dIDName = idID->GetDIDName();
		REQUIRE(dIDName != "");

		std::string strPubKey = "";
		strPubKey = idID->GetPublicKey();
		//std::cout <<" strPubKey  "<<strPubKey << std::endl;
		REQUIRE(strPubKey != "");


		std::string message= "Hello World";
		std::string signMessage = "";
		signMessage = idID->Sign(message, payPassword);
		REQUIRE(signMessage != "");

		std::cout <<" signMessage  "<<signMessage << std::endl;

		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}//

//GetPublicKey

TEST_CASE( "CheckSign test", "[didTest]" )
{
	//Enviroment::InitializeRootPath("Data");
	initMasterWallet();
	CDidManager* didManager = NULL;

	SECTION("CheckSign normal ok") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		IDID * idID = didManager->CreateDID(payPassword);
		REQUIRE(idID != nullptr);

		std::string dIDName = "";
		dIDName = idID->GetDIDName();
		REQUIRE(dIDName != "");

		std::string strPubKey = "";
		strPubKey = idID->GetPublicKey();
		//std::cout <<" strPubKey  "<<strPubKey << std::endl;
		REQUIRE(strPubKey != "");


		std::string message= "Hello World";
		std::string signMsg = "";
		signMsg = idID->Sign(message, payPassword);
		REQUIRE(signMsg != "");

		std::cout <<" signMessage  "<<signMsg << std::endl;


		nlohmann::json jsonRet ;
		jsonRet = idID->CheckSign(message, signMsg);

		std::cout <<" jsonRet  "<<jsonRet << std::endl;
		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}//

