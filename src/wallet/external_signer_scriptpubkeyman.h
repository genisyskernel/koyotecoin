// Copyright (c) 2019-2021 The Bitcoin Core developers
// Copyright (c) 2023-2023 The Koyotecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KOYOTECOIN_WALLET_EXTERNAL_SIGNER_SCRIPTPUBKEYMAN_H
#define KOYOTECOIN_WALLET_EXTERNAL_SIGNER_SCRIPTPUBKEYMAN_H

#include <wallet/scriptpubkeyman.h>

#include <memory>

namespace wallet {
class ExternalSignerScriptPubKeyMan : public DescriptorScriptPubKeyMan
{
  public:
  ExternalSignerScriptPubKeyMan(WalletStorage& storage, WalletDescriptor& descriptor)
      :   DescriptorScriptPubKeyMan(storage, descriptor)
      {}
  ExternalSignerScriptPubKeyMan(WalletStorage& storage)
      :   DescriptorScriptPubKeyMan(storage)
      {}

  /** Provide a descriptor at setup time
  * Returns false if already setup or setup fails, true if setup is successful
  */
  bool SetupDescriptor(std::unique_ptr<Descriptor>desc);

  static ExternalSigner GetExternalSigner();

  bool DisplayAddress(const CScript scriptPubKey, const ExternalSigner &signer) const;

  TransactionError FillPSBT(PartiallySignedTransaction& psbt, const PrecomputedTransactionData& txdata, int sighash_type = 1 /* SIGHASH_ALL */, bool sign = true, bool bip32derivs = false, int* n_signed = nullptr, bool finalize = true) const override;
};
} // namespace wallet
#endif // KOYOTECOIN_WALLET_EXTERNAL_SIGNER_SCRIPTPUBKEYMAN_H
