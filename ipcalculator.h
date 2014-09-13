/*
 * Copyright (C) 2012 Matthew Haley
 * 
 * This file is part of Ipcalc.
 * 
 * Ipcalc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Ipcalc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Ipcalc.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef IPCALCULATOR_H
#define IPCALCULATOR_H

#include <QString>
#include <QStringList>

class IpCalculator
{
public:
    static const QString classA;
    static const QString classB;
    static const QString classC;
    static const QString classUndefined;

public:
    IpCalculator(const QString &addressInput);

    QString getAddressInput();
    int getPrefixLength();
    QString getBroadcast();
    QString getHostsPerNetwork();
    QString getMaxHost();
    QString getMinHost();
    QString getNetwork();
    QString getNetworkClass();
    QString getSubnetMask();
    QString getWildcard();

private:
    QString addressInput;
    quint32 ipAddress;
    int prefixLength;
    quint32 calculatedSubnetMask;

private:
    quint32 broadcast();
    quint32 hostspernet();
    quint32 mask();
    quint32 maxhost();
    quint32 minhost();
    quint32 network();
    quint32 wildcard();

    int defaultPrefixLengthFromAddress(quint32 address);
    QString toString(quint32 address);
    quint32 toIPv4Address(const QString &address);
};

#endif // IPCALCULATOR_H
