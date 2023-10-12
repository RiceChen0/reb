from building import *
import os

cwd = GetCurrentDir()

src = Split('''
src/reb_broker.c
src/reb_observer.c
src/reb_publisher.c
adapter/rtthread/reb_mutex.c
adapter/rtthread/reb_sem.c
adapter/rtthread/reb_task.c
adapter/rtthread/reb_queue.c
''')

CPPPATH = [cwd + '/include']

if GetDepend(['PKG_USING_REB_EXAMPLE']):
    src += ['example/reb_rtt_example.cpp']

group = DefineGroup('reb', src, depend = [''], CPPPATH = CPPPATH)
Return('group')
