# SPDX-FileCopyrightText: 2022 Linblow <dev@linblow.com>
# SPDX-License-Identifier: BSD-3-Clause

# This file gets and sets the pspdev and pspsdk path:
# - PSPDEV / ENV{PSPDEV} eg. /usr/local/pspdev
# - PSPSDK / ENV{PSPSDK} eg. /usr/local/pspdev/psp/sdk

if (NOT DEFINED PSPDEV)
  if (DEFINED ENV{PSPDEV})
    SET(PSPDEV $ENV{PSPDEV})
  else()
    execute_process(
      COMMAND psp-config --pspdev-path
      OUTPUT_VARIABLE PSPDEV OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_VARIABLE  STDERR_OUTPUT
      RESULT_VARIABLE STATUS_CODE
    )
    if (NOT "${STATUS_CODE}" STREQUAL "0")
      set(MSG "PSPDEV is not defined.")
      if (NOT "${STDERR_OUTPUT}" STREQUAL "")
        string(APPEND MSG "${STDERR_OUTPUT}")
      endif()
      message(FATAL_ERROR ${MSG})
    else()
      set(ENV{PSPDEV} "${PSPDEV}")
      set(PSPDEV "${PSPDEV}" CACHE PATH "pspdev path")
    endif()
  endif()
endif()

SET(PSP_PREFIX "${PSPDEV}/psp")
set(PSPSDK "${PSPDEV}/psp/sdk")
set(ENV{PSP_PREFIX} "${PSP_PREFIX}")
set(ENV{PSPSDK} "${PSPSDK}")
