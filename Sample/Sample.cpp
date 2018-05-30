
#include <iostream>
#include <cassert>
#include <boost/scoped_ptr.hpp>

#include "WalletFactory.h"
#include "IdManager.h"
#include "leveldb/db.h"

using namespace Elastos::SDK;

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

	IdManager *idManager(new IdManager(idChainSubWallet));
	TestCallback *callback(new TestCallback);
	idManager->AddCallback(callback);

	nlohmann::json addresses = subWallet->GetAllAddress(0, INT_MAX);
	std::cout << "wallet addrs: " << addresses << std::endl;

	while (true) sleep(1);
}