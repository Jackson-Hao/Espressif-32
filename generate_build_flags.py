import os
import sys

def get_include_dirs(base_dir, app_name):
    include_dirs = []
    app_dir = os.path.join(base_dir, app_name)
    for root, dirs, files in os.walk(app_dir):
        if 'include' in dirs:
            include_dirs.append(os.path.join(root, 'include'))
    return include_dirs

def generate_build_flags(include_dirs):
    build_flags = []
    for dir in include_dirs:
        build_flags.append(f'-I {dir}')
    return ' '.join(build_flags)

def is_included_in_cmake(base_dir, app_name):
    cmake_file = os.path.join(base_dir, 'CMakeLists.txt')
    if not os.path.exists(cmake_file):
        return False

    with open(cmake_file, 'r') as file:
        lines = file.readlines()
        for line in lines:
            if f'include({os.path.join(base_dir, app_name, "CMakeLists.txt")})' in line and not line.strip().startswith('#'):
                return True
    return False

if __name__ == "__main__":
    base_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src', 'application', 'wifi_iot', 'app')
    app_name = sys.argv[1]

    if not is_included_in_cmake(base_dir, app_name):
        print('')
        sys.exit(0)

    include_dirs = get_include_dirs(base_dir, app_name)
    build_flags = generate_build_flags(include_dirs)
    print(build_flags)