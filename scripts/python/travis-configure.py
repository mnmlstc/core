#!/usr/bin/env python
# -*- coding: utf-8 -*-

# travis-configure.py - build script
# Written in 2015 by MNMLSTC
# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty. You should have
# received a copy of the CC0 Public Domain Dedication along with this software.
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

from __future__ import print_function
from utility import EnvironError
from utility import LocateError
from utility import execute
from utility import getenv
from utility import which
from utility import exit
from os.path import join
from os import getcwd
from os import mkdir

if __name__ == '__main__':
    try:
        build_type = getenv('BUILD_TYPE')
        version = getenv('PACKAGE')
        cxx = getenv('CXX')
    except EnvironError as e: exit(e)
    current = getcwd()
    build = join(current, 'build')
    arguments = [
        current,
        '-DCMAKE_CXX_COMPILER:STRING={}-{}'.format(cxx, version),
        '-DCMAKE_BUILD_TYPE:STRING={}'.format(build_type),
        '-DBUILD_WITH_LIBCXX:BOOL=OFF',
        '-DBUILD_TESTING:BOOL=ON'
    ]
    try: arguments.insert(0, which('cmake'))
    except LocateError as e: exit(e)
    try: mkdir('build')
    except OSError as e: exit(e)
    execute(*arguments, cwd=build)
