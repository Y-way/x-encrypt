
pushd .
cd ../XEncrypt

set build_cfg=%1
if not defined build_cfg set build_cfg=MinSizeRel

set cmake_options=-DBUILD_SHARED_LIBS=ON

cmake -B build_x64 -A x64 %cmake_options%
cmake --build build_x64 --config %build_cfg%

md plugin_win\Plugins\x86_64

copy /y build_x64\%build_cfg%\*.dll plugin_win\Plugins\x86_64\

cmake -B build_x86 -A Win32 %cmake_options%
cmake --build build_x86 --config %build_cfg%

md plugin_win\Plugins\x86
copy /y build_x86\%build_cfg%\*.dll plugin_win\Plugins\x86\


set cmake_options=-DBUILD_SHARED_LIBS=OFF

cmake -B build_x64_lib -A x64 %cmake_options%
cmake --build build_x64_lib --config %build_cfg%

set cmake_options=-DBUILD_SHARED_LIBS=OFF
cmake -B build_x64 -A x64 %cmake_options%
cmake --build build_x64 --config %build_cfg%

md plugin_win\Plugins\lib\x86_64
copy /y build_x64_lib\%build_cfg%\*.lib plugin_win\Plugins\lib\x86_64\

cmake -B build_x86_lib -A Win32 %cmake_options%
cmake --build build_x86_lib --config %build_cfg%

md plugin_win\Plugins\lib\x86
copy /y build_x86_lib\%build_cfg%\*.lib plugin_win\Plugins\lib\x86\

popd
