import subprocess
import os

Import("env")

def before_build(source, target, env):
    app_name = env['PIOENV']
    result = subprocess.run(['python3', 'generate_build_flags.py', app_name], capture_output=True, text=True)
    build_flags = result.stdout.strip()
    env.Append(CPPPATH=build_flags.split())

env.AddPreAction("build", before_build)