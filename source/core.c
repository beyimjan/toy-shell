#include "core.h"
#include "builtin_commands.h"
#include "external_programs.h"
#include "debug.h"

void invoke_command(int argc, char *const *argv)
{
#if DEBUG_LEVEL >= 2
  DEBUG_TRACE_DVAR(argc);
#endif
  if (is_builtin_command(argv[0])) {
    invoke_builtin_command(argc, argv);
  }
  else {
    run_external_program(argv);
  }
}
