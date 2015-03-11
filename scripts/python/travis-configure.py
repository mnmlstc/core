#!/usr/bin/env python
# -*- coding: utf-8 -*-

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
        use_libcxx = getenv('USE_LIBCXX')
        version = getenv('PACKAGE')
        cxx = getenv('CXX')
    except EnvironError as e: exit(e)
    current = getcwd()
    build = join(current, 'build')
    arguments = [
        current,
        '-DCMAKE_CXX_COMPILER:STRING={}-{}'.format(cxx, version),
        '-DCMAKE_BUILD_TYPE:STRING={}'.format(build_type),
        '-DBUILD_WITH_LIBCXX:BOOL={}'.format(use_libcxx),
        '-DBUILD_TESTING:BOOL=ON'
    ]
    try: arguments.insert(0, which('cmake'))
    except LocateError as e: exit(e)
    try: mkdir('build')
    except OSError as e: exit(e)
    execute(*arguments, cwd=build)
