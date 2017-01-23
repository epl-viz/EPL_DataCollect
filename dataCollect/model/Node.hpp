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
 * \file Node.hpp
 * \brief Contains class Node
 * \todo IMPLEMENT
 */


#pragma once



#include "defines.hpp"
#include "OD.hpp"
#include "ODDescription.hpp"

namespace EPL_DataCollect {

/*!
 * \brief The status of the node
 * \todo IMPLEMENT
 */
enum NodeStatus { NS_OK };

/*!
  * class Node
  * \brief Representation of a Node in ethernetPOWERLINK
  */
class Node {
 public:
  // Constructors/Destructors
  //


  /*!
   * Empty Constructor
   */
  Node();

  /*!
   * Empty Destructor
   */
  virtual ~Node();

  Node(const Node &) = default;
  Node(Node &&)      = default;

  Node &operator=(const Node &) = default;
  Node &operator=(Node &&) = default;

  // Static Public attributes
  //

  // Public attributes
  //


  // Public attribute accessor methods
  //


  // Public attribute accessor methods
  //



  /*!
   * \brief Returns a pointer to the OD
   * \return OD *
   */
  OD *getOD() { return nullptr; }


  /*!
   * \brief Returns a pointer to the ODDescription object
   * \return ODDescription *
   */
  ODDescription *getODDescription() { return nullptr; }


  /*!
   * \brief Returns the status of the node
   * \return NodeStatus
   */
  NodeStatus getStatus() { return NS_OK; }

 protected:
  // Static Protected attributes
  //

  // Protected attributes
  //

 public:
  // Protected attribute accessor methods
  //

 protected:
 public:
  // Protected attribute accessor methods
  //

 protected:
 private:
  // Static Private attributes
  //

  // Private attributes
  //

  // The Object dictionary
  OD od;
  // Pointer to the ODDescription of the Node
  ODDescription odDesc;

 public:
  // Private attribute accessor methods
  //

 private:
 public:
  // Private attribute accessor methods
  //


  /*!
   * Set the value of od
   * The Object dictionary
   * \param new_var the new value of od
   */
  void setOd(OD new_var) { od = new_var; }

  /*!
   * Get the value of od
   * The Object dictionary
   * \return the value of od
   */
  OD *getOd() { return &od; }

  /*!
   * Set the value of odDesc
   * Pointer to the ODDescription of the Node
   * \param new_var the new value of odDesc
   */
  void setOdDesc(ODDescription new_var) { odDesc = new_var; }

  /*!
   * Get the value of odDesc
   * Pointer to the ODDescription of the Node
   * \return the value of odDesc
   */
  ODDescription *getOdDesc() { return &odDesc; }

 private:
};
}
