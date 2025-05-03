#!/bin/bash

# Usage: ./rename_project.sh NewProductName NewProjectName new_namespace [--dry-run]

OLD_PRODUCT_NAME=WaveshaperPlugin
OLD_PROJECT_NAME=AdamWaveshaper
OLD_NAMESPACE_NAME=waveshaper_plugin

NEW_PRODUCT_NAME=$1
NEW_PROJECT_NAME=$2
NEW_NAMESPACE_NAME=$3
DRY_RUN=false

if [ "$4" == "--dry-run" ]; then
  DRY_RUN=true
fi

if [ -z "$NEW_PRODUCT_NAME" ] || [ -z "$NEW_PROJECT_NAME" ]; then
    echo "Usage: $0 NewProductName NewProjectName [--dry-run]"
    exit 1
fi

echo "Preparing to rename:"
echo "  PRODUCT_NAME: $OLD_PRODUCT_NAME -> $NEW_PRODUCT_NAME"
echo "  PROJECT_NAME: $OLD_PROJECT_NAME -> $NEW_PROJECT_NAME"
if $DRY_RUN; then echo "  Mode: DRY RUN (no files will be changed)"; fi
echo


# Function to replace PRODUCT_NAME assignment lines
replace_in_file() {
  local file=$1
  local modified=false

  # Replace PRODUCT_NAME string literals like "MyPlugin"
  if grep -q "\"${OLD_PRODUCT_NAME}\"" "$file"; then
    modified=true
    $DRY_RUN && echo "[Would replace PRODUCT_NAME] $file"
    ! $DRY_RUN && sed -i'' -E "s/\"${OLD_PRODUCT_NAME}\"/\"${NEW_PRODUCT_NAME}\"/g" "$file"
  fi

  # Replace include path strings like include/MyPlugin
  if grep -q "include/${OLD_PRODUCT_NAME}" "$file"; then
    modified=true
    $DRY_RUN && echo "[Would update include path] $file"
    ! $DRY_RUN && sed -i'' -E "s|include/${OLD_PRODUCT_NAME}|include/${NEW_PRODUCT_NAME}|g" "$file"
  fi

  # Replace C++ #include lines like #include "MyPlugin/Foo.h"
  if grep -q "#include[[:space:]]\+[\"<]${OLD_PRODUCT_NAME}/" "$file"; then
    modified=true
    $DRY_RUN && echo "[Would update C++ includes] $file"
    ! $DRY_RUN && sed -i'' -E "s|([\"<])${OLD_PRODUCT_NAME}/|\1${NEW_PRODUCT_NAME}/|g" "$file"
  fi

  # Replace safe PROJECT_NAME references (CMake targets, project() line, etc.)
  if grep -qE "(project|target_compile_definitions|--target)[^a-zA-Z0-9]*${OLD_PROJECT_NAME}(_VST3)?" "$file"; then
    modified=true
    $DRY_RUN && echo "[Would replace PROJECT_NAME target refs] $file"
    ! $DRY_RUN && sed -i'' -E "s/\b${OLD_PROJECT_NAME}(_VST3)?\b/${NEW_PROJECT_NAME}\1/g" "$file"
  fi

  echo $file

  # Match and replace namespace declarations (including nested, like audio_plugin::dsp)
  if grep -q "namespace ${OLD_NAMESPACE_NAME}" "$file"; then
    modified=true
    $DRY_RUN && echo "[Would update namespace declaration] $file"
    ! $DRY_RUN && sed -i'' -E "s/\bnamespace ${OLD_NAMESPACE_NAME}(\b|::)/namespace ${NEW_NAMESPACE_NAME}\1/g" "$file"
  fi

  # Match and replace closing namespace comments
  if grep -q "// namespace ${OLD_NAMESPACE_NAME}" "$file"; then
    modified=true
    $DRY_RUN && echo "[Would update namespace comment] $file"
    ! $DRY_RUN && sed -i'' -E "s|(// namespace )${OLD_NAMESPACE_NAME}(\b|::)|\1${NEW_NAMESPACE_NAME}\2|g" "$file"
  fi

  $modified && ! $DRY_RUN && echo "Modified: $file"
}


# Optionally rename include directory if it matches OLD_PRODUCT_NAME
rename_dir() {
  local old=$1
  local new=$2
  if [ -d "$old" ]; then
    if $DRY_RUN; then
      echo "[Would rename dir] $old -> $new"
    else
      mv "$old" "$new"
      echo "Renamed dir: $old -> $new"
    fi
  fi
}

# Update PRODUCT_NAME in config files
for path in plugin/CMakeLists.txt test/CMakeLists.txt CMakePresets.json Makefile; do
  [ -f "$path" ] && replace_in_file "$path"
done

# Update include paths in source files
find plugin/source test/source -name '*.cpp' -o -name '*.h' | while read -r file; do
  replace_in_file "$file"
done

# Update include paths in header files
find plugin/include test/source -name '*.cpp' -o -name '*.h' | while read -r file; do
  replace_in_file "$file"
done

# Rename include dir if it exists
rename_dir "./plugin/include/${OLD_PRODUCT_NAME}" "./plugin/include/${NEW_PRODUCT_NAME}"

echo
echo "Done."
