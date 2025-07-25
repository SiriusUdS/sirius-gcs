@echo off

REM Install git submodules
pushd %~dp0..
echo Installing git submodules...
git submodule update --init --recursive

REM Install dependencies
call vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install --host-triplet=x64-windows --x-install-root=vcpkg\installed
.\vcpkg\vcpkg integrate install

REM Generate project files
call setup\generate_vs_solution.bat

REM Go back to current directory and exit
popd
popd
exit /b 0
