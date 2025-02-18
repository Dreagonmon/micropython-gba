help('modules')

import platform
print(platform.platform())

# mount filesystem
from _vfs import init_filesystem as __ifs
__ifs()

import os
print(os.listdir())

# import machine
# import time
# start = time.ticks_ms()
# print(start)
# while True:
#     pass

# while True:
#     machine.idle()
#     now = time.ticks_ms()
#     print(now)
#     if time.ticks_diff(now, start) > 5000:
#         machine.reset()