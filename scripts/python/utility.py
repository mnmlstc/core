# -*- coding: utf-8 -*-
# Utilities used by other scripts here.
# Many imitate common unix utilities.

from __future__ import print_function
from subprocess import PIPE, STDOUT, Popen as popen
from contextlib import contextmanager
import sys
import os

try: from urllib.request import ContentTooShortError, urlretrieve as retrieve
except: from urllib import ContentTooShortError, urlretrieve as retrieve

class DownloadError(Exception):
    def __init__ (self, path): self.path = path
    def __str__ (self): return "Error downloading '{}'".format(self.path)

class LocateError(Exception):
    def __init__ (self, path): self.path = path
    def __str__ (self): return 'Could not locate {}'.format(self.path)

class EnvironError(Exception):
    def __init__ (self, var): self.var = var
    def __str__ (self):
        return "Environment Variable '{}' not found".format(self.var)

def exit (error): sys.exit(str(error))

def download (path, name):
    try: retrieve(path, name)
    except ContentTooShortError: raise DownloadError(path)

# May not be necessary
@contextmanager
def pushd (directory):
    if not os.path.exists(directory): raise LocateError(directory)
    old = os.getcwd()
    os.chdir(directory)
    yield
    os.chdir(old)

def make_executable (path):
    if os.access(path, os.X_OK): return
    stat = os.stat(path)
    os.chmod(path, stat.st_mode | stat.S_IEXEC)

def is_executable (path): return os.access(path, os.X_OK)

# returns first path found...
def which (name):
    if sys.platform == 'win32' and not name.endswith('.exe'):
        name = '{}.exe'.format(name)
    path, _ = os.path.split(name)
    if path and is_executable(name): return name
    for path in os.environ['PATH'].split(os.pathsep):
        test = os.path.join(path, name)
        if is_executable(test): return test
    raise LocateError(name)

def execute(*command, **kwargs):
    kwargs.update(stdout=PIPE, stderr=STDOUT, universal_newlines=True)
    proc = process(command, **kwargs)
    iterator = iter(proc.stdout.readline, b'')
    while proc.poll() is None:
        for line in iterator: print(line, end='')
    code = proc.returncode
    if not code: return code
    exit(code)

def getenv (var):
    value = os.environ.get(var, None)
    if value is None: raise EnvironError(var)
    return value
