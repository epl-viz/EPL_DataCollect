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
 * \file EventBase.hpp
 * \brief Contains class EventBase
 * \todo IMPLEMENT
 */


#pragma once



#include "defines.hpp"
#include <unordered_map>

namespace EPL_DataCollect {

/*!
 * \brief The type of an event
 * \todo IMPLEMENT
 */
enum EvType { EVT_ERROR };

/*!
  * class EventBase
  * \brief Base class for all events
  */
class EventBase {
 public:
  // Constructors/Destructors
  //


  /*!
   * Empty Constructor
   */
  EventBase();

  /*!
   * Empty Destructor
   */
  virtual ~EventBase();

  EventBase(const EventBase &) = default;
  EventBase(EventBase &&)      = default;

  EventBase &operator=(const EventBase &) = default;
  EventBase &operator=(EventBase &&) = default;

  // Static Public attributes
  //

  // Public attributes
  //


  // Public attribute accessor methods
  //


  // Public attribute accessor methods
  //



  /*!
   * Get the type of the event
   * \return EvType
   */
  EvType getType() const { return EVT_ERROR; }


  /*!
   * Get the name of the Event
   * \return std::string
   */
  std::string getName() const { return ""; }


  /*!
   * Returns the description of the event
   * \return std::string
   */
  std::string getDescription() const { return ""; }


  /*!
   * Returns the event type flags
   * \return unsigned long int
   */
  unsigned long int getEventFlags() const { return 0; }


  /*!
   * \param  first The first cycle where the event occurred
   * \param  last The last cycle where the event occurred
   */
  void getCycleRange(unsigned int *first, unsigned int *last) {
    *first = 0;
    *last  = 0;
  }


  /*!
   * Returns a list / map of affected OD indecies by this event. Also contains a
   * descriptive std::string
   * TODO: use a vector of tuples instead of an unordered_map?
   * \return unordered_map<int, std::string>
   */
  std::unordered_map<int, std::string> getAffectedIndices() const { return std::unordered_map<int, std::string>(); }


  /*!
   * Returns a pointer to Event specific metadata
   * Returns nullptr when no data is specified or the child class does not overwrite
   * this function
   *
   * \return unordered_map<std::string,std::string>*
   */
  virtual std::unordered_map<std::string, std::string> *getMetadata() const { return nullptr; }


  /*!
   * \brief Returns the ID of the plugin triggering the event
   * \return std::string
   */
  std::string getPluginID() { return ""; }


  /*!
   * \brief Updates the cycle range
   * \note -1 represents no change
   * \param  first The first cycle
   * \param  last The last cycle
   */
  void updateRange(int first = -1, int last = -1) {
    (void)first;
    (void)last;
  }

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
  /*!
   * This constructor must be called by a child class
   * \param  type The event type
   * \param  name The name of the event
   * \param  desc The description
   * \param  flags Flags for the new event \sa EvFlags
   */
  EventBase(EvType type, std::string name, std::string desc, unsigned long int flags) {
    (void)type;
    (void)name;
    (void)desc;
    (void)flags;
  }

 private:
  // Static Private attributes
  //

  // Private attributes
  //

 public:
  // Private attribute accessor methods
  //

 private:
 public:
  // Private attribute accessor methods
  //

 private:
};
}
