/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _MAP_UPDATER_H_INCLUDED
#define _MAP_UPDATER_H_INCLUDED

#include "Platform/Define.h"
#include "Util/ProducerConsumerQueue.h"

#include <mutex>
#include <thread>
#include <atomic>
#include <vector>
#include <condition_variable>

class Worker;

class MapUpdater
{
    public:
        MapUpdater() : _cancelationToken(false), _enable_updates_loop(false), pending_once_requests(0), pending_loop_requests(0) {}
        MapUpdater(size_t num_threads);
        MapUpdater(const MapUpdater&) = delete;
        ~MapUpdater();
        
        void activate(size_t num_threads);
        void deactivate();
        void wait();
        void join();
        bool activated();
        void update_finished();
        void schedule_update(Map& map, Worker* worker);

        void waitUpdateOnces();
        void enableUpdateLoop(bool enable);
        void waitUpdateLoops();

    private:
        void onceMapFinished();
        void loopMapFinished();

        //this will ensure once_map_workerThreads match the pending_once_maps count
        void spawnMissingOnceUpdateThreads();

        ProducerConsumerQueue<Worker *> _loop_queue;
        ProducerConsumerQueue<Worker *> _once_queue;

        std::vector<std::thread> _loop_workerThreads;
        std::vector<std::thread> _once_workerThreads;
        std::atomic<bool> _cancelationToken;
        std::atomic<bool> _enable_updates_loop;

        std::mutex _lock;
        std::condition_variable _loops_condition; //notified when an update loop request is finished
        std::condition_variable _onces_condition; //notified when an update once request is finished
        size_t pending_loop_requests;
        size_t pending_once_requests;

        /* Loop workers keep running and processing _loop_queue, updating maps and requeuing them afterwards.
        When onceMapsFinished becomes true, the worker finish the current request and delete the request instead of requeuing it.
        */
        void LoopWorkerThread(std::atomic<bool>* onceMapsFinished);
        //Single update, descrease pending_once_maps when done
        void OnceWorkerThread();
};

#endif //_MAP_UPDATER_H_INCLUDED