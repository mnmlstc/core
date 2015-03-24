#!/usr/bin/env python
# -*- coding: utf-8 -*-

# travis-failure.py - build script
# Written in 2015 by MNMLSTC
# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty. You should have
# received a copy of the CC0 Public Domain Dedication along with this software.
# If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

from __future__ import print_function
from os.path import getcwd
from os.path import join
from os import walk

def find (name, path):
    return [join(root, name) for root, _, files in walk(path) if name in files]

if __name__ == '__main__':
    for path in find('LastTest.log', getcwd()):
        with open(path) as log:
            for line in log:
                print(line, end='')
