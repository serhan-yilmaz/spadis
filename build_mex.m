current_path=cd;
cd([current_path, '/src/cpp/']);
mex('-g', '-O', '-largeArrayDims', '-lut', '-output', [current_path, '/binaries/spadis'], '*.cpp');
cd([current_path]);



