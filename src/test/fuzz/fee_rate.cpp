// Copyright (c) 2020-2021 The Bitcoin Core developers
// Copyright (c) 2023-2023 The Koyotecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <consensus/amount.h>
#include <policy/feerate.h>
#include <test/fuzz/FuzzedDataProvider.h>
#include <test/fuzz/fuzz.h>
#include <test/fuzz/util.h>

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

FUZZ_TARGET(fee_rate)
{
    FuzzedDataProvider fuzzed_data_provider(buffer.data(), buffer.size());
    const CAmount howlers_per_k = ConsumeMoney(fuzzed_data_provider);
    const CFeeRate fee_rate{howlers_per_k};

    (void)fee_rate.GetFeePerK();
    const auto bytes = fuzzed_data_provider.ConsumeIntegral<uint32_t>();
    if (!MultiplicationOverflow(int64_t{bytes}, howlers_per_k)) {
        (void)fee_rate.GetFee(bytes);
    }
    (void)fee_rate.ToString();

    const CAmount another_howlers_per_k = ConsumeMoney(fuzzed_data_provider);
    CFeeRate larger_fee_rate{another_howlers_per_k};
    larger_fee_rate += fee_rate;
    if (howlers_per_k != 0 && another_howlers_per_k != 0) {
        assert(fee_rate < larger_fee_rate);
        assert(!(fee_rate > larger_fee_rate));
        assert(!(fee_rate == larger_fee_rate));
        assert(fee_rate <= larger_fee_rate);
        assert(!(fee_rate >= larger_fee_rate));
        assert(fee_rate != larger_fee_rate);
    }
}
