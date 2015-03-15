#!/usr/bin/env python
# -*- coding: utf-8 -*-

# travis-build.py - build script
# Written in 2015 by MNMLSTC
# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty. You should have
# received a copy of the CC0 Public Domain Dedication along with this software.
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.


from __future__ import print_function
from utility import LocateError
from utility import execute
from utility import which
from utility import exit

if __name__ == '__main__':
    try: execute(which('cmake'), '--build', 'build', '--target', 'check')
    except LocateError as e: exit(e)
