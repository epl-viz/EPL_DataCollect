/* Copyright (c) 2017, EPL-Vizards
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the EPL-Vizards nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL EPL-Vizards BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!
 * \file Node.cpp
 * \brief Contains class Node
 */

#include "Node.hpp"
#include "EPLEnum2Str.hpp"

namespace EPL_DataCollect {

Node::~Node() {}

/*!
 * \brief Returns the status of the node
 * \return The current NodeStatus
 */
NMTState Node::getStatus() const noexcept { return status; }

std::string Node::getStatusStr() noexcept { return EPLEnum2Str::toStr(status); };

/*!
 * \brief Retrieves the identity of this node
 * \return The identity of this node
 */
Node::IDENT Node::getIdentity() const noexcept { return identity; }

/*!
 * \brief Returns the ID of the node
 * \return The node ID
 */
uint8_t Node::getID() const noexcept { return id; }

/*!
 * \brief Returns a pointer to the Object Dictionary of the node
 * \return The pointer to the OD
 */
OD *Node::getOD() noexcept { return &od; }

/*!
 * \brief Returns a pointer to the Object Dictionary Description of the Node
 * \return The pointer to the OD Description
 */
ODDescription *Node::getODDesc() noexcept { return od.getODDesc(); }

/*!
 * \brief Sets the node status
 * \param newStatus The new node status
 */
void Node::setStatus(NMTState newStatus) noexcept { status = newStatus; }

/*!
 * \brief Sets the node identity
 * \param i The new node identity
 */
void Node::setIdentity(Node::IDENT i) noexcept { identity = i; }

std::string Node::getDeviceType() noexcept { return identity.DeviceType; }

std::string Node::getIPAddress() noexcept { return identity.IPAddress; }
std::string Node::getSubnetMask() noexcept { return identity.SubnetMask; }
std::string Node::getDefaultGateway() noexcept { return identity.DefaultGateway; }
std::string Node::getHostName() noexcept { return identity.HostName; }
uint32_t    Node::getProfile() noexcept { return static_cast<uint32_t>(identity.Profile); }
uint32_t    Node::getVendorId() noexcept { return identity.VendorId; }
uint32_t    Node::getProductCode() noexcept { return identity.ProductCode; }
uint32_t    Node::getRevisionNumber() noexcept { return identity.RevisionNumber; }
uint32_t    Node::getSerialNumber() noexcept { return identity.SerialNumber; }
uint32_t    Node::getResponseTime() noexcept { return identity.ResponseTime; }
}
