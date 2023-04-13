// Copyright (c) 2019-2021 The Bitcoin Core developers
// Copyright (c) 2023-2023 The Koyotecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KOYOTECOIN_TEST_UTIL_MINING_H
#define KOYOTECOIN_TEST_UTIL_MINING_H

#include <memory>
#include <string>
#include <vector>

class CBlock;
class CChainParams;
class CScript;
class CTxIn;
namespace node {
struct NodeContext;
} // namespace node

/** Create a blockchain, starting from genesis */
std::vector<std::shared_ptr<CBlock>> CreateBlockChain(size_t total_height, const CChainParams& params);

/** Returns the generated coin */
CTxIn MineBlock(const node::NodeContext&, const CScript& coinbase_scriptPubKey);

/** Prepare a block to be mined */
std::shared_ptr<CBlock> PrepareBlock(const node::NodeContext&, const CScript& coinbase_scriptPubKey);

/** RPC-like helper function, returns the generated coin */
CTxIn generatetoaddress(const node::NodeContext&, const std::string& address);

#endif // KOYOTECOIN_TEST_UTIL_MINING_H
