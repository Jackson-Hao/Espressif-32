import os, sys
import shutil
import json
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
            file.write("monitor_speed = 115200\n")
            file.write("extra_scripts = pre:extra_script.py\n\n")
            file.write("\n")
            file.write("[common]\n")
            file.write("build_flags =\n")

'''
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
'''

def select(service_name=None):
    services_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src', 'Services.json')
    cmake_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src', 'CMakeLists.txt')
    ini_file = 'platformio.ini'
    
    if not os.path.exists(services_file):
        print(Fore.RED + f"{services_file} does not exist.")
        return
    
    with open(services_file, 'r') as file:
        services = json.load(file)
    
    if not services:
        print(Fore.RED + "[Error]: No services found.")
        return
    
    service_names = list(services.keys())
    
    if service_name and service_name in services:
        selected_service = service_name
    else:
        print(Fore.RED + f"[Error]: Service \"{service_name}\" not found.")
        if len(service_names) >= 1:
            print(Fore.GREEN + "\nMultiple services found.")
            print(Fore.GREEN + "-----------------------------")
            for i, name in enumerate(service_names):
                print(Fore.CYAN + f"{i + 1}: {name}")
            print(Fore.GREEN + "-----------------------------")
            index = int(input(Fore.BLUE + "Please select one: ")) - 1
            selected_service = service_names[index]
        else:
            print(Fore.RED + "[Error]: No services available for selection.")
            return
    
    selected_dir = services[selected_service]['dir']
    
    with open(cmake_file, 'w') as file:
        file.write(f"include(\n    {selected_dir}\n)")
    
    include_dirs = parse_cmake_file(cmake_file)
    generate_platformio_ini(include_dirs, ini_file)
    
    print(Fore.GREEN + f"\n[Notice]: Selected service: {selected_service}")
    print(Fore.GREEN + "[Notice]: \"CMakeLists.txt\" has been updated successfully.\n")

def list_services():
    services_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src', 'Services.json')
    cmake_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src', 'CMakeLists.txt')
    
    if not os.path.exists(services_file):
        print(Fore.RED + f"{services_file} does not exist.")
        return
    
    with open(services_file, 'r') as file:
        services = json.load(file)
    
    if not services:
        print(Fore.RED + "[Error]: No services found.")
        return
    
    included_services = []
    if os.path.exists(cmake_file):
        with open(cmake_file, 'r') as file:
            cmake_content = file.read()
            for service in services.keys():
                if service in cmake_content:
                    included_services.append(service)
    
    print(Fore.GREEN + "\nServices found:")
    print(Fore.GREEN + "-----------------------------")
    for name in services.keys():
        if name in included_services:
            print(Fore.YELLOW + f"-> {name}")
        else:
            print(Fore.CYAN + f"{name}")
    print(Fore.GREEN + "-----------------------------\n")

def clean_linux():
    build_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src', 'build')
    if os.path.exists(build_dir):
        os.system(f'rm -rf {build_dir}/*')
        print(Fore.GREEN + f"[Notice]: {build_dir} has been removed successfully.")
    else:
        print(Fore.RED + f"[Error]: {build_dir} does not exist.")

def clean_windows():
    build_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src', 'build')
    if os.path.exists(build_dir):
        for root, dirs, files in os.walk(build_dir):
            for file in files:
                os.remove(os.path.join(root, file))
                print(Fore.GREEN + f"[Notice]: {file} has been removed successfully.")
            for dir in dirs:
                shutil.rmtree(os.path.join(root, dir))
                print(Fore.GREEN + f"[Notice]: {dir} has been removed successfully.")
        print(Fore.GREEN + f"[Notice]: {build_dir} has been removed successfully.")
    else:
        print(Fore.RED + f"[Error]: {build_dir} does not exist.")

