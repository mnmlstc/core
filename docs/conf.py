import alabaster as theme
import os

project = 'MNMLSTC Core'
copyright = 'MNMLSTC'

version = '1.2'
release = '1.2'

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
'''

extensions = ['sphinx.ext.todo', 'alabaster']

todo_include_todos = True
