#!/bin/bash
# SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
# SPDX-License-Identifier: BSD-3-Clause

# Available configurations:
# Debug - minimal debug info, no code optimization, debug output (define DEBUG=1)
# DebugWithInfo - same as Debug with maximum debug info (define DEBUG=1)
# Release - smallest code size, optimized, no debug output (define NDEBUG=1)
# RelWithDebugOutput - same as Release with debug output (define BUILD_WITH_DEBUG_OUTPUT=1)
# RelForDev - same as RelWithDebugOutput with specific features for devs (define BUILD_FOR_DEV_MODE=1)

# Set the configurations to build by default (when option -c not specified):
# default_configs+=('Debug')
# default_configs+=('DebugWithInfo')
default_configs+=('Release')
# default_configs+=('RelWithDebugOutput')
# default_configs+=('RelForDev')

# Set the DEFAULT install prefix if none given.
# Install everything to the project tree:
prefix=./dist
# OR, install files to $PSPDEV/psp (PSP user libraries):
# prefix=`psp-config --psp-prefix`
# OR, install files to $PSPDEV/psp/sdk (PSP user/kernel stuff like public headers):
# prefix=`psp-config --pspsdk-path`

build_dir=build
generator="Ninja Multi-Config"

show_help()
{
    echo "Usage:"
    echo "  ./b.sh [options] <command>..."
    # echo "  ./b.sh pspinstall <ms0_root>"
    # echo "  ./b.sh pspuninstall <ms0_root>"
    echo "Options:"
    echo "  -h --help       Show this screen"
    echo "  -v --verbose    Verbose CMake build output"
    echo "  -P --prefix=<path>"
    echo "                  Set the project install directory prefix"
    echo "  -c --config=<types>"
    echo "                  Set the configuration(s) to build:"
    echo "                    Debug"
    echo "                    DebugWithInfo"
    echo "                    Release"
    echo "                    RelWithDebugOutput"
    echo "                    RelForDev"
    echo "                  eg. --config Debug,Release,RelForDev'"
    # echo "  --ppsspp        Indicate the host target is the PPSSPP emulator"
    echo "  --pspgo         Indicate the target system is a PSP Go"
    echo "Commands:"
    echo "  all             Build the project with the set configuration(s),"
    echo "                  is the default command to run when none given."
    # echo "  debug           Build the Debug configuration"
    # echo "  release         Build the Release configuration"
    # echo "  relfordev       Build the RelForDev configuration"
    echo "  clean           Delete the build directory"
    echo "  rebuild         Clean then build (all)"
    echo "  install         Install project files (CMake install)"
    echo "  uninstall       Remove installed project files (if any)"
    echo "  dist            Create a distribution zip file for this project"
    echo "  distclean       Delete all distribution zip files"
    # echo "  pspinstall      Install the dist contents to the PSP MemoryStick"
    # echo "  pspuninstall    Remove the dist contents installed to the PSP MemoryStick"
    echo "  help            Show this screen"
}

short=v,h,P:,c:
long=verbose,help,prefix:,config:,pspgo
opts=`getopt -a -n b --options $short --longoptions $long -- "$@"`

cmake_configs=()
cmake_var_defs=()

eval set -- "$opts"
while :
do
  case "$1" in
    -h | --help )
      show_help
      exit 0
      ;;
    -v | --verbose )
      # See https://cmake.org/cmake/help/latest/envvar/VERBOSE.html#envvar:VERBOSE
      export VERBOSE=1
      shift
      ;;
    -P | --prefix )
      prefix="$2"
      shift 2
      ;;
    -c | --config )
      IFS=',; ' read -r -a configs <<< "$2"
      cmake_configs+=(${configs[@]})
      shift 2
      ;;
    --pspgo )
      cmake_var_defs+=('PSP_GO=true' 'PSPGO=true' 'BUILD_PSP_GO=true')
      shift
      ;;
    -- )
      shift;
      break
      ;;
    *)
      echo "Unexpected option: $1"
      ;;
  esac
done

