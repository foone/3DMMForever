:: Copyright (c) Microsoft Corporation.
:: Licensed under the MIT License.

:: Variables documented in README.md

:: Setup the Socrates Directory Environment Variable
set SOC_ROOT=%cd%
:: Setup the Kauai dir that's needed by most things
set KAUAI_ROOT=%SOC_ROOT%\kauai
:: Update includes to cover whats needed
set include=%include%;%SOC_ROOT%\INC;%SOC_ROOT%\BREN\INC;%KAUAI_ROOT%\SRC;%SOC_ROOT%\SRC

set MSVCNT_ROOT=C:\MSVC20

:: Set project
set PROJ=SOC

:: Set operating system to compile for
set ARCH=WIN

:: Uncomment this to compile for Unicode
:: set UNICODE=1

:: Set build type
set TYPE=DBSHIP

:: Use optimized assembly for Intel 80386
set CHIP=IN_80386

:: BLD_TYPE_DIR will be set automatically