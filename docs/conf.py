# -*- coding: utf-8 -*-
import guzzle_sphinx_theme
import os

project = 'MNMLSTC Core'
copyright = 'MNMLSTC'

version = '2.0'
release = '2.0'

needs_sphinx = '1.3'

html_translate_class = 'guzzle_sphinx_theme.HTMLTranslator'
html_theme_path = guzzle_sphinx_theme.html_theme_path()
html_theme = 'guzzle_sphinx_theme'
html_theme_options = dict(project_nav_name='MNMLSTC Core')

highlight_language = 'cpp'
primary_domain = 'cpp'

pygments_style = 'manni'

exclude_patterns = ['_build']
#templates_path = ['templates']
source_suffix = '.rst'
master_doc = 'index'

rst_prolog = '''
.. role:: cxx(code)
   :language: cpp

.. role:: cmake(code)
   :language: cmake

'''

extlinks = {
    'wg21': ('https://wg21.link/%s', ''),
    'issue': ('https://github.com/mnmlstc/core/issues/%s', 'issue')
}

extensions = [
    'sphinx.ext.todo',
    'sphinx.ext.githubpages',
    'sphinx.ext.extlinks',
#    'sphinxcontrib.inlinesyntaxhighlight',
#    'sphinxcontrib.versioning'
]

todo_include_todos = True
