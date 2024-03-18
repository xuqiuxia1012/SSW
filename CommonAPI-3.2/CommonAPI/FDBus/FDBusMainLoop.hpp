/*
 * China Entry Infotainment Project
 * Copyright (c) 2023 FAW-VW, P-VC & MOSI-Tech.
 * All Rights Reserved.
 */

#ifndef COMMONAPI_FDBUS_MAIN_LOOP_HPP_
#define COMMONAPI_FDBUS_MAIN_LOOP_HPP_

#if !defined (COMMONAPI_INTERNAL_COMPILATION)
#define COMMONAPI_INTERNAL_COMPILATION
#endif

#include <CommonAPI/MainLoopContext.hpp>

#include <condition_variable>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <vector>
#include <atomic>

namespace CommonAPI {
namespace FDBus {

typedef pollfd FDBusMainLoopPollFd;

class FDBusMainLoop {
 public:
    FDBusMainLoop() = delete;
    FDBusMainLoop(const FDBusMainLoop&) = delete;
    FDBusMainLoop& operator=(const FDBusMainLoop&) = delete;
    FDBusMainLoop(FDBusMainLoop&&) = delete;
    FDBusMainLoop& operator=(FDBusMainLoop&&) = delete;

    COMMONAPI_EXPORT explicit FDBusMainLoop(std::shared_ptr<MainLoopContext> context);
    COMMONAPI_EXPORT ~FDBusMainLoop();

    /**
     * \brief Runs the mainloop indefinitely until stop() is called.
     *
     * Runs the mainloop indefinitely until stop() is called. The given timeout (milliseconds)
     * will be overridden if a timeout-event is present that defines an earlier ready time.
     */
    COMMONAPI_EXPORT void run(const int64_t& timeoutInterval = TIMEOUT_INFINITE);
    COMMONAPI_EXPORT void stop();

    /**
     * \brief Executes a single cycle of the mainloop.
     *
     * Subsequently calls prepare(), poll(), check() and, if necessary, dispatch().
     * The given timeout (milliseconds) represents the maximum time
     * this iteration will remain in the poll state. All other steps
     * are handled in a non-blocking way. Note however that a source
     * might claim to have infinite amounts of data to dispatch.
     * This demo-implementation of a Mainloop will dispatch a source
     * until it no longer claims to have data to dispatch.
     * Dispatch will not be called if no sources, watches and timeouts
     * claim to be ready during the check()-phase.
     *
     * @param timeout The maximum poll-timeout for this iteration.
     */
    COMMONAPI_EXPORT void doSingleIteration(const int64_t& timeout = TIMEOUT_INFINITE);

    /*
     * The given timeout is a maximum timeout in ms, measured from the current time in the future
     * (a value of 0 means "no timeout"). It will be overridden if a timeout-event is present
     * that defines an earlier ready time.
     */
    COMMONAPI_EXPORT bool prepare(const int64_t& timeout = TIMEOUT_INFINITE);
    COMMONAPI_EXPORT void poll();
    COMMONAPI_EXPORT bool check();
    COMMONAPI_EXPORT void dispatch();

 private:
    COMMONAPI_EXPORT void wakeup();
    COMMONAPI_EXPORT void wakeupAck();

    COMMONAPI_EXPORT void cleanup();

    COMMONAPI_EXPORT void registerFileDescriptor(const FDBusMainLoopPollFd& fileDescriptor);
    COMMONAPI_EXPORT void unregisterFileDescriptor(const FDBusMainLoopPollFd& fileDescriptor);

    COMMONAPI_EXPORT void registerDispatchSource(DispatchSource* dispatchSource, const DispatchPriority dispatchPriority);
    COMMONAPI_EXPORT void unregisterDispatchSource(DispatchSource* dispatchSource);

    COMMONAPI_EXPORT void registerWatch(Watch* watch, const DispatchPriority dispatchPriority);
    COMMONAPI_EXPORT void unregisterWatch(Watch* watch);

