// Copyright (c) 2022 The Renaiss Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef RENAISS_KERNEL_CHAINSTATEMANAGER_OPTS_H
#define RENAISS_KERNEL_CHAINSTATEMANAGER_OPTS_H

#include <util/time.h>

#include <cstdint>
#include <functional>

class CChainParams;

namespace kernel {

/**
 * An options struct for `ChainstateManager`, more ergonomically referred to as
 * `ChainstateManager::Options` due to the using-declaration in
 * `ChainstateManager`.
 */
struct ChainstateManagerOpts {
    const CChainParams& chainparams;
    const std::function<NodeClock::time_point()> adjusted_time_callback{nullptr};
};

} // namespace kernel

#endif // RENAISS_KERNEL_CHAINSTATEMANAGER_OPTS_H
