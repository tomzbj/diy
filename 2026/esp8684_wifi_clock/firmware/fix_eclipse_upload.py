Import("env")
import os

# Parallel compile (Eclipse "platformio -c eclipse" often effectively runs with low jobs).
jobs = os.cpu_count() or 4
env.SetOption("num_jobs", jobs)

# upload_flags in platformio.ini are treated as global esptool options.
# --no-progress only exists on the write-flash subcommand (esptool v5).
flags = list(env.get("UPLOADERFLAGS", []))
if "write-flash" in flags and "--no-progress" not in flags:
    i = flags.index("write-flash")
    flags.insert(i + 1, "--no-progress")
    env.Replace(UPLOADERFLAGS=flags)
