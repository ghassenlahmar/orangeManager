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
#include "ipcalculator.h"

///this class does all the IP range calculations
///borrowed from the internet


const QString IpCalculator::classA = QString("A");
const QString IpCalculator::classB = QString("B");
const QString IpCalculator::classC = QString("C");
const QString IpCalculator::classUndefined = QString("U");

IpCalculator::IpCalculator(const QString &addressInput) :
    addressInput(addressInput),
    ipAddress(0),
    prefixLength(0),
    calculatedSubnetMask(0xFFFFFFFF)
{
    QStringList inputList = addressInput.split("/", QString::SkipEmptyParts);

    if (inputList.size() == 0) {
        return;
    }

    ipAddress = toIPv4Address(inputList[0]);


    if (inputList.size() == 2) {
        prefixLength = inputList[1].toInt();
    } else {
        prefixLength = defaultPrefixLengthFromAddress(ipAddress);
    }
    calculatedSubnetMask <<= (32-prefixLength);
}

QString IpCalculator::getAddressInput()
{
    return addressInput;
}

QString IpCalculator::getBroadcast()
{
    return toString(broadcast());
}

QString IpCalculator::getHostsPerNetwork()
{
    return QString::number(hostspernet());
}

QString IpCalculator::getNetwork()
{
    return toString(network());
}

QString IpCalculator::getNetworkClass()
{
    QString networkClass;
    if (ipAddress < 0x7fffffff) {
        networkClass = classA;
    } else if (ipAddress < 0xbfffffff) {
        networkClass = classB;
    } else if (ipAddress < 0xdfffffff) {
        networkClass = classC;
    } else {
        networkClass = classUndefined;
    }
    return networkClass;
}

QString IpCalculator::getMaxHost()
{
    if(hostspernet()==0){return QString("no available hosts");}
    if (broadcast()==network()){return toString(broadcast());}
    return toString(maxhost());
}

QString IpCalculator::getMinHost()
{
    if(hostspernet()==0){return QString("no available hosts");}
    if (broadcast()==network()){return toString(broadcast());}
    return toString(minhost());
}

int IpCalculator::getPrefixLength()
{
    return prefixLength;
}

QString IpCalculator::getSubnetMask()
{
    return toString(calculatedSubnetMask);
}

QString IpCalculator::getWildcard()
{
    return toString(wildcard());
}

quint32 IpCalculator::broadcast()
{
    return network() | wildcard();
}

quint32 IpCalculator::hostspernet()
{
    if (broadcast()==network()){return 1;}
    return broadcast()-network()-1;
}

quint32 IpCalculator::maxhost()
{
    return broadcast()-1;
}

quint32 IpCalculator::minhost()
{

    return network()+1;
}

quint32 IpCalculator::wildcard()
{
    return ~calculatedSubnetMask;
}

quint32 IpCalculator::network()
{
    return ipAddress & mask();
}

quint32 IpCalculator::mask()
{
    return calculatedSubnetMask;
}

int IpCalculator::defaultPrefixLengthFromAddress(quint32 address)
{
    int prefixLength = 0;
    // Set prefix based upon network class
    if ((address & 0x80000000) == 0x00000000) {
        // Class A = 0 000
        prefixLength = 8;
    } else if ((address & 0xC0000000) == 0x80000000) {
        // Class B = 10 00
        prefixLength = 16;
    } else if ((address & 0xE0000000) == 0xC0000000) {
        // Class C = 110 0
        prefixLength = 24;
    } else {
        // Nothing valid?
        prefixLength = 0;
    }
    return prefixLength;
}

QString IpCalculator::toString(quint32 address)
{
    QStringList parts;

    for (int i = 0; i < 4; i++) {
        quint32 value = 0x000000FF & address;
        parts.push_front(QString::number(value));
        address >>= 8;
    }
    
    return parts.join(QString("."));
}

quint32 IpCalculator::toIPv4Address(const QString &address)
{
    QStringList quads = address.split('.', QString::KeepEmptyParts);
    if (quads.size() != 4) return -1;

    quint32 ipV4Address = 0;
    for (int i = 0; i < 4; i++) {
        quint32 byteVal = quads[i].toInt();
        ipV4Address |= (byteVal << (24-8*i));
    }
    return ipV4Address;
}
