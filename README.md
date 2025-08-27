[![Release](https://img.shields.io/github/v/release/marco-oliva/afm?include_prereleases)](https://github.com/marco-oliva/afm/releases/tag/1.0.0)

# Accelerated FM-Index (PFP-FM)

This repository contains an implementation of **PFP-FM**, a method for accelerating FM-index queries using **Prefix-Free Parsing (PFP)**.  
FM-indexes are central to DNA alignment, but typically require at least one random access per character in the query pattern.  
By applying **word-based indexing through PFP**, we significantly reduce the number of random accesses and speed up query performance.

Compared to previous grammar-based approaches (e.g., FIGISS), PFP-FM achieves speedups even for **shorter patterns (125–1000 bp)**, which are highly relevant in genomics.  
On datasets such as **GRCh38** and collections of **25k–100k SARS-CoV-2 genomes**, PFP-FM is consistently **2×–25× faster** than state-of-the-art methods, with only a modest increase in memory usage.

---

## Usage

### 1. Build the PFP
We use the [PFP implementation](https://github.com/marco-oliva/pfp) from the Boucher Lab

```bash
singularity pull pfp_sif docker://moliva3/pfp:latest
./pfp_sif pfp++ --help
```

## Build the AFM (PFP-FM)

```
git clone https://github.com/marco-oliva/afm.git
cd afm
mkdir build && cd build
cmake ..
make
./afm -i input_file -w window-size -p modulo -n patterns-number -l patterns-length > output.txt
```

## Docker
PFP-FM is also available via Docker:
```
docker pull aaronhong1024/afm:v1
```
# Benchmark
## Methodology
Comparison of the construction performance with the construction time (in seconds), memory usage (in gigabytes), and index size (in gigabytes)
for all datasets. The number of characters in each dataset (denoted as n) is in the second column. The implementation of the FM-index that
we used was sourced from the SDSL library.
## Specification
We ran all experiments on a server with AMD EPYC 75F3 CPU with Red Hat Enterprise Linux 7.7 (64bit, kernel 3.10.0). The code was compiled using g++ version
12.2.0. We measured running time and memory usage using the Snakemake. We set a memory limitation of 128 GB and a time limitation of 24 hours.

| Dataset    | n            | Method        | CONSTRUCT MEMORY (GB)| INDEX SIZE (GB)| CONSTRUCT TIME (s)|
|-----------|--------------|--------------|-----------------|------------|----------------|
| **SARS-25k** | 751,526,774  | RLCSA        | 9.90            | 0.026      | 322.85      |
|           |              | RLFM         | 3.47            | 0.136      | 363.74         |
|           |              | FIGISS       | 4.89            | 0.003      | 378.49         |
|           |              | PFP-FM       | 10.52           | 0.061      | 176.62         |
|           |              | PFP-FM-CSA   | 15.68           | 1.689      | 772.98         |
|           |              | FM-index     | 13.35           | 4.399      | 120.08         |
|           |              | Bowtie       | 3.55            | 0.47       | 7851.35        |
|           |              | Bowtie2      | 3.54            | 0.59       | 6847.03        |
| **SARS-50k** | 1,503,252,577 | RLCSA        | 19.88           | 0.051      | 679.89         |
|           |              | RLFM         | 6.94            | 0.278      | 701.36         |
|           |              | FIGISS       | 12.44           | 0.006      | 795.70         |
|           |              | PFP-FM       | 21.03           | 0.12       | 264.84         |
|           |              | PFP-FM-CSA   | 30.95           | 3.078      | 1546.75        |
|           |              | FM-index     | 26.12           | 8.490      | 237.50         |
|           |              | Bowtie       | 7.09            | 0.94       | 28238.74       |
|           |              | Bowtie2      | 7.09            | 1.18       | 15242.00       |
| **SARS-100k** | 3,004,588,730 | RLCSA        | 39.47           | 0.099      | 1690.22        |
|           |              | RLFM         | 25.01           | 0.571      | 1432.16        |
|           |              | FIGISS       | 25.57           | 0.009      | 1840.80        |
|           |              | PFP-FM       | 41.97           | 0.173      | 517.78         |
|           |              | PFP-FM-CSA   | 61.86           | 5.758      | 3150.72        |
|           |              | FM-index     | 51.85           | 16.73      | 434.55         |
|           |              | Bowtie       | 14.20           | 1.884      | 32143.48       |
|           |              | Bowtie2      | 14.19           | 2.37       | 33914.46       |
| **GRCh38** | 3,189,750,467 | RLCSA        | 45.45           | 2.022      | 924.60         |
|           |              | RLFM         | 26.31           | 3.101      | 1839.25        |
|           |              | FIGISS       | 34.65           | 1.538      | 1440.19        |
|           |              | PFP-FM       | 64.64           | 7.12       | 1341.75        |
|           |              | FM-index     | 70.93           | 32.54      | 877.43         |
|           |              | PFP-FM-CSA   | N/A             | N/A        | N/A            |
|           |              | Bowtie       | 13.99           | 1.833      | 2160.76        |
|           |              | Bowtie2      | 14.00           | 2.31       | 2170.32        |

## Citation

If you use **PFP-FM** in your research, please cite:

> Aaron Hong, Marco Oliva, Dominik Köppl, Hideo Bannai, Christina Boucher, and Travis Gagie. Acceleration of FM-Index Queries Through Prefix-Free Parsing. In 23rd International Workshop on Algorithms in Bioinformatics (WABI 2023). Leibniz International Proceedings in Informatics (LIPIcs), Volume 273, pp. 13:1-13:16, Schloss Dagstuhl – Leibniz-Zentrum für Informatik (2023) https://doi.org/10.4230/LIPIcs.WABI.2023.13

