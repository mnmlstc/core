#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from utility import LocateError
from utility import execute
from utility import which
from utility import exit

if __name__ == '__main__':
    try: execute(which('cmake'), '--build', 'build', '--target', 'check')
    except LocateError as e: exit(e)
