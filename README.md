# SPADIS
#### An algorithm for Selecting Predictive and Diverse SNPs in GWAS
SPADIS is an algorithm designed to recover single nucleotide polymorphisms (SNPs) that are related with a phenotype in genome wide association studies (GWAS) using a SNP-SNP interaction network.

## Getting Started:
SPADIS provides a MATLAB interface for ease of use. These instructions will guide you to build and run SPADIS on MATLAB.

### Requirements:
Building SPADIS requires the Boost C++ library. Most Linux distributions come with Boost pre-installed. However, if your operating system does not include Boost, follow the [Boost getting started guide](http://www.boost.org/more/getting_started/index.html) for instructions on how to install it.  
Check [Boost website](http://www.boost.org) for more information.

### Installation:
In order to build SPADIS for MATLAB, you can use GNU make:
```
make matlab
```
or directly run the mex building script:
```
build_mex.m
```

### Examples


## License
This project is licensed under GNU GPL v3 - see the [LICENSE.md](LICENSE.md) file for details.

## Acknowledgements
