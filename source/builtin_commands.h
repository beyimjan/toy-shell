#ifndef BUILTIN_COMMANDS_H_SENTRY
#define BUILTIN_COMMANDS_H_SENTRY

int is_builtin_command(const char *cmd);
void invoke_builtin_command(int argc, char *const *argv);

#endif
