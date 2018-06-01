
#include <iostream>
#include <cassert>
#include <boost/scoped_ptr.hpp>
#include <climits>

#include "WalletFactory.h"
#include "IdManager.h"
#include "leveldb/db.h"

using namespace Elastos::SDK;

#define PURPOSE 55

class TestCallback : public IIdManagerCallback {
public:
	void OnIdStatusChanged(
			const std::string &id,
			const std::string &path,
			const nlohmann::json &value) {
		std::cout << "Id: " << id << ", path: " << path << ", value: " << value << std::endl;
	}
};

int main(int argc, char *argv[]) {
	boost::scoped_ptr<WalletFactory> walletFactory(new WalletFactory);

	std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
	std::string phrasePassword = "";
	std::string payPassword = "payPassword";
	IMasterWallet *masterWallet(
			walletFactory->ImportWalletWithMnemonic(mnemonic, phrasePassword, payPassword));

	ISubWallet *subWallet(masterWallet->CreateSubWallet(Idchain, "ELA", 0, payPassword, false));
	IIdChainSubWallet *idChainSubWallet = dynamic_cast<IIdChainSubWallet *>(subWallet);

	std::string idPassword = "idPassword";
	IdManager *idManager(new IdManager);
	std::string id;
	{
		std::string key;
		masterWallet->DeriveIdAndKeyForPurpose(PURPOSE, 0, payPassword, id, key);
		idManager->RegisterId(id, key, idPassword);
	}
	TestCallback callback;
	idManager->RegisterCallback(id, &callback, idChainSubWallet);

	nlohmann::json addresses = subWallet->GetAllAddress(0, INT_MAX);
	std::cout << "wallet addrs: " << addresses << std::endl;

	while (true) sleep(1);
}