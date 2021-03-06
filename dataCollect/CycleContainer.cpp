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
 * \file CycleContainer.cpp
 * \brief Contains class CycleContainer
 */


#include "CycleContainer.hpp"
#include "CaptureInstance.hpp"


namespace EPL_DataCollect {

CycleContainer::CycleContainer(CaptureInstance *ci) { parent = ci; }

CycleContainer::~CycleContainer() {}

/*!
   * \brief Returns the specified cycle
   * \throws When the cycle does not exist
   * \return The cycle with the given number
   * \param  cycleNum The ID of the cycle to get
   */
Cycle CycleContainer::getCycle(uint32_t cycleNum) noexcept {
  Cycle worker = parent->getSnapshotManager()->getClosestCycle(cycleNum);

  if (cycleNum == worker.getCycleNum())
    return worker;


  CycleBuilder builder(parent);
  return builder.seekCycle(cycleNum, std::move(worker));
}


/*!
 * \brief Returns the current cycle
 * \return The current Cycle
 */
Cycle CycleContainer::pollCycle() const noexcept { return parent->getCycleBuilder()->getCurrentCycle(); }

/*!
 * \brief Retruns a wrapper object for a Cycle pointer
 * The returned object locks the CycleBuilder until it is destroyed
 */
CycleBuilder::Locker CycleContainer::pollCyclePTR() noexcept { return parent->getCycleBuilder()->getCurrentCyclePTR(); }
}
