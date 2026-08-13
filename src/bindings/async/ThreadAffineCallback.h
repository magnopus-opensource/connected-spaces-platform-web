#pragma once

#include "CallbackProxyQueue.h"
#include "emscripten/bind.h"
#include "emscripten/proxying.h"
#include "emscripten/val.h"
#include <iostream>
#include <pthread.h>

namespace bindings::async {

/*
 * Weird type that "claims" ownership of a val based callback, and provides mechanisms to retrieve
 * it if you are on the affine thread, as that is the only place that a callback may be invoked
 * in emscripten-land.
 * That's not to say callbacks can't be "called" off-thread in C++, they can
 * and are, it's just when they get into emscripten, they need to actually be executed on the
 * thread they were created on, which is what this allows.
 *
 * In this context, to be executing on an "affine thread" means that the callback is executing
 * on the same thread it was instantiated on. We use pthread id's to ensure this.
 *
 * It's not just execution, pretty much anything to do with a `val` has the same restrictions,
 * including destruction. We use the same ownership claiming mechanism to ensure that when
 * destruction runs, the decref that would trigger callback destruction only occurs on the
 * affine thread.
 *
 * This object should be constructed when the callback is REGISTERED. Ie, at the time that JS
 * has called `SetMyCallback(myCallback)` --> This call path leads to this object being constructed.
 * We _store_ the `myCallback` provided inside this object, which ends up being the callback CSP
 * actually invokes later off-thread when it receives notice (normally via a signalR message being recieved on a worker thread).
 * Via this link, storing the pthread of the registration,
 * that's howe we do the marshalling, using the emscripten proxy queue.
 */

class ThreadAffineCallback {
public:
    explicit ThreadAffineCallback(const emscripten::val& cb) : m_affineThread(pthread_self())
    {
        /*
         * This does seem weird, and it is. `val` isn't really intended to be used cross thread it seems.
         * What we're doing by copying, is invoking an internal incref (in the copy constructor) to the reference count.
         * This keeps the callback alive, even if the `val` here that carrys it falls out of scope on this thread.
         * `release_ownership()` then releases it, which disarms the `val` destructor, which means we don't end up decreffing.
         * This gives us raw ownership of the EM_VAL, extracted from the `val`. ... I wish embind had more natural way of doing this.
         *
         * We're wanting to keep the callbacks alive manually, our callbacks are long running, get called multiple times,
         * and even in await/async get called more than once depending on `inprogress` states, which is why we need to do this.
         */
        emscripten::val ownedCBCopy = cb;
        m_callbackHandle = ownedCBCopy.release_ownership();
    }

    ~ThreadAffineCallback()
    {
        /*
            * Destruction has to happen on the affine thread.
            * In theory, we could just ignore this and leak, or even just destruct if we happen to be on the correct
            * thread and ignore otherwise. But let's try to be correct anyhow and handle both cases.
            */
        if (pthread_equal(m_affineThread, pthread_self())) {
            RunCallbackHandleDestructor(m_callbackHandle);
            return;
        }

        /*
         * Okay, so we're not on the affine thread, so we have to dispatch it. Be async, don't want to block on destruction.
         * Honestly, i don't think the CSP codebase expresses this path at all at time of writing. ... But don't quote me on that!
         * Try/catch to be super paranoid ... this is a destructor afterall.
         */
        try {
            if (!CallbackProxyQueue().proxyAsync(m_affineThread, [handle = m_callbackHandle]() { RunCallbackHandleDestructor(handle); })) {
                std::cerr << "Could not queue callback handle destruction on the affine thread. Leaking the handle." << std::endl;
            }
        } catch (...) {
            std::cerr << "Exception while queueing callback handle destruction. Leaking the handle." << std::endl;
        }
    }

    ThreadAffineCallback(const ThreadAffineCallback&) = delete;
    ThreadAffineCallback& operator=(const ThreadAffineCallback&) = delete;
    ThreadAffineCallback(ThreadAffineCallback&&) = delete;
    ThreadAffineCallback& operator=(ThreadAffineCallback&&) = delete;

    bool OnAffineThread() const { return pthread_equal(m_affineThread, pthread_self()); }
    pthread_t AffineThreadId() const { return m_affineThread; }

    emscripten::EM_VAL GetCallbackHandle() const
    {
        assert(OnAffineThread() && "ThreadAffineCallback::GetCallbackHandle() called from incorrect thread.");
        return m_callbackHandle;
    }

private:
    static void RunCallbackHandleDestructor(emscripten::EM_VAL callbackHandle)
    {
        /*
         * Again, strange. We're making a `val` here, and explicitly claiming ownership of a callbackHandle.
         * this means that we're not causing an incref from callbackHandle (we did this in the constructor),
         * but we ARE arming the destructor. This means that when the val we make here falls out of scope (immediately),
         * the destructor runs! (which decrefs)
         * By doing it this way, we can only run the destructor when we're sure we're on an affine thread.
         */
        emscripten::val::take_ownership(callbackHandle);
    }

    emscripten::EM_VAL m_callbackHandle; //EM_VAL is effectively a pointer, does not cause constructor/destructor/incref/decref behaviour passing it around
    pthread_t m_affineThread; //The affine thread is the thread this object is constructed on.
};
}
