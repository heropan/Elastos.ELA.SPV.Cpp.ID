// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
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
#include "SDK/Common/Log.h"
#include "SDK/Implement/MasterWallet.h"

using namespace Elastos::ElaWallet;
using namespace Elastos::DID;







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
boost::scoped_ptr<TestMasterWalletManager> masterWalletManager(new TestMasterWalletManager());
IMasterWallet *masterWallet = nullptr;
std::string payPassword = "idPassword";

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

std::string registerId(IDIDManager *idManager) {
	std::string idPassword = "idPassword";//must same as  initMasterWallet  payPassword
	std::string id;
	{
		std::string key;
		CDidManager* didManager = (CDidManager*)idManager;
		didManager->CreateDID(idPassword);
	}
	return id;
}

TEST_CASE( "GetAllKeys func test", "[didManagerTest]" )
{
	//Enviroment::InitializeRootPath("Data");

	initMasterWallet();
	CDidManager* didManager = NULL;

	SECTION("CreateDID") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		nlohmann::json allPathJson;
		allPathJson = didManager->GetDIDList();
		Log::getLogger()->info("GetAllKeys func test1 ----------->allPathJson {}", allPathJson.dump());

		//IDID * idID = didManager->CreateDID(payPassword);
		//REQUIRE(idID != nullptr);
//		int lz = 0;
//
//		int lc = 1;
//		lc = lc /lz;
		allPathJson = didManager->GetDIDList();

		Log::getLogger()->info("GetAllKeys func test 2----------->allPathJson {}", allPathJson.dump());
		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}

TEST_CASE( "didManagerTest construct func test", "[didManagerTest]" )
{
	//Enviroment::InitializeRootPath("Data");
	initMasterWallet();
	CDidManager* didManager = NULL;

	SECTION("new CDidManager masterWallet ok") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);
		delete didManager;
		didManager = NULL;
	}
	SECTION("new CDidManager masterWallet NULL") {
		IMasterWallet *masterWallet2 = nullptr;
		CHECK_THROWS_AS(new CDidManager(masterWallet2, "Data"), std::invalid_argument);
	}
	DestroyMasterWallet();

}

TEST_CASE( "CreateDID func test", "[didManagerTest]" )
{
	//Enviroment::InitializeRootPath("Data");

	initMasterWallet();
	CDidManager* didManager = NULL;

	SECTION("CreateDID") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);
		IDID * idID = didManager->CreateDID(payPassword);
		REQUIRE(idID != nullptr);
		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}


TEST_CASE( "GetDID func test", "[didManagerTest]" )
{
	//Enviroment::InitializeRootPath("Data");

	initMasterWallet();
	CDidManager* didManager = NULL;


	SECTION("GetDID") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);
		IDID * idID = didManager->CreateDID(payPassword);
		REQUIRE(idID != nullptr);
		delete didManager;
		didManager = NULL;
	}

	SECTION("GetDID twice") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);
		IDID * idID = didManager->CreateDID(payPassword);
		REQUIRE(idID != nullptr);

		std::string dIDName = "";
		dIDName = idID->GetDIDName();

		REQUIRE(dIDName != "");

		IDID * idID2 = didManager->GetDID(dIDName);

		REQUIRE(idID == idID2);
		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();
}

TEST_CASE( "DestoryDID func test", "[didManagerTest]" )
{
	//Enviroment::InitializeRootPath("Data");

	initMasterWallet();
	CDidManager* didManager = NULL;

	SECTION("DestoryDID") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);
		IDID * idID = didManager->CreateDID(payPassword);
		REQUIRE(idID != nullptr);

		std::string dIDName = "";
		dIDName = idID->GetDIDName();

		REQUIRE(dIDName != "");
		CHECK_NOTHROW(didManager->DestoryDID(dIDName));
		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}

//

TEST_CASE( "GetDIDList func test", "[didManagerTest]" )
{
	//Enviroment::InitializeRootPath("Data");

	initMasterWallet();
	CDidManager* didManager = NULL;

	SECTION("GetDIDList") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		CHECK_NOTHROW(didManager->GetDIDList());
		
//		IDID * idID = didManager->CreateDID(payPassword);
//		REQUIRE(idID != nullptr);
//
//		std::string dIDName = "";
//		dIDName = idID->GetDIDName();
//
//		REQUIRE(dIDName != "");
//		CHECK_NOTHROW(didManager->DestoryDID(dIDName));
		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}



TEST_CASE( "simulate android app", "[didManagerTest]" )
{
	//Enviroment::InitializeRootPath("Data");

	initMasterWallet();
	CDidManager* didManager = NULL;

	SECTION("GetDIDList") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		nlohmann::json didListJson = didManager->GetDIDList();

		std::cout<<"didListJson1 "<<didListJson <<std::endl;

		IDID * idID = didManager->CreateDID(payPassword);

		didListJson = didManager->GetDIDList();
		std::cout<<"didListJson2 "<<didListJson <<std::endl;
		//CHECK_NOTHROW(didManager->GetDIDList());

//		IDID * idID = didManager->CreateDID(payPassword);
//		REQUIRE(idID != nullptr);
//
//		std::string dIDName = "";
//		dIDName = idID->GetDIDName();
//
//		REQUIRE(dIDName != "");
//		CHECK_NOTHROW(didManager->DestoryDID(dIDName));
		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}

