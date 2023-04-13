// Copyright (c) 2022 The Bitcoin Core developers
// Copyright (c) 2023-2023 The Koyotecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <node/mempool_persist_args.h>

#include <fs.h>
#include <util/system.h>
#include <validation.h>

namespace node {

bool ShouldPersistMempool(const ArgsManager& argsman)
{
    return argsman.GetBoolArg("-persistmempool", DEFAULT_PERSIST_MEMPOOL);
}

fs::path MempoolPath(const ArgsManager& argsman)
{
    return argsman.GetDataDirNet() / "mempool.dat";
}

} // namespace node
