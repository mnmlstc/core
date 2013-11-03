import cloud_sptheme as csp
import os

project = 'MNMLSTC Core'
copyright = '2013, MNMLSTC'

version = '1.0'
release = '1.0'

highlight_language = 'cpp'

html_static_path = ['static']
html_theme_path = [csp.get_theme_dir()]
html_theme = 'cloud'

exclude_patterns = ['_build']
templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'index'

extensions = ['cloud_sptheme.ext.issue_tracker', 'sphinx.ext.todo']

issue_tracker_url = 'https://github.com/mnmlstc/core/issues'
todo_include_todos = True
