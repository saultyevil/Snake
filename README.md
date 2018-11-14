# Snake - attempting to solve an optically thick problem

## Purpose

**Currently a WIP...**

**!!!!ALART!!!! Currently doesn't seem to work on my MacBook using gfortran-8!!!**

The purpose of this code is to test the performance of using the [Eddington approximation](https://en.wikipedia.org/wiki/Radiative_transfer#The_Eddington_approximation) to estimate the physical conditions in cells within the optically thick limit.

The idea is that for cells in the optically thick limit, the physical conditions will be approximated by iterating towards final conditions using the Eddington approximation. However, cells in the optically thin limit will have their physical conditions approximated using Monte Carlo Radiative Transfer (MCRT). This *should* create a hybrid scheme which will allow better treatment of optically thick regions in MCRT simulations. To determine the Rosseland mean opacity of grid cells, the Opal Opacity Code is used, which can be found [here](https://opalopacity.llnl.gov/). 

A more detailed overview of the general idea and algorithm can be found on this [Evernote page](https://www.evernote.com/shard/s669/sh/e6f7ca5e-1055-4d07-8366-5b144d2e5d88/4327343bd2391cc6a6b96d53eea29d20).

## Requirements

Both a C and Fortran compiler are required, such as `gcc` and `gfortran`. 

## Building

Snake can be built using the provided Makefile and maybe the CMake file as well.

```bash
$ make snake
```

Once built, the executable is stored in the `bin` directory. It is recommended that you add this directory to you `PATH` variable.

## Usage

To execute a simulation, the opacity tables `GN93hz` is required to be in the working directory. The simulation executable, `snake`, also requires a parameter file to be provided as a command line argument, e.g., 

```bash
$ snake plane.par
```

Example parameter files, and the `GN93Hz` tables can be found in the `examples` directory.

## Acknowledgements 
 
I would like to acknowledge financial support from the EPSRC Centre for Doctoral Training in Next Generation Computational Modelleing grant EP/L015382/1.

I would also like to say thanks to the team who made the Opal Opacity Code which saved me a lot of time, effort and sanity, and to Stuart for reccomending I use it!
