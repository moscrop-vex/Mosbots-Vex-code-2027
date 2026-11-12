# Operating rules for AI Agents

## Never loop
- Never retry a command or action with the identical error. After 2 identical
  failures, STOP: change approach or ask the user.
- If a failure happens BEFORE the command runs (e.g. `NotFound: FileSystem.access`,
  cwd / no-such-file / environment errors), treat it as an ENVIRONMENT problem,
  not a command problem. Diagnose the environment; do NOT re-run the same command.
- Use the `workdir` parameter instead of `cd`. Never depend on the session cwd.

## Be careful with destructive actions
- Never rename, move, or delete project directories or files without asking first.

## Work in small, verifiable steps
- Use a todo list for multi-step tasks.
- After editing, run the build/tests and read the actual output before claiming
  success.
- Do not dump huge tool output; summarize and truncate.

## This project (VEX V5 / C++)
- Build with `make`. The project directory name contains spaces, which the VEX
  makefile rejects. Build via a temporary copy or pass `P=` rather than renaming
  the directory.
- Source lives in `src/`, headers in `include/`.
