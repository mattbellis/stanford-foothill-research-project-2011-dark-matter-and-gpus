#!/usr/bin/env python

import sys
import random 

num_entries = int(sys.argv[1])

lo = 0.0
hi = 11.0

num_range = hi - lo

print num_entries

for i in xrange(num_entries):
    num = num_range*(random.random()) + lo
    print num

