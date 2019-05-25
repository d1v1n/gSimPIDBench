gSimPIDBench
============

Application implements the simple model of PID governor with plant process 

## How to build

MahtGL version 2.3.2 was required, with 2.4.3 does not commpile due to changes in API for C interfacing

Building MathGL
~~~
cmake .
cmake .
make
sudo make install
~~~
Removing MathGL
~~~
cat install_manifest.txt | xargs echo sudo rm | sh
~~~
Building using existing cmake configuration
~~~
cmake .
make
~~~
With debugging option
~~~
cmake -DDEFINE_DEBUG=ON .
make
~~~
## Running application
~~~
./gSimPIDBench 
~~~
### If there are problems with loading shared library
~~~
LD_LIBRARY_PATH=/usr/local/lib/
export LD_LIBRARY_PATH
~~~
## Tables with additional data
Simple implementation, use spaces as separator between values
### example_railTable.txt  
Represents fuel rate (same unit as first couml of _trqTable) as function of engine speed (in s-1) (first line) and relative control input (first column 0 - minimum, 1 - maximum)
~~~
0 0 30 100 200
0 0 0 0 0
0.50 0 55 60 65
0.75 0 95 100 105
1.00 0 125 130 135
~~~
### example_trqTable.txt
Represents engine torque (in Nm) as function of engine speed (in s-1) (first line) and fuel rate (first column)
~~~
0 0 30 100 200
0 0 0 0 0
50 0 90  100 90
100 0 500 510 500
150 0 600 1050 900
~~~