# Bayesian Estimation Engine #

Welcome to the **computational engine** for Bayesian Estimation.

[![pipeline status](https://gitlab.girs-spc.seas.ucla.edu/jpl/bayesian-estimation-engine/badges/develop/pipeline.svg)](https://gitlab.girs-spc.seas.ucla.edu/jpl/bayesian-estimation-engine/commits/develop)
[![coverage report](https://gitlab.girs-spc.seas.ucla.edu/jpl/bayesian-estimation-engine/badges/develop/coverage.svg)](https://gitlab.girs-spc.seas.ucla.edu/jpl/bayesian-estimation-engine/index.html)


## Features ##

  * Homogeneous Estimation
  * Heterogeneous Estimation

## Platforms ##

Bayesian Estimation has been tested on a variety of platforms:

  * Ubuntu 16
  * macOS 10.14

### Linux Requirements ###

These are the base requirements to build and use Bayesian Estimation from a source package (as described below):

  * CMake v3.8.0 or newer
  * GCC v5.4.0
  * GNU-compatible Make

### macOS Requirements ###

  * CMake v3.8.0 or newer
  * Xcode Developer Tools v10

## Development Instructions ##

### Cloning Instructions ###

The Bayesian Estimation project includes a number of dependencies as git submodules that need to be initialized and updated individually.
In order to avoid this, the following command can be used:

    git clone --recursive git@gitlab.girs-spc.seas.ucla.edu:jpl/bayesian-estimation-engine.git

### Working on the Project ###

Before starting any work, __checkout__ to the `develop` branch by running:

    git checkout develop
    
Create and/or assign an issue to yourself. Next, create a branch out of the `develop` branch from the issue in Gitlab.

The Bayesian Estimation project has the following submodules:
 * [catch](https://github.com/catchorg/Catch2)

#### Pulling in Upstream Changes ####

For each submodule the __upstream changes__ can be __pulled__ individually by going into its directory and running:

    git fetch
    git merge

the upstream branch to update the local code.

Alternatively, if you prefer to __automatically fetch__ and __merge__ for each submodule, you can run:

    git submodule update --remote

Git will go into your submodules and fetch and update for you. Note, after each update, you have to commit any potential changes.

#### Working on a Submodule ####

If you run the `git submodule update --remote` command to pull changes from the submodule repositories, Git will get the changes and update the files in the subdirectory but will leave the sub-repository in a “detached HEAD” state. This means that there is no local working branch tracking changes.

Thus, you need to do two steps in order to set up your submodule for development.

* First, go into our submodule directory and __checkout__ the `master` and `develop` branches:

        git checkout master
        git checkout develop

Continue the development as you would do on a standalone repository. After you are done, switch to the `master` branch and merge your changes on the `develop` branch.

* Second, __merge__ the `master` branch changes from the submodules with:

        git submodule update --remote --merge

For the Git documentation on working with submodules, please go [here](https://git-scm.com/book/en/v2/Git-Tools-Submodules).


### Build Instructions using CMake ###

Bayesian Estimation comes with a CMake build script ([CMakeLists.txt](src/CMakeLists.txt)) that can be used on a wide range of platforms ("C" stands for cross-platform). If you don't have CMake installed already, you can download it for free from [here](http://www.cmake.org/).

CMake works by generating native makefiles or build projects that can be used in the compiler environment of your choice.

#### Coverage Tool Installation ####

* On Ubuntu use [APT](https://help.ubuntu.com/community/AptGet/Howto):

        sudo apt-get install gcovr

* On macOS use [Homebrew](https://brew.sh):

        brew install lcov

#### Standalone CMake Project ####

When building Bayesian Estimation as a standalone project, the typical workflow starts with:

    mkdir build       # Create a directory to hold the build output.
    cd build
    cmake ../src -DCMAKE_BUILD_TYPE=Debug  # Generate native build scripts.

If you are on a \*nix system, you should now see a `Makefile` in the current directory. Continue with the following commands:

    make              # Build Bayesian Estimation.
    ctest             # Run the tests.

To generate a coverage report continue with:

    make coverage     # Generate coverage report.

If you use Windows and have Visual Studio installed, a `.sln` file and several `.vcproj` files will be created.  You can then build them using Visual Studio.

On macOS with Xcode installed, a `.xcodeproj` file will be generated.

The supported build types are: `Release` and `Debug`.

## Bayesian Estimation Architecture ##

The [src/](src/) folder contains the following projects:

* __brassweibull__: .
* [catch](https://github.com/catchorg/Catch2): A modern, C++-native, header-only, test framework for unit-tests.
* __cephes__: .
* __core__: .
* __dcdf__: .
* __global__: .
* __homogeneous__: .
* __nonhomogeneous__: .
* __parametric__: .
* __ran__: .
* __redcas__: .
* __redcassrv__: .
* __slatec__: .
* __tests__: Unit tests using the Catch library.
* __testsuite__: Old tests library.
* __twostage__: .
