# -*- coding: utf-8 -*-
import alabaster as theme
import os

project = 'MNMLSTC Core'
copyright = 'MNMLSTC'

version = '2.0'
release = '2.0'

needs_sphinx = '1.3'

html_static_path = ['static']
html_theme_path = [theme.get_path()]
html_theme = 'alabaster'
html_theme_options = { 'show_powered_by' : False }
html_style = 'theme.css'
html_sidebars = { '**' : ['navigation.html'] }

highlight_language = 'cpp'
primary_domain = 'cpp'

pygments_style = 'manni'

exclude_patterns = ['_build']
templates_path = ['templates']
source_suffix = '.rst'
master_doc = 'index'

rst_prolog = '''
.. role:: cxx(code)
   :language: cpp

.. role:: cmake(code)
   :language: cmake

'''

extensions = ['sphinx.ext.todo', 'alabaster']

todo_include_todos = True
