#!/usr/bin/env python
# -*- coding: utf-8 -*-

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