    COMMONAPI_EXPORT void registerTimeout(Timeout* timeout, const DispatchPriority dispatchPriority);
    COMMONAPI_EXPORT void unregisterTimeout(Timeout* timeout);


    std::shared_ptr<MainLoopContext> context_;

    std::vector<FDBusMainLoopPollFd> managedFileDescriptors_;
    std::mutex fileDescriptorsMutex_;

    struct DispatchSourceToDispatchStruct {
        DispatchSource* dispatchSource_;
        std::atomic<bool> isExecuted_; /* execution flag: indicates, whether the dispatchSource is dispatched currently */
        std::atomic<bool> deleteObject_; /* delete flag: indicates, whether the dispatchSource can be deleted*/

        DispatchSourceToDispatchStruct(DispatchSource* _dispatchSource,
            bool _isExecuted,
            bool _deleteObject) {
                dispatchSource_ = _dispatchSource;
                isExecuted_ = _isExecuted;
                deleteObject_ = _deleteObject;
        }
    };

    struct TimeoutToDispatchStruct {
        Timeout* timeout_;
        std::atomic<bool> isExecuted_; /* execution flag: indicates, whether the timeout is dispatched currently */
        std::atomic<bool> deleteObject_; /* delete flag: indicates, whether the timeout can be deleted*/
        std::atomic<bool> timeoutElapsed_; /* timeout elapsed flag: indicates, whether the timeout is elapsed*/

        TimeoutToDispatchStruct(Timeout* _timeout,
            bool _isExecuted,
            bool _deleteObject,
            bool _timeoutElapsed) {
                timeout_ = _timeout;
                isExecuted_ = _isExecuted;
                deleteObject_ = _deleteObject;
                timeoutElapsed_ = _timeoutElapsed;
        }
    };

    struct WatchToDispatchStruct {
        int fd_;
        Watch* watch_;
        std::atomic<bool> isExecuted_; /* execution flag: indicates, whether the watch is dispatched currently */
        std::atomic<bool> deleteObject_; /* delete flag: indicates, whether the watch can be deleted*/

        WatchToDispatchStruct(int _fd,
            Watch* _watch,
            bool _isExecuted,
            bool _deleteObject) {
                fd_ = _fd;
                watch_ = _watch;
                isExecuted_ = _isExecuted;
                deleteObject_ = _deleteObject;
        }
    };

    std::multimap<DispatchPriority, DispatchSourceToDispatchStruct*> registeredDispatchSources_;
    std::multimap<DispatchPriority, WatchToDispatchStruct*> registeredWatches_;
    std::multimap<DispatchPriority, TimeoutToDispatchStruct*> registeredTimeouts_;

    std::recursive_mutex dispatchSourcesMutex_;
    std::mutex watchesMutex_;
    std::mutex timeoutsMutex_;

    std::set<std::pair<DispatchPriority, DispatchSourceToDispatchStruct*>> sourcesToDispatch_;
    std::set<std::pair<DispatchPriority, WatchToDispatchStruct*>> watchesToDispatch_;
    std::set<std::pair<DispatchPriority, TimeoutToDispatchStruct*>> timeoutsToDispatch_;

    DispatchSourceListenerSubscription dispatchSourceListenerSubscription_;
    WatchListenerSubscription watchListenerSubscription_;
    TimeoutSourceListenerSubscription timeoutSourceListenerSubscription_;
    WakeupListenerSubscription wakeupListenerSubscription_;

    int64_t currentMinimalTimeoutInterval_;

    FDBusMainLoopPollFd wakeFd_;

#ifdef _WIN32
    FDBusMainLoopPollFd sendFd_;
#endif

    std::atomic<bool> hasToStop_;
    std::atomic<bool> isBroken_;
};

} // namespace FDBus
} // namespace CommonAPI

#endif // COMMONAPI_DEMO_MAIN_LOOP_HPP_
