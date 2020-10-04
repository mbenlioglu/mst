# Minimum Spanning Tree


**Implemented by:**

 * [M.Mucahid Benlioglu](https://github.gatech.edu/mbenlioglu3)
 
## Getting Started

**Prerequisites:**

 - [CMake](https://cmake.org/download/) (version 3.13 or higher)
 - [Make](https://www.gnu.org/software/make) (tested with GNU Make 4.2.1)
 - C++17 compatible compiler (tested with g++ 9.3.0)

_(Should work in Windows as well with either nmake or msbuild combined with cmake,
but it's untested)_

**For Plotting:**

 - [Python 3](https://www.python.org/) (tested with version 3.8.2)
 - [pip](https://pip.pypa.io/en/stable/) (tested with version 20.2.2)
 
### Installation and Running

There is a Makefile provided for the project. From the project source running running

```shell script
make
```

will create a `build` directory and compile the main executable and test executables.
It will also copy the main executable under project root directory for convenience.
It detects CMake automatically and makes the necessary build calls. This `Makefile`
should be able compile even when CMake is not present.

Alternatively running the following commands will give the same results

```shell script
mkdir build && cd build
cmake ..
make -j
```

### Testing

There are some unit tests included with this project. Running the following
command from the root directory after the project is built will run them show
their outputs.
```shell script
make test
```

Alternatively you can do the following and get the same result
```shell script
cd build
ctest -j
```
 
**Important:** due to time limitations `Makefile` is not as comprehensive as the
CMake version. Especially testing is not fully supported if your machine does not
have CMake installed. It's highly recommended that you have CMake on your machine
before running above commands.

### Plotting Results

A python script is provided to regenerate the plots and from scratch. Following
command will install prerequisites and regenerate the plots under `results` directory
```shell script
cd results
pip install -r requirements.txt
python3 plotter.py
``` 

## Algorithm and Discussion

![Static MST as the Number of Edges increase](/results/static.pdf)
![Insertion as the Number of Edges increase](/results/insertion.pdf)

