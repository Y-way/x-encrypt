pushd .

cd ../XEncrypt

@REM Building shared libs
set cmake_options=-DBUILD_SHARED_LIBS=ON
cmake -B build_uwp -A Win32 -DCMAKE_SYSTEM_NAME=WindowsStore "-DCMAKE_SYSTEM_VERSION=10.0" %cmake_options%
cmake --build build_uwp --config MinSizeRel
md plugin_win\shared\Plugins\WSA\x86
copy /Y build_uwp\MinSizeRel\*.dll plugin_win\shared\Plugins\WSA\x86

cmake -B build_uwp64 -A x64 -DCMAKE_SYSTEM_NAME=WindowsStore "-DCMAKE_SYSTEM_VERSION=10.0" %cmake_options%
cmake --build build_uwp64 --config MinSizeRel
md plugin_win\shared\Plugins\WSA\x64
copy /Y build_uwp64\MinSizeRel\*.dll plugin_win\shared\Plugins\WSA\x64

cmake -B build_uwp_arm -A ARM -DCMAKE_SYSTEM_NAME=WindowsStore "-DCMAKE_SYSTEM_VERSION=10.0" %cmake_options%
cmake --build build_uwp_arm --config MinSizeRel
md plugin_win\shared\Plugins\WSA\ARM
copy /Y build_uwp_arm\MinSizeRel\*.dll plugin_win\shared\Plugins\WSA\ARM

cmake -B build_uwp_arm64 -A ARM64 -DCMAKE_SYSTEM_NAME=WindowsStore "-DCMAKE_SYSTEM_VERSION=10.0" %cmake_options%
cmake --build build_uwp_arm64 --config MinSizeRel
md plugin_win\shared\Plugins\WSA\ARM64
copy /Y build_uwp_arm64\MinSizeRel\*.dll plugin_win\shared\Plugins\WSA\ARM64

@REM Building static libs
set cmake_options=-DBUILD_SHARED_LIBS=OFF

cmake -B build_uwp -A Win32 -DCMAKE_SYSTEM_NAME=WindowsStore "-DCMAKE_SYSTEM_VERSION=10.0" %cmake_options%
cmake --build build_uwp --config MinSizeRel
md plugin_win\static\Plugins\WSA\x86
copy /Y build_uwp\MinSizeRel\*.lib plugin_win\static\Plugins\WSA\x86

cmake -B build_uwp64 -A x64 -DCMAKE_SYSTEM_NAME=WindowsStore "-DCMAKE_SYSTEM_VERSION=10.0" %cmake_options%
cmake --build build_uwp64 --config MinSizeRel
md plugin_win\static\Plugins\WSA\x64
copy /Y build_uwp64\MinSizeRel\*.lib plugin_win\static\Plugins\WSA\x64

cmake -B build_uwp_arm -A ARM -DCMAKE_SYSTEM_NAME=WindowsStore "-DCMAKE_SYSTEM_VERSION=10.0" %cmake_options%
cmake --build build_uwp_arm --config MinSizeRel
md plugin_win\static\Plugins\WSA\ARM
copy /Y build_uwp_arm\MinSizeRel\*.lib plugin_win\static\Plugins\WSA\ARM

cmake -B build_uwp_arm64 -A ARM64 -DCMAKE_SYSTEM_NAME=WindowsStore "-DCMAKE_SYSTEM_VERSION=10.0" %cmake_options%
cmake --build build_uwp_arm64 --config MinSizeRel
md plugin_win\static\Plugins\WSA\ARM64
copy /Y build_uwp_arm64\MinSizeRel\*.lib plugin_win\static\Plugins\WSA\ARM64

popd
