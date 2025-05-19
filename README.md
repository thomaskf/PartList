# PartList

**PartList** is a simple command-line tool that lists the partitions and corresponding edge lengths for each edge in a given phylogenetic tree.

## Features

- Parses a Newick-formatted tree
- Outputs a table showing each bipartition (split) and its edge length
- Outputs partitions in lexicographical order for consistency

## Compilation

To compile the program, use:

```bash
g++ partlist.cpp -o partlist
```

## Usage

```bash
./partlist [tree_file]
```

- `tree_file`: A text file containing a tree in Newick format.

## Example

**Input file**: `example.treefile`

```
(protopterus:0.3099067061,((python:0.0963168749,(Taeniopygia:0.0641893111,(caiman:0.0134032165,(caretta:0.0105108952,phrynops:0.0269820819):0.0371040668):0.0204716388):0.0144828411):0.0360088912,Ornithorhynchus:0.1286353020):0.0905913632,Xenopus:0.3075276653);
```

**Command**:

```bash
./partlist example.treefile
```

**Output**:

| Partition | Edge length |
|-----------|-------------|
| Ornithorhynchus,Taeniopygia,Xenopus,caiman,caretta,phrynops,protopterus::python	| 0.096317 |
| Ornithorhynchus,Taeniopygia,Xenopus,caiman,caretta,phrynops,python::protopterus	| 0.309907 |
| Ornithorhynchus,Taeniopygia,Xenopus,caiman,caretta,protopterus,python::phrynops	| 0.026982 |
| Ornithorhynchus,Taeniopygia,Xenopus,caiman,phrynops,protopterus,python::caretta	| 0.010511 |
| Ornithorhynchus,Taeniopygia,Xenopus,caiman,protopterus,python::caretta,phrynops	| 0.037104 |
| Ornithorhynchus,Taeniopygia,Xenopus,caretta,phrynops,protopterus,python::caiman	| 0.013403 |
| ... *(remaining rows omitted for clarity)* ...

Each row represents a bipartition induced by an internal edge of the tree, with the two sets of taxa separated by `::`.

## License

This project is released under the MIT License.
