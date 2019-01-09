# Snake - attempting to solve an optically thick problem

## Purpose

The purpose of this code is to test the performance of using the [Eddington approximation](https://en.wikipedia.org/wiki/Radiative_transfer#The_Eddington_approximation) to estimate the physical conditions in cells within the optically thick limit.

The idea is that for cells in the optically thick limit, the physical conditions will be approximated by iterating towards final conditions using the Eddington approximation. However, cells in the optically thin limit will have their physical conditions approximated using Monte Carlo Radiative Transfer (MCRT). This *should* create a hybrid scheme which will allow better treatment of optically thick regions in MCRT simulations. To determine the Rosseland mean opacity of grid cells, the Opal Opacity Code is used, which can be found [here](https://opalopacity.llnl.gov/). 

A more detailed overview of the general idea and algorithm can be found on this [Evernote page](https://www.evernote.com/shard/s669/sh/e6f7ca5e-1055-4d07-8366-5b144d2e5d88/4327343bd2391cc6a6b96d53eea29d20).

## Requirements

Both a C and Fortran compiler are required, such as `gcc` and `gfortran`. The GNU Science libraries are also required.

## Building

Snake can be built using the provided Makefile and maybe the CMake file as well.

```bash
$ make snake
```

Once built, the executable is stored in the `bin` directory. It is recommended that you add this directory to you `PATH` variable.

## Usage

To execute a simulation, an opacity table, such as the Opal `GN93hz`, is required to be in the working directory. The simulation executable, `snake`, also requires a parameter file to be provided as a command line argument, e.g., 

```bash
$ snake plane.par
```

Example parameter files, and the `GN93Hz` tables can be found in the `examples` directory.

## Tabulated Opacities

To calculate the Rosseland Mean Opacity, either the Rosseland Mean Opacity is found using 4D interpolation provided by the Opal Opacity tables, or the Rosseland Mean Opacity is calculated using 2D interpolation over a table created by the `create_opacity_table.py` script located in the `libs` directory. Usage of this script can be found by invoking it with the `-h` switch.

When specifying the opacity table to use, if `GN93Hz` is given (the Opal table), then Snake will calculate the Rosseland Mean Opacity using 4D interpolation from Opal over the variables R, T, X and Z. Providing any other table name will result in 2D interpolation using GSL over the variables R and T. 

## Acknowledgements 
 
I would like to acknowledge financial support from the EPSRC Centre for Doctoral Training in Next Generation Computational Modelleing grant EP/L015382/1.

I would also like to say thanks to the team who made the Opal Opacity Code which saved me a lot of time, effort and sanity, and to Stuart for reccomending I use it!
