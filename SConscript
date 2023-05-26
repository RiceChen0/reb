from building import *
import os

cwd = GetCurrentDir()

src = Split('''
src/revent_bus.c
src/rthread_pool.c
platform/rtthread/pf_task_adapter.c
platform/rtthread/pf_mutex_adapter.c
platform/rtthread/pf_sem_adapter.c
platform/rtthread/pf_event_adapter.c
''')

CPPPATH = [cwd + '/include']
CPPPATH += [cwd + '/platform']

if GetDepend(['PKG_USING_EVENTBUS_EXAMPLE']):
    src += ['example/reb_rtt_example.cpp']

group = DefineGroup('revent_bus', src, depend = [''], CPPPATH = CPPPATH)
Return('group')
