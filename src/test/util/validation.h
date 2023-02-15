// Copyright (c) 2020 The Renaiss Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef RENAISS_TEST_UTIL_VALIDATION_H
#define RENAISS_TEST_UTIL_VALIDATION_H

#include <validation.h>

class CValidationInterface;

struct TestChainState : public Chainstate {
    /** Reset the ibd cache to its initial state */
    void ResetIbd();
    /** Toggle IsInitialBlockDownload from true to false */
    void JumpOutOfIbd();
};

class ValidationInterfaceTest
{
public:
    static void BlockConnected(CValidationInterface& obj, const std::shared_ptr<const CBlock>& block, const CBlockIndex* pindex);
};

#endif // RENAISS_TEST_UTIL_VALIDATION_H
