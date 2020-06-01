# Pride Face

you'll need to install docker for this to work

## How to compile
cd into the current directory. now use docker to build the app:

docker run --rm -it -v $(pwd):/work dmorgan81/rebble:latest build

now install it on your phone (adjust the ip address!)

docker run --rm -it -v $(pwd):/work dmorgan81/rebble:latest install --phone 192.168.2.103

## Whats with the CMakeLists.txt?

Thats just for my IDE (CLion) to be happy and be able to find the headers for autocomplete.
In case you use this too, copy "sdk-core" to the parent directory so that the parent contains
sdk-core and this pride repository. the sdk-core can be obtained from https://raw.githubusercontent.com/aveao/PebbleArchive/master/SDKCores/sdk-core-4.3.tar.bz2
