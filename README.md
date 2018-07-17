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
or directly run the MATLAB script for building mex files:
```
build_mex.m
```

### Examples
We provide a few examples on how to run SPADIS on MATLAB. 
Simply run the demo file:
```
demo.m
```

In the provided examples, the genotype and flowering time phenotypes data of Arabidopsis Thaliana (AT) obtained from [Atwell et. al. (2010)](https://www.ncbi.nlm.nih.gov/pubmed/20336072) are used. For descriptions and format of the data, check the [readme file for data](data/readme_data.txt).

## License
This project is licensed under GNU GPL v3 - see the [LICENSE](LICENSE) file for details.

## References
Yilmaz, Serhan, Tastan, Oznur & Cicek, A. Ercument (2018). [SPADIS: An Algorithm for Selecting Predictive and Diverse SNPs in GWAS](https://www.biorxiv.org/content/early/2018/05/17/256677). bioRxiv

Atwell, S. et al. (2010) [Genome-wide association study of 107 phenotypes
in Arabidopsis thaliana inbred lines](https://www.ncbi.nlm.nih.gov/pubmed/20336072). Nature, 465(7298), 627–631.

Wu, M. C. et al. (2011) [Rare-variant association testing for sequencing
data with the sequence kernel association test](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC3135811/). The American Journal of
Human Genetics, 89(1), 82–93.
