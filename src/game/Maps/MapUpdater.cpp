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

#include "MapUpdater.h"
#include "MapWorkers.h"

//MapUpdater::MapUpdater(size_t num_threads) : _cancelationToken(false), pending_requests(0)
//{
//    for (size_t i = 0; i < num_threads; ++i)
//        _workerThreads.push_back(std::thread(&MapUpdater::WorkerThread, this));
//}
MapUpdater::~MapUpdater()
{
    if (activated())
        deactivate();
}

void MapUpdater::activate(size_t num_threads)
{
    if (activated())
        return;

    //spawn instances & battlegrounds threads
    for (size_t i = 0; i < num_threads; ++i)
        _loop_workerThreads.push_back(std::thread(&MapUpdater::LoopWorkerThread, this, &_enable_updates_loop));

    //continents threads are spawned later when request are received

    //for (size_t i = 0; i < num_threads; ++i)
    //    _workerThreads.push_back(std::thread(&MapUpdater::WorkerThread, this));
}

void MapUpdater::deactivate()
{
    _cancelationToken = true;

    _loop_queue.Cancel();
    _once_queue.Cancel();

    waitUpdateOnces();
    waitUpdateLoops();

    for (auto& thread : _once_workerThreads)
        thread.join();

    _once_workerThreads.clear();

    for (auto& thread : _loop_workerThreads)
        thread.join();

    _loop_workerThreads.clear();
}

void MapUpdater::waitUpdateOnces()
{
    std::unique_lock<std::mutex> lock(_lock);

    while (pending_once_requests > 0)
        _onces_condition.wait(lock);

    lock.unlock();
}

void MapUpdater::enableUpdateLoop(bool enable)
{
    _enable_updates_loop = enable;
}

void MapUpdater::waitUpdateLoops()
{
    std::unique_lock<std::mutex> lock(_lock);

    while (pending_loop_requests > 0)
        _loops_condition.wait(lock);

    lock.unlock();
}

void MapUpdater::spawnMissingOnceUpdateThreads()
{
    for (uint32 i = _once_workerThreads.size(); i < pending_once_requests; i++)
        _once_workerThreads.push_back(std::thread(&MapUpdater::OnceWorkerThread, this));
}

//void MapUpdater::join()
//{
//    wait();
//    deactivate();
//}

bool MapUpdater::activated()
{
    return _loop_workerThreads.size() > 0;
}

void MapUpdater::schedule_update(Map& map, Worker* worker)
{
    std::lock_guard<std::mutex> lock(_lock);

    // MapInstanced re schedule the instances it contains by itself, so we want to call it only once
    // Also currently test maps needs to be updated once per world update
    bool useLagMitigation = false;
    if (useLagMitigation && map.Instanceable() 
#ifdef ENABLE_PLAYERBOTS
        && map.HasRealPlayers()
#endif
        )
    {
        pending_loop_requests++;
        _loop_queue.Push(std::move(worker));
    }
    else
    {
        pending_once_requests++;
        _once_queue.Push(std::move(worker));
    }

    spawnMissingOnceUpdateThreads();
}

void MapUpdater::LoopWorkerThread(std::atomic<bool>* enable_instance_updates_loop)
{
    while (true)
    {
        Worker* request = nullptr;

        _loop_queue.WaitAndPop(request);

        if (_cancelationToken)
        {
            if (request)
                loopMapFinished();
            return;
        }

        MANGOS_ASSERT(request);
        request->execute();

        //repush at end of queue, or delete if loop has been disabled by MapManager
        if (!(*enable_instance_updates_loop))
        {
            delete request;
            loopMapFinished();
        }
        else {
            _loop_queue.Push(std::move(request));
        }
    }
}

void MapUpdater::OnceWorkerThread()
{
    while (true)
    {
        Worker* request = nullptr;

        _once_queue.WaitAndPop(request);

        if (_cancelationToken)
        {
            if (request)
                onceMapFinished();
            return;
        }

        MANGOS_ASSERT(request);
        request->execute();

        delete request;
        onceMapFinished();
    }
}

void MapUpdater::onceMapFinished()
{
    std::lock_guard<std::mutex> lock(_lock);
    MANGOS_ASSERT(pending_once_requests > 0);
    --pending_once_requests;
    if (pending_once_requests == 0)
        _onces_condition.notify_all();
}

void MapUpdater::loopMapFinished()
{
    std::lock_guard<std::mutex> lock(_lock);
    MANGOS_ASSERT(pending_loop_requests > 0);
    --pending_loop_requests;
    if (pending_loop_requests == 0)
        _loops_condition.notify_all();
}