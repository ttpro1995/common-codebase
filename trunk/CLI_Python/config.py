r'''
  Configuration file for CLI.
'''

cli_version = 'V1.00'

# define all command search path here.
cmd_search_path = ['\\system_cmd', '\\user_cmd']

cmd_file_prefix = 'cmd_'

# set CLI prompt.
cli_prompt = 'CLI'

# list all remap commands here.
cmd_remap_table = { "?"     : "help",
                    "clr"   : "cls",
                    "clear" : "cls",
                    "quit"  : "exit",
                    "dir"   : "ls",
                    "ll"    : "ls"
                  }
