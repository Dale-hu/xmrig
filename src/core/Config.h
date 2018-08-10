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

#ifndef __CONFIG_H__
#define __CONFIG_H__


#include <stdint.h>
#include <vector>


#include "common/config/CommonConfig.h"
#include "common/xmrig.h"
#include "rapidjson/fwd.h"
#include "workers/CpuThread.h"


class Addr;
class Url;


namespace xmrig {


class ConfigLoader;
class IThread;
class IWatcherListener;


/**
 * @brief The Config class
 *
 * Options with dynamic reload:
 *   colors
 *   debug
 *   verbose
 *   custom-diff (only for new connections)
 *   api/worker-id
 *   pools/
 */
class Config : public CommonConfig
{
public:
    enum ThreadsMode {
        Automatic,
        Simple,
        Advanced
    };


    Config();
    ~Config();

    bool reload(const char *json);

    void getJSON(rapidjson::Document &doc) const override;

    inline AesMode aesMode() const                       { return m_aesMode; }
    inline AlgoVariant algoVariant() const               { return m_algoVariant; }
    inline bool isHugePages() const                      { return m_hugePages; }
    inline const std::vector<IThread *> &threads() const { return m_threads.list; }
    inline int priority() const                          { return m_priority; }
    inline int threadsCount() const                      { return m_threads.list.size(); }
    inline int64_t affinity() const                      { return m_threads.mask; }
    inline ThreadsMode threadsMode() const               { return m_threads.mode; }

    static Config *load(int argc, char **argv, IWatcherListener *listener);

protected:
    bool finalize() override;
    bool parseBoolean(int key, bool enable) override;
    bool parseString(int key, const char *arg) override;
    bool parseUint64(int key, uint64_t arg) override;
    void parseJSON(const rapidjson::Document &doc) override;

private:
    bool parseInt(int key, int arg);

    AlgoVariant getAlgoVariant() const;
#   ifndef XMRIG_NO_AEON
    AlgoVariant getAlgoVariantLite() const;
#   endif


    struct Threads
    {
       inline Threads() : mask(-1L), count(0), mode(Automatic) {}

       int64_t mask;
       size_t count;
       std::vector<CpuThread::Data> cpu;
       std::vector<IThread *> list;
       ThreadsMode mode;
    };


    AesMode m_aesMode;
    AlgoVariant m_algoVariant;
    bool m_hugePages;
    bool m_safe;
    int m_maxCpuUsage;
    int m_priority;
    Threads m_threads;
};


} /* namespace xmrig */

#endif /* __CONFIG_H__ */
