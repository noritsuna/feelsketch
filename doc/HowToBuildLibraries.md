# C++Library
PM-Code is constructed by three C++ Libraries listed below.
  * PMCodeCommon
    * ommon parts of the encoder and the decoder.
  * PMCodeDecoder
    * he decoder. Converts PM-Code data to binary data.
  * PMCodeEncoder
    * he encoder. Converts binary data to PM-Code data.

# Build C++ Library

## Download
Check out from this site.
https://github.com/noritsuna/feelsketch.git

## Build
```
cd feelsketch/lib/cpp
./configure
make
sudo make install
```

# How JNI Library is organized.
Presently, JNI Library’s encoder is only offered.
  * PMCodeEncoderJNI
    * JIN Library for the encoder.
    * Depends on both PM-Code Common and PM-Code Encoder.。

# Building of JNI Library

## Download
Check out from this site.
https://github.com/noritsuna/feelsketch.git

## Preparation
Install JDK.
http://java.sun.com/javase/downloads/index.jsp

Always, select the JDK directory in the environmental variable JAVA\_HOME.

ex)
```
export JAVA_HOME=/System/Library/Frameworks/JavaVM.framework/Versions/1.6.0/Home
```

## Build
```
cd feelsketch/lib/jni/PMCodeEncoderJNI
./configure
make
sudo make install
```

# JNI sample
## Build
```
cd feelsketch/lib/sample/jni-sample
javac jp/co/cia/pmcode/PMCodeEncoderSample.java
javac jp/co/cia/pmcode/PMCodeEncoderJNI.java
```
## Perform
### Mac OSX
```
./run_osx.sh
```
### Linux
```
./run_linux.sh
```
## Conferm
If a file “pmcode.bmp” is created successfully, then the sample works.
Open the image file.