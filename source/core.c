#include "core.h"
#include "builtin_commands.h"
#include "external_programs.h"

void invoke_command(int argc, char *const *argv)
{
  if (is_builtin_command(argv[0])) {
    invoke_builtin_command(argc, argv);
  }
  else {
    run_external_program(argv);
  }
}
