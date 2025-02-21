pushd .

cd ../XEncrypt/

cmake -B build_linux64 -DBUILD_SHARED_LIBS=ON -DYAISO_ENABLE_NI=ON
cmake --build build_linux64 --config Release

mkdir -p plugin_linux/Plugins/x86_64
cp build_linux64/*.so plugin_linux/Plugins/x86_64/
cp build_linux64/*.a plugin_linux/Plugins/x86_64/

popd