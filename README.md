
# install dependencies
```
sudo apt install -y libcpprest-dev
```

# generate client
```
openapi-generator-cli generate
```

# build
```
$ mkdir build
$ cd build
$ cmake ../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake
$ cmake --build . -- -j8
```