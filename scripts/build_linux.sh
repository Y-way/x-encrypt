pushd .

cd ../XEncrypt/

cmake -B build_linux64 -DBUILD_SHARED_LIBS=ON
cmake --build build_linux64 --config Release

mkdir -p plugin_linux/shared/Plugins/Linux/x86_64
cp build_linux64/*.so plugin_linux/shared/Plugins/Linux/x86_64

cmake -B build_linux64_static -DBUILD_SHARED_LIBS=OFF
cmake --build build_linux64_static --config Release

mkdir -p plugin_linux/static/Plugins/Linux/x86_64
cp build_linux64_static/*.a plugin_linux/static/Plugins/Linux/x86_64

popd
