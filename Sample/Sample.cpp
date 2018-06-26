
#include <iostream>
#include <cassert>
#include <boost/scoped_ptr.hpp>
#include <climits>

#include "IdManager.h"
#include "leveldb/db.h"
#include "Interface/IMasterWallet.h"
#include "Interface/IdManagerFactory.h"

#include "MasterWalletManager.h"
#include "Interface/ididManager.h"
#include "Interface/idid.h"
#include "Implement/didManager.h"
using namespace Elastos::ElaWallet;
using namespace Elastos::DID;
#define PURPOSE 55

IMasterWallet *masterWallet = nullptr;
std::string payPassword = "payPassword";

class TestMasterWalletManager : public MasterWalletManager {
public:
	TestMasterWalletManager() : MasterWalletManager("Data") {
	}
};


class TestCallback : public IIdManagerCallback {
public:
	void OnIdStatusChanged(
			const std::string &id,
			const std::string &path,
			const nlohmann::json &value) {
		std::cout << "Id: " << id << ", path: " << path << ", value: " << value << std::endl;
	}
};

void initMasterWallet() {

	boost::scoped_ptr<TestMasterWalletManager> masterWalletManager(new TestMasterWalletManager());
	std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
	std::string phrasePassword = "";
	std::string payPassword = "idPassword";
	std::string masterWalletId = "MasterWalletId";
	//masterWallet = masterWalletManager->ImportWalletWithMnemonic(masterWalletId, mnemonic, phrasePassword, payPassword);

	masterWallet = masterWalletManager->CreateMasterWallet("123456", mnemonic , phrasePassword, payPassword , "english");


	//masterWalletManager->InitializeMasterWallet(masterWallet->GetId(), mnemonic, phrasePassword,
												//payPassword);


}

std::string registerId(IDIDManager *idManager) {
	std::string idPassword = "idPassword";//must same as  initMasterWallet  payPassword
	std::string id;
	{
		std::string key;
		//masterWallet->DeriveIdAndKeyForPurpose(PURPOSE, 0, payPassword, id, key);
		CDidManager* didManager = (CDidManager*)idManager;

		didManager->CreateDID(idPassword);

	}
	return id;
}

int main(int argc, char *argv[]) {

	//Enviroment::InitializeRootPath("Data");
	//initMasterWallet();

	IdManagerFactory idManagerFactory;

	initMasterWallet();


	IDIDManager *idManager = idManagerFactory.CreateIdManager(masterWallet, "Data");
	std::string id = registerId(idManager);
	//std::cout << "Id address: " << id << std::endl;

	//TestCallback callback;
	//idManager->RegisterCallback(id, &callback);

	while (true) sleep(1);
}