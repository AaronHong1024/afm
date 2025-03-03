[![Release](https://img.shields.io/github/v/release/marco-oliva/afm?include_prereleases)](https://github.com/marco-oliva/afm/releases/tag/1.0.0)

# Accelerated FM-index

FM-indexes are a crucial data structure in DNA alignment, but searching with them usually takes at least one random access per character in the query pattern.  Ferragina and Fischer observed in 2007 that word-based indexes often use fewer random accesses than character-based indexes, and thus support faster searches.  Since DNA lacks natural word-boundaries, however, it is necessary to parse it somehow before applying word-based FM-indexing.  Last year, Deng et al. proposed parsing genomic data by induced suffix sorting, and showed the resulting word-based FM-indexes support faster counting queries than standard FM-indexes when patterns are a few thousand characters or longer.  In this paper we show that using prefix-free parsing---which takes parameters that let us tune the average length of the phrases—instead of induced suffix sorting, gives a significant speedup for patterns of only a few hundred characters.  We implement our method and demonstrate it is between 3 and 18 times faster than competing methods on queries to GRCh38.  And was consistently faster on queries made to 25,000, 50,000 and 100,000 SARS-CoV-2 genomes. Hence, it is very clear that our method accelerates the performance of count over all state-of-the-art methods with a minor increase in the memory.

If you use the AFM in your research, please cite: (https://arxiv.org/abs/2305.05893)

Hong, Aaron., Oliva, Marco., Köppl, Dominik., Bannai, Hideo., Boucher, Christina., & Gagie, Travis. (2023). Acceleration of FM-index Queries Through Prefix-free Parsing. ArXiv. /abs/2305.05893

# Usage

## Build the PFP
We used the PFP implementation from the Boucher lab at the University of Florida. The GitHub link is: (https://github.com/marco-oliva/pfp). To build the PFP, run the following commands:
```
singularity pull pfp_sif docker://moliva3/pfp:latest
./pfp_sif pfp++ --help
```
For more details, please refer to the PFP GitHub page.

## Build the AFM
Then we can build the afm by running the following commands:
```
git clone https://github.com/marco-oliva/afm.git
cd afm
mkdir build
cd build
cmake ..
make
./afm -i input_file -w window-size -p modulo -n patterns-number -l patterns-length > output.txt
```
## Docker
AFM is avaliable on docker:
```
docker pull aaronhong1024/afm:v1
```
## Benchmark
| Dataset    | n            | Method        | CONSTRUCT MEMORY | INDEX SIZE | CONSTRUCT TIME |
|-----------|--------------|--------------|-----------------|------------|----------------|
| **SARS-25k** | 751,526,774  | RLCSA        | 9.90            | 0.026      | 322.85         |
|           |              | RLFM         | 3.47            | 0.136      | 363.74         |
|           |              | FIGISS       | 4.89            | 0.003      | 378.49         |
|           |              | \ours        | 10.52           | 0.061      | 176.62         |
|           |              | PFP-FM-CSA   | 15.68           | 1.689      | 772.98         |
|           |              | FM-index     | 13.35           | 4.399      | 120.08         |
|           |              | Bowtie       | 3.55            | 0.47       | 7851.35        |
|           |              | Bowtie2      | 3.54            | 0.59       | 6847.03        |
| **SARS-50k** | 1,503,252,577 | RLCSA        | 19.88           | 0.051      | 679.89         |
|           |              | RLFM         | 6.94            | 0.278      | 701.36         |
|           |              | FIGISS       | 12.44           | 0.006      | 795.70         |
|           |              | \ours        | 21.03           | 0.12       | 264.84         |
|           |              | PFP-FM-CSA   | 30.95           | 3.078      | 1546.75        |
|           |              | FM-index     | 26.12           | 8.490      | 237.50         |
|           |              | Bowtie       | 7.09            | 0.94       | 28238.74       |
|           |              | Bowtie2      | 7.09            | 1.18       | 15242.00       |
| **SARS-100k** | 3,004,588,730 | RLCSA        | 39.47           | 0.099      | 1690.22        |
|           |              | RLFM         | 25.01           | 0.571      | 1432.16        |
|           |              | FIGISS       | 25.57           | 0.009      | 1840.80        |
|           |              | \ours        | 41.97           | 0.173      | 517.78         |
|           |              | PFP-FM-CSA   | 61.86           | 5.758      | 3150.72        |
|           |              | FM-index     | 51.85           | 16.73      | 434.55         |
|           |              | Bowtie       | 14.20           | 1.884      | 32143.48       |
|           |              | Bowtie2      | 14.19           | 2.37       | 33914.46       |
| **GRCh38** | 3,189,750,467 | RLCSA        | 45.45           | 2.022      | 924.60         |
|           |              | RLFM         | 26.31           | 3.101      | 1839.25        |
|           |              | FIGISS       | 34.65           | 1.538      | 1440.19        |
|           |              | \ours        | 64.64           | 7.12       | 1341.75        |
|           |              | FM-index     | 70.93           | 32.54      | 877.43         |
|           |              | PFP-FM-CSA   | N/A             | N/A        | N/A            |
|           |              | Bowtie       | 13.99           | 1.833      | 2160.76        |
|           |              | Bowtie2      | 14.00           | 2.31       | 2170.32        |


# Author
* Aaron Hong
* Marco Oliva
* Dominik Köppl
* Hideo Bannai
* Christina Boucher
* Travis Gagie

# Reference
[1] Boucher, C., Gagie, T., Kuhnle, A. et al. Prefix-free parsing for building big BWTs. Algorithms Mol Biol 14, 13 (2019). https://doi.org/10.1186/s13015-019-0148-5
