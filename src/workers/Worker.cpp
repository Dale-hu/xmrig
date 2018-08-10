/* XMRig
 * Copyright 2014      imonero <https://github.com/global-blockchain>
 * Copyright 2016-2017 XMRig       <admin@imonero.org>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <chrono>


#include "common/Platform.h"
#include "Cpu.h"
#include "workers/CpuThread.h"
#include "workers/Handle.h"
#include "workers/Worker.h"


Worker::Worker(Handle *handle) :
    m_id(handle->threadId()),
    m_totalWays(handle->totalWays()),
    m_offset(handle->offset()),
    m_hashCount(0),
    m_timestamp(0),
    m_count(0),
    m_sequence(0),
    m_thread(static_cast<xmrig::CpuThread *>(handle->config()))
{
    if (Cpu::threads() > 1 && m_thread->affinity() != -1L) {
        Platform::setThreadAffinity(m_thread->affinity());
    }

    Platform::setThreadPriority(m_thread->priority());
}


void Worker::storeStats()
{
    using namespace std::chrono;

    const uint64_t timestamp = time_point_cast<milliseconds>(high_resolution_clock::now()).time_since_epoch().count();
    m_hashCount.store(m_count, std::memory_order_relaxed);
    m_timestamp.store(timestamp, std::memory_order_relaxed);
}
