import sphinx_rtd_theme as theme
import os

project = 'MNMLSTC Core'
copyright = '2013 - 2014, MNMLSTC'

version = '1.1'
release = '1.1'

highlight_language = 'cpp'

html_static_path = ['static']
html_theme_path = [theme.get_html_theme_path()]
html_theme = 'sphinx_rtd_theme'

exclude_patterns = ['_build']
templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'index'

extensions = ['sphinx.ext.todo']

todo_include_todos = True
