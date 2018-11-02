# Snake - attempting to solve an optically thick problem

## Purpose

 **Currently a WIP...**

 The purpose of this code is to test the performance of using the [Eddington approximation](https://en.wikipedia.org/wiki/Radiative_transfer#The_Eddington_approximation) to estimate the physical conditions in cells within the optically thick limit.

 The idea is that for cells in the optically thick limit, the physical conditions will be approximated by iterating towards final conditions using the Eddington approximation. However, cells in the optically thin limit will have their physical conditions approximated using Monte Carlo Radiative Transfer (MCRT). This *should* create a hybrid scheme which will allow better treatment of optically thick regions in MCRT simulations.


## Requirements

Just a C compiler!

## Building

Snake can be built using the provided Makefile and maybe the CMake file as well.

```bash
$ make snake
```

Once built, the executable is stored in the `bin` directory. It is recommended that you add this directory to you `PATH` variable.

## Usage

To execute a simulation, a parameter file is required to be provided as a command line argument, e.g., 

```bash
$ snake plane.par
```

Example parameter files can be found in the `examples` directory.

## Acknowledgements 

I would like to acknowledge financial support from the EPSRC Centre for Doctoral Training in Next Generation Computational Modelleing grant EP/L015382/1.
