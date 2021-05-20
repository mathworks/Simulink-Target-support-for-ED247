
# ED247 for Simulink

## Speedgoat compatibility

### ED247 library

#### Compilation

ED247 shall be compiled with QNX to be integrated on Speedgoat.

##### Prerequisites

- Install "Simulink Real-Time Target Support Package"

##### Steps

- Clone ED247 library sources
- Create a BAT-script with:

    ```
    SET CMAKE=<CMAKE_INSTALLATION_FOLDER>\bin\cmake.exe
    call %ALLUSERSPROFILE%"\MATLAB\SupportPackages\<MATLAB_RELEASE>\toolbox\slrealtime\target\supportpackage\qnx<QNX_VERSION>\qnxsdp-env.bat"
    %CMAKE% . -B"_build" -DCMAKE_TOOLCHAIN_FILE="cmake/toolchains/qnx.cmake" -G"Unix Makefiles" -DCMAKE_INSTALL_PREFIX=_install -DBUILD_DEPS_LIBXML2=OFF -DBUILD_SAMPLES=ON -DCMAKE_C_COMPILER_WORKS=1 -DCMAKE_CXX_COMPILER_WORKS=1
    %CMAKE% --build _build --target install
    ```

- Replace tokens by:
    - <CMAKE_INSTALLATION_FOLDER> : Absolute path to CMake installation (ex. "C:\PROGRAMS\cmake\3.20.2")
    - <MATLAB_RELEASE> : MATLAB Release used with Speedgoat and with Support Package installed (ex. R2020b)
    - <QNX_VERSION> : QNX version associated with MATLAB Release (ex. 700 for R2020b or 710 for R2021a)

- Run script in Windows PowerShell
- ED247 library is generated in _install/lib (libed247.so and libed247_static.a)
