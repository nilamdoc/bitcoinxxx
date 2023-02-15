// Copyright (c) 2011-2020 The Renaiss Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef RENAISS_QT_RENAISSADDRESSVALIDATOR_H
#define RENAISS_QT_RENAISSADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class RenaissAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit RenaissAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** Renaiss address widget validator, checks for a valid renaiss address.
 */
class RenaissAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit RenaissAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // RENAISS_QT_RENAISSADDRESSVALIDATOR_H
