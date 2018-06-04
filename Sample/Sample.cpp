
#include <iostream>
#include <cassert>
#include <boost/scoped_ptr.hpp>
#include <climits>

#include "WalletFactory.h"
#include "IdManager.h"
#include "leveldb/db.h"
#include "Interface/IdManagerFactory.h"

using namespace Elastos::SDK;

#define PURPOSE 55

IMasterWallet *masterWallet = nullptr;
std::string payPassword = "payPassword";

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
	boost::scoped_ptr<WalletFactory> walletFactory(new WalletFactory);

	std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
	std::string phrasePassword = "";
	std::string payPassword = "payPassword";
	masterWallet = walletFactory->ImportWalletWithMnemonic(mnemonic, phrasePassword, payPassword);
}

std::string registerId(IIdManager *idManager) {
	std::string idPassword = "idPassword";
	std::string id;
	{
		std::string key;
		masterWallet->DeriveIdAndKeyForPurpose(PURPOSE, 0, payPassword, id, key);
		idManager->RegisterId(id, key, idPassword);
	}
	return id;
}

int main(int argc, char *argv[]) {

	initMasterWallet();

	std::vector<std::string> initialAddresses;
	IdManagerFactory idManagerFactory;
	IIdManager *idManager = idManagerFactory.CreateIdManager(initialAddresses);
	std::string id = registerId(idManager);
	std::cout << "Id address: " << id << std::endl;

	TestCallback callback;
	idManager->RegisterCallback(id, &callback);

	while (true) sleep(1);
}