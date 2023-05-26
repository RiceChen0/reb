from building import *
import os

cwd = GetCurrentDir()

src = Split('''
src/revent_bus.c
''')

CPPPATH = [cwd + '/include']

if GetDepend(['PKG_USING_EVENTBUS_EXAMPLE']):
    src += ['example/reb_rtt_example.cpp']

group = DefineGroup('revent_bus', src, depend = [''], CPPPATH = CPPPATH)
Return('group')