class ManagerCallback : public IIdManagerCallback {
public:
	ManagerCallback()  {
	}
	virtual void OnIdStatusChanged(
		const std::string &id,
		const std::string &path,
		const nlohmann::json &value){

		Log::getLogger()->info("ManagerCallback::OnIdStatusChanged id = {} path {} value {} begin"
						 , id, path ,  value.dump());
	}
};

TEST_CASE( "RegisterCallback", "[didManagerTest]" )
{
	initMasterWallet();
	CDidManager* didManager = NULL;
	ManagerCallback loCallback1;
	ManagerCallback loCallback2;
	//ManagerCallback loCallback3;
	SECTION("RegisterCallback normal") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		nlohmann::json didListJson = didManager->GetDIDList();


		IDID * idID1 = didManager->CreateDID(payPassword);
		std::string did1Name = idID1->GetDIDName() ;

		didManager->RegisterCallback(did1Name, &loCallback1);

		//didListJson = didManager->GetDIDList();
		std::cout<<"didListJson2 "<<didListJson <<std::endl;

		delete didManager;
		didManager = NULL;
	}
	SECTION("RegisterCallback twice") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		nlohmann::json didListJson = didManager->GetDIDList();


		IDID * idID1 = didManager->CreateDID(payPassword);
		std::string did1Name = idID1->GetDIDName() ;

		didManager->RegisterCallback(did1Name, &loCallback1);
		didManager->RegisterCallback(did1Name, &loCallback1);
		//didListJson = didManager->GetDIDList();
		std::cout<<"didListJson2 "<<didListJson <<std::endl;

		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}

TEST_CASE( "UnregisterCallback", "[didManagerTest]" )
{
	initMasterWallet();
	CDidManager* didManager = NULL;
	ManagerCallback loCallback1;
	ManagerCallback loCallback2;
	//ManagerCallback loCallback3;
	SECTION("UnregisterCallback normal") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		nlohmann::json didListJson = didManager->GetDIDList();


		IDID * idID1 = didManager->CreateDID(payPassword);
		std::string did1Name = idID1->GetDIDName() ;

		didManager->RegisterCallback(did1Name, &loCallback1);

		didManager->UnregisterCallback(did1Name);

		//didListJson = didManager->GetDIDList();
		std::cout<<"didListJson2 "<<didListJson <<std::endl;

		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}

TEST_CASE( "OnTransactionStatusChanged", "[didManagerTest]" )
{
	initMasterWallet();
	CDidManager* didManager = NULL;
	ManagerCallback loCallback1;
	ManagerCallback loCallback2;
	//ManagerCallback loCallback3;

	SECTION("GetDIDList") {
		didManager = new CDidManager(masterWallet, "Data");
		REQUIRE(didManager != nullptr);

		nlohmann::json didListJson = didManager->GetDIDList();


		IDID * idID1 = didManager->CreateDID(payPassword);
		std::string did1Name = idID1->GetDIDName() ;

		IDID * idID2 = didManager->CreateDID(payPassword);
		std::string did1Name2 = idID2->GetDIDName() ;

		didManager->RegisterCallback(did1Name, &loCallback1);
		didManager->RegisterCallback(did1Name2, &loCallback2);

		const std::string status = "ok";

		nlohmann::json defJson ={
			{"121", {{"datahash", "datahash1"}, {"proof", "hello proof1"}, {"sign", "hello sign1"}}},
			{"132", {{"datahash", "datahash2"}, {"proof", "hello proof2"}, {"sign", "hello sign2"}}},
			{"103", {{"datahash", "datahash3"}, {"proof", "hello proof3"}, {"sign", "hello sign3"}}}
		};

		nlohmann::json defJson2 ={
			{"221", {{"datahash", "datahash1"}, {"proof", "hello proof1"}, {"sign", "hello sign1"}}},
			{"232", {{"datahash", "datahash2"}, {"proof", "hello proof2"}, {"sign", "hello sign2"}}},
			{"203", {{"datahash", "datahash3"}, {"proof", "hello proof3"}, {"sign", "hello sign3"}}}
		};
		std::cout<<"did1Name "<<did1Name <<std::endl;
		std::cout<<"did1Name2 "<<did1Name2 <<std::endl;

		didManager->OnTransactionStatusChanged(did1Name, status, defJson, 101);
		didManager->OnTransactionStatusChanged(did1Name2, status, defJson2, 102);

		didManager->UnregisterCallback(did1Name);
		didManager->UnregisterCallback(did1Name2);


		delete didManager;
		didManager = NULL;
	}
	DestroyMasterWallet();

}