def create_new_directory(dirname):
    base_path = os.path.join('src', 'application', 'wifi_iot', 'app', dirname)
    include_path = os.path.join(base_path, 'include')
    main_c_path = os.path.join(base_path, 'main.c')
    main_h_path = os.path.join(include_path, 'main.h')
    cmake_file_path = os.path.join(base_path, 'CMakeLists.txt')
    services_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src', 'Services.json')

    os.makedirs(include_path, exist_ok=True)

    with open(main_c_path, 'w') as main_c_file:
        main_c_file.write(f'''#include <stdio.h>
#include "main.h"

void app_main() {{
    printf("Hello, {dirname}!\\n");
}}
''')

    with open(main_h_path, 'w') as main_h_file:
        main_h_file.write(f'''#ifndef __MAIN_H__
#define __MAIN_H__

void app_main();

#endif''')
    with open(cmake_file_path, 'w') as cmake_file:
        cmake_file.write(f'''FILE(GLOB_RECURSE app_sources ${{CMAKE_SOURCE_DIR}}/src/application/wifi_iot/app/{dirname}/*.*)

idf_component_register(
    SRCS ${{app_sources}}

    INCLUDE_DIRS
        ${{CMAKE_SOURCE_DIR}}/src/application/wifi_iot/app/{dirname}/include
)
''')
    # 更新 Services.json 文件
    if os.path.exists(services_file):
        with open(services_file, 'r') as file:
            services = json.load(file)
    else:
        services = {}
    
    services[dirname] = {
        "dir": f"${{CMAKE_SOURCE_DIR}}/src/application/wifi_iot/app/{dirname}/CMakeLists.txt"
    }
    
    with open(services_file, 'w') as file:
        json.dump(services, file, indent=4)
    
    print(Fore.GREEN + f"[Notice]: Directory \"{dirname}\" has been created successfully.")

def del_new_directory(service_name):
    base_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src', 'application', 'wifi_iot', 'app', service_name)
    services_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src', 'Services.json')
    cmake_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'src', 'CMakeLists.txt')
    platformio_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'platformio.ini')
    sdkconfig_file = os.path.join(os.path.dirname(os.path.abspath(__file__)), f'sdkconfig.{service_name}')
    # 删除服务目录
    if os.path.exists(base_dir):
        shutil.rmtree(base_dir)
        print(f"Service directory '{base_dir}' has been deleted.")
    else:
        print(f"Service directory '{base_dir}' does not exist.")
    
    # 更新 Services.json 文件
    if os.path.exists(services_file):
        with open(services_file, 'r') as file:
            services = json.load(file)
        
        if service_name in services:
            del services[service_name]
            with open(services_file, 'w') as file:
                json.dump(services, file, indent=4)
            print(f"Service '{service_name}' has been removed from Services.json.")
        else:
            print(f"Service '{service_name}' not found in Services.json.")
    
    # 更新 CMakeLists.txt 文件
    if os.path.exists(cmake_file):
        with open(cmake_file, 'r') as file:
            lines = file.readlines()
        
        with open(cmake_file, 'w') as file:
            for line in lines:
                if service_name not in line:
                    file.write(line)
        print(f"Service '{service_name}' has been removed from CMakeLists.txt.")
    if os.path.exists(platformio_file):
        with open(platformio_file, 'r') as file:
            lines = file.readlines()
        
        with open(platformio_file, 'w') as file:
            skip = False
            for line in lines:
                if line.strip().startswith(f"[env:{service_name}]"):
                    skip = True
                elif line.strip().startswith("[env:") and skip:
                    skip = False
                if not skip:
                    file.write(line)
        print(f"Service '{service_name}' has been removed from platformio.ini.")
    if os.path.exists(sdkconfig_file):
        os.remove(sdkconfig_file)
        print(f"Service '{service_name}' has been removed from sdkconfig.{service_name}.")

def build_project():
    os.system('pio run')

def upload_project():
    os.system('pio run -t upload')


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Manage script")
    parser.add_argument('--list', action='store_true', help="List all the services")
    parser.add_argument('--select', type=str, nargs='?', const=None, help="Select a service") # nargs='?' 表示可选参数, const='' 表示默认值
    parser.add_argument('--clean', action='store_true', help="Clean all the build files")
    parser.add_argument('--version', action='store_true', help= 'Show Version')
    parser.add_argument('--new', type=str, help='Create a new directory')
    parser.add_argument('--delete', type=str, help='Delete a directory')
    parser.add_argument('--build', action='store_true', help='Build the project')
    parser.add_argument('--upload', action='store_true', help='Upload the project')

    args = parser.parse_args()
    if args.select:
        select(args.select)
    elif args.list:
        list_services()
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
    elif args.new:
        create_new_directory(args.new)
    elif args.delete:
        del_new_directory(args.delete)
    elif args.build:
        build_project()
    elif args.upload:
        upload_project()
    else:
        print(Fore.RED + "No arguments provided.")
        parser.print_help()