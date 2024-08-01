# This file is part of u8clip.
#
# Copyright (c) 2024 Flokart World, Inc.
# All rights reserved.

cmake_minimum_required (VERSION 3.16.6)

if (NOT WIN32)
  message (FATAL_ERROR "Platforms supported by this program are Windows only.")
endif ()

project (u8clip VERSION 0.1.0)
find_package (CMakeSupports 0.0.7 REQUIRED)

CMS_BEGIN(Executable u8clip)
  CMS_SET_PROPERTY(Version ${PROJECT_VERSION})

  CMS_ADD_FEATURES(PUBLIC cxx_std_17)
  CMS_ADD_DEFINITIONS(NOMINMAX)

  if (MSVC)
    CMS_ADD_DEFINITIONS(_CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS)
  endif ()

  add_subdirectory (src)
CMS_END()
