#!  /usr/bin/env python
# 
# Copyright (C) 2009  The University of Texas at Austin.
# 
# This file is part of Hydra: A wireless multihop testbed.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 

"""
Finite state machine class; contains `FSM` and `ParamEvent` classes.

This file contains classes for running a finite state machine with asynchronous
events that can be triggered externally.

Timing
======

Timing is implemented using the python time module. This is only accurate up to
the millisecond level. Further modifications are needed for higher precision
timing.

:author: Ketan Mandke <mandke@ece.utexas.edu>

:var FSM_VERBOSE:
    Constant enumeration to control verbose thresholds in this file.
  
    Setting the verbose level of a `FSM` above this threshold will cause the
    corresponding output in this file to be written (or logged).
"""

import threading
from threading import _Event, Thread, Event

import sys
import time

FSM_VERBOSE=14

class ParamEvent(_Event):
    """
    Parameterized Python `Event`.

    This can be used as a normal `Event`. It can also be used to pass a
    parameter to the waiting `Thread` when calling `set()`.

    :IVariables:
     * `__param`: Private variable to maintain parameters set while calling
       `set(). Access this member through `param`.

    :CVariables:
     * `param`: Property for `__param`
    """
    def __init__(self, *args, **kwargs):
        """Constructor."""
        _Event.__init__(self, *args, **kwargs)
        self.__param = None

    param = property(lambda self: self.__param)

    def set(self, param=None):
        """
        Wakeup all Threads waiting on this `Event`.

        :param param: Optional parameter that will be assigned to `param`.
        """
        self.__param = param
        return _Event.set(self)

    def clear(self):
        """Overloaded method also resets `param` to `None`."""
        self.__param = None
        return _Event.clear(self)

class FSM(Thread):
    """
    Finite State Machine.

    This class can be used to run a series of states, where each state is
    defined by a method. Use `goto()` to transition to another state from inside
    of a state method. All state methods must take the calling `FSM` as an
    argument.

    :IVariables:
     * `verbose`: Verbose level of output.

     * `__state`: Private member to maintain current state.

     * `__nextstate`: Private `ParamEvent` used to manage state transitions.

    :CVariables:
     * `nextstate`: Property for `__nextstate`.

     * `state`: Property for `__state`.

     * `rstate`: Property for retrieving readable representation of `__state`.
    """
    def __init__(self, name="FSM", initstate=None, \
                 daemon=True, verbose=None, **kwargs):
        """
        Constructor.

        :param name: Name of underlying Thread.
        :param initstate: Initial state.
        :param daemon: Boolean; if True, `Thread` will run as a Daemon.
        :param verbose: Verbose level of output.

        Initial state can be set through the parameter above or by using calling
        `goto()` before calling `start()`.
        """
        self.verbose = verbose
        self.__state = initstate
        self.__nextstate = ParamEvent()
        Thread.__init__(self, name=name)
        self.setDaemon(daemon)

    def __del__(self):
        """Destructor; call `halt()`."""
        self.halt()

    def _get_state(self, readable=False):
        """
        Internal method to get current state.

        :param readable: Boolean; if True, return readable representation of
                         current `state`.
        :returns: Current state.

        :note: Use the properties `state` and `rstate` instead of using this
               method directly.
        """
        s = self.__state
        if readable:
            if hasattr(self.__state, 'im_func'): s = self.__state.im_func
            if hasattr(s, 'func_name'): s = s.func_name
        return s

    # Properties
    state  = property(fget=_get_state)
    rstate = property(fget=lambda self: self._get_state(readable=True) )
    nextstate = property(fget=lambda self: self.__nextstate)

    def run(self):
        """Manage execution of `FSM` by monitoring `nextstate`."""
        assert isinstance(self.nextstate, ParamEvent), \
               "[FSM] Nextstate event must be a ParamEvent!"
        # state should have been initialized before calling start()
        args, kwargs = (), {}
        if self.nextstate.isSet():
            self.__state, args, kwargs = self.nextstate.param
        if self.verbose>FSM_VERBOSE: self.debug()
        # run state - initial state can't take parameters
        while callable(self.state):
            # reset nextstate
            self.nextstate.clear()
            # start new state
            self.state(self, *args, **kwargs)
            # wait for state transition
            self.nextstate.wait()
            assert self.nextstate.isSet(), \
                    "[FSM]: Nextstate signalled unexpectedly!"
            self.__state, args, kwargs = self.nextstate.param
            if self.verbose>FSM_VERBOSE: self.debug()

    def debug(self, s=None, force=False):
        """
        Convenience method for logging debug information.

        :param s:     Debug string to log.
        :param force: Boolean; if False, only output debug information if
                      `verbose` level is greater than `FSM_VERBOSE`.
        """
        ok = force
        ok |= (self.verbose>FSM_VERBOSE)
        if not ok: return
        # assemble debug message
        d = "%.6f: "%(time.time() )
        d += "%s"%(self)
        if s is not None: d += " -- %s\t"%(s)
        d += "\n"
        sys.stderr.write(d)

    def goto(self, ns, *args, **kwargs):
        """
        Transition to next state.

        :param ns:     Next state to transition to must be a callable method.
        :param args:   Additional arguments to pass to `ns`.
        :param kwargs: Addition keywords to pass to `ns.

        :note: Multiple simultaneous calls to `goto()`, will be ignored. Only
               the first call will be executed.
        """
        if (not self.nextstate.isSet() ) or (ns is None):
            self.nextstate.set((ns, args, kwargs) )
        return ns

    def halt(self):
        """Halt execution; completes the current state and the ignores any more
        attempts to `goto()` a  new state."""
        self.goto(None)

    def waitonevents(self, *args, **kwargs):
        """
        Wait on one or more events with the aid of `EventWaiters`.

        :param args: One or more Event or ParamEvent objects to be waited on.
        :param kwargs: Additional keyword arguments to be used while waiting.

        The only keyword that this method uses is 'timeout'. If 'timeout' is
        specified, this method will use the timeout while waiting on the events.
        """
        timeout = None
        if 'timeout' in kwargs: timeout = kwargs['timeout']
        done = Event()
        done.clear()
        # Spawn waiters
        for e in [a for a in args if isinstance(a, _Event)]:
            w = EventWaiter(name="waiter", wait=e, done=done)
            w.start()
        done.wait(timeout)
        return done.isSet()

    def __str__(self):
        """String representation of `FSM`."""
        return "%s(%s)"%(self.getName(), self.rstate)

class EventWaiter(FSM):
    def __init__(self, name="event_waiter", wait=None, done=None, \
                 initstate=None, daemon=True, **kwargs):
        """
        Constructor

        :param wait: Event to wait on.
        :param done: Event to set if `wait` has occurred.
        """
        initstate = self.WAIT
        daemon = True
        FSM.__init__(self, name=name, initstate=initstate, daemon=True, **kwargs)
        self.__wait = wait
        self.__done = done

    # Properties
    wait = property(lambda self: self.__wait)
    done = property(lambda self: self.__done)

    def WAIT(self, fsm):
        if isinstance(self.wait, _Event):
            self.wait.wait()
            if self.wait.isSet() and isinstance(self.done, _Event):
                self.done.set()
        return fsm.halt()