if [[ "${#cmake_configs[@]}" -eq 0 ]]; then
    cmake_configs=(${default_configs[@]})
fi

do_cmake_configure()
{
    configs=$(printf ", %s" "${cmake_configs[@]}")
    echo "Configure with install prefix '$prefix'"
    echo "Configure with configurations ${configs:2}"

    var_defs=
    if [[ "${#cmake_var_defs[@]}" -ne 0 ]]; then
        var_defs=$(printf -- "-D%s " "${cmake_var_defs[@]}")
    fi

    configs=$(printf ";%s" "${cmake_configs[@]}")
    configs=${configs:1}
    cmake -S . -B "$build_dir" -G "$generator"          \
        -DCMAKE_INSTALL_PREFIX="$prefix"                \
        -DCMAKE_CONFIGURATION_TYPES="$configs"          \
        -DCMAKE_DEBUG_POSTFIX="_debug"                  \
        -DCMAKE_DEBUGWITHINFO_POSTFIX="_debuginfo"      \
        -DCMAKE_RELFORDEV_POSTFIX="_dev"                \
        $var_defs                                       \
        --no-warn-unused-cli
}

do_cmake_build()
{
    for c in ${cmake_configs[@]}; do
        cmake --build "$build_dir" --config $c
        if [[ "$?" -ne 0 ]]; then
            exit $?
        fi
    done
}

do_cmake_install()
{
    for c in ${cmake_configs[@]}; do
        cmake --install "$build_dir" --config $c
        if [[ "$?" -ne 0 ]]; then
            exit $?
        fi
    done
}

do_build()
{
    do_cmake_configure && do_cmake_build
}

do_clean()
{
    rm -rf "$build_dir/"
}

do_install()
{
    do_cmake_install
}

do_uninstall()
{
    install_manifest="$build_dir/install_manifest.txt"
    if [ ! -f "$install_manifest" ]; then
        echo "Nothing to do (install_manifest not found)"
        exit 0
    fi

    files=()
    while IFS=$' \t\r\n' read -r line || [ -n "$line" ]; do
        # printf '%s\n' "$line"
        if [ -f "$line" ]; then
            files+=("$line")
        fi
    done < $install_manifest

    if [[ "${#files[@]}" -eq 0 ]]; then
        echo "Files were already removed"
    else
        # rm `printf '%s\n' "${files[@]}"`
        rm "${files[@]}"
        if [[ "$?" -eq 0 ]]; then
            echo "${#files[@]} files removed"
        fi
    fi
}

do_dist()
{
    #NOTE this is a test
    echo "TODO package the project installed files into a .zip archive"
    cmake --build "$build_dir" --target dist_test
    exit 0

    for c in ${cmake_configs[@]}; do
        #TODO
        cmake --build "$build_dir" --target dist_zip_Release --config $c
        if [[ "$?" -ne 0 ]]; then
            exit $?
        fi
    done
}

do_distclean()
{
    echo "TODO remove dist"
    # rm -rf "$dist_dir/"
}

#TODO remove duplicate commands AND preserve the order
# set -- `echo "$@" | tr ' ' '\n' | sort -u`

# Set the default command to run when none given.
if [ -z ${1+x} ]; then
    set -- "all"
fi

# Run the given command(s).
while :
do
  case "$1" in
    help )
      show_help
      exit 0
      shift
      ;;
    all )
      do_build
      # do_install
      # do_dist
      shift
      ;;
    clean )
      do_clean
      shift
      ;;
    rebuild )
      do_clean
      do_build
      shift
      ;;
    install )
      do_install
      shift
      ;;
    uninstall )
      do_uninstall
      shift
      ;;
    dist )
      do_dist
      shift
      ;;
    distclean )
      do_distclean
      shift
      ;;
    # pspinstall )
    #   do_pspinstall
    #   shift
    #   ;;
    # pspuninstall )
    #   do_pspuninstall
    #   shift
    #   ;;
    *)
      if [[ "$#" -eq 0 ]]; then
          break
      fi
      echo "Unknown command: $1"
      shift
      ;;
  esac
done

exit $?
