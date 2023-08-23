
pushd .
cd ../XEncrypt

cmake -B build_x64 -A x64
cmake --build build_x64 --config MinSizeRel
md plugin_win\Plugins\x86_64
copy /y build_x64\MinSizeRel\libXEncrypt.lib plugin_win\Plugins\x86_64\
copy /y build_x64\MinSizeRel\XEncrypt.dll plugin_win\Plugins\x86_64\

cmake -B build_x86 -A Win32
cmake --build build_x86 --config MinSizeRel
md plugin_win\Plugins\x86
copy /y build_x86\MinSizeRel\libXEncrypt.lib plugin_win\Plugins\x86\
copy /y build_x86\MinSizeRel\XEncrypt.dll plugin_win\Plugins\x86\

popd