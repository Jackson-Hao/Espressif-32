import os
import re
import argparse
from colorama import init, Fore, Style

# 初始化 colorama
init(autoreset=True)

def parse_cmake_file(cmake_file):
    with open(cmake_file, 'r') as file:
        lines = file.readlines()
    
    include_dirs = []
    for line in lines:
        line = line.strip()
        if line.startswith('#'):
            continue
        match = re.search(r'\$\{CMAKE_SOURCE_DIR\}/src/application/wifi_iot/app/([^/]+)/CMakeLists.txt', line)
        if match:
            dir_name = match.group(1)
            include_dirs.append(dir_name)
    
    return include_dirs

def generate_platformio_ini(include_dirs, ini_file):
    with open(ini_file, 'w') as file:  # 使用 'w' 模式覆盖文件内容
        for dir in include_dirs:
            file.write("[platformio]\n")
            file.write("build_dir = src/build\n\n")
            file.write(f"[env:{dir}]\n")
            file.write("platform = espressif32\n")
            file.write("board = esp32-s3-devkitc-1\n")
            file.write("framework = espidf\n")
            file.write("build_flags =\n")
            file.write("    -DBOARD_HAS_PSRAM\n")
            file.write("    ${common.build_flags}\n")
            file.write("board_build.partitions = src/setting/partition.csv\n")
            file.write("board_build.flash_mode = qio\n")
            file.write("extra_scripts = pre:extra_script.py\n\n")
            file.write("\n")
            file.write("[common]\n")
            file.write("build_flags =\n")

def select():
    cmake_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src','CMakeLists.txt')
    ini_file = 'platformio.ini'
    
    if not os.path.exists(cmake_file):
        print(Fore.RED + f"{cmake_file} does not exist.")
        return
    
    include_dirs = parse_cmake_file(cmake_file)
    if not include_dirs:
        print(Fore.RED + "[Error]: No include directories found.")
        return
    # 如果存在多个 include 目录，问用户选择一个
    if len(include_dirs) >= 1:
        print(Fore.GREEN + "\nMultiple include directories found.")
        print(Fore.GREEN + "-----------------------------")
        for i, dir in enumerate(include_dirs):
            print(Fore.CYAN + f"{i + 1}: {dir}")
        print(Fore.GREEN + "-----------------------------")
        index = int(input(Fore.BLUE + "Please select one: ")) - 1
        include_dirs = [include_dirs[index]]
    generate_platformio_ini(include_dirs, ini_file)
    print(Fore.GREEN + f"\n[Notice]: Selected service directory: {include_dirs[0]}")
    print(Fore.GREEN + "[Notice]: \"platformio.ini\" has been updated successfully.\n")

def clean_linux():
    build_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src', 'build')
    if os.path.exists(build_dir):
        os.system(f'rm -rf {build_dir}')
        print(Fore.GREEN + f"[Notice]: {build_dir} has been removed successfully.")
    else:
        print(Fore.RED + f"[Error]: {build_dir} does not exist.")

def clean_windows():
    build_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src', 'build')
    if os.path.exists(build_dir):
        os.system(f'rmdir /s /q {build_dir}')
        print(Fore.GREEN + f"[Notice]: {build_dir} has been removed successfully.")
    else:
        print(Fore.RED + f"[Error]: {build_dir} does not exist.")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Manage script")
    parser.add_argument('--select', action='store_true', help="Execute the select function")
    parser.add_argument('--clean', action='store_true', help="Clean all the build files")
    parser.add_argument('--version', action='store_true', help= 'Show Version')

    args = parser.parse_args()
    if args.select:
        select()
    elif args.clean:
        if os.name == 'nt':
            clean_windows()
        elif os.name == 'posix':
            clean_linux()
        else:
            print(Fore.RED + "Unsupported operating system")
    elif args.version:
        if os.name == 'nt':
            print(Fore.GREEN + "OS: Windows Platform")
        elif os.name == 'posix':
            print(Fore.GREEN + "OS: Linux Platform")
        else:
            print(Fore.RED + "OS: Unsupported")
        print(Fore.GREEN + "Version: 1.0")
    else:
        print(Fore.RED + "No arguments provided.")
        parser.print_help()