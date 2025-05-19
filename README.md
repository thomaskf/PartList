# PartList
Shows the partition and the length of each edge in a given tree

# Compilation
g++ partlist.cpp -o partlist

# Syntax
`partlist [tree_file]`

# Example

example.treefile:
```
(protopterus:0.3099067061,((python:0.0963168749,(Taeniopygia:0.0641893111,(caiman:0.0134032165,((chelonoidis_nigra:0.0125863554,caretta:0.0105108952):0.0104962915,phrynops:0.0269820819):0.0371040668):0.0204716388):0.0144828411):0.0360088912,(Homo:0.1054232027,Ornithorhynchus:0.1286353020):0.0545461797):0.0905913632,Xenopus:0.3075276653);
```

Command:
```
partlist example.treefile
```

Output:
|Partition|Edge length|
|---------|-----------|
|Homo,Ornithorhynchus,Taeniopygia,Xenopus,caiman,caretta,chelonoidis_nigra,phrynops,protopterus::python|0.0963169|
|Homo,Ornithorhynchus,Taeniopygia,Xenopus,caiman,caretta,chelonoidis_nigra,phrynops,python::protopterus|0.309907|
|Homo,Ornithorhynchus,Taeniopygia,Xenopus,caiman,caretta,chelonoidis_nigra,protopterus,python::phrynops|0.0269821|
|Homo,Ornithorhynchus,Taeniopygia,Xenopus,caiman,caretta,phrynops,protopterus,python::chelonoidis_nigra|0.0125864|
|Homo,Ornithorhynchus,Taeniopygia,Xenopus,caiman,chelonoidis_nigra,phrynops,protopterus,python::caretta|0.0105109|
|Homo,Ornithorhynchus,Taeniopygia,Xenopus,caiman,phrynops,protopterus,python::caretta,chelonoidis_nigra|0.0104963|
|Homo,Ornithorhynchus,Taeniopygia,Xenopus,caiman,protopterus,python::caretta,chelonoidis_nigra,phrynops|0.0371041|
|Homo,Ornithorhynchus,Taeniopygia,Xenopus,caretta,chelonoidis_nigra,phrynops,protopterus,python::caiman|0.0134032|
|Homo,Ornithorhynchus,Taeniopygia,Xenopus,protopterus,python::caiman,caretta,chelonoidis_nigra,phrynops|0.0204716|
|Homo,Ornithorhynchus,Taeniopygia,caiman,caretta,chelonoidis_nigra,phrynops,protopterus,python::Xenopus|0.307528|
|Homo,Ornithorhynchus,Taeniopygia,caiman,caretta,chelonoidis_nigra,phrynops,python::Xenopus,protopterus|0.0905914|
|Homo,Ornithorhynchus,Xenopus,caiman,caretta,chelonoidis_nigra,phrynops,protopterus,python::Taeniopygia|0.0641893|
|Homo,Ornithorhynchus,Xenopus,protopterus,python::Taeniopygia,caiman,caretta,chelonoidis_nigra,phrynops|0.0144828|
|Homo,Ornithorhynchus,Xenopus,protopterus::Taeniopygia,caiman,caretta,chelonoidis_nigra,phrynops,python|0.0360089|
|Homo,Ornithorhynchus::Taeniopygia,Xenopus,caiman,caretta,chelonoidis_nigra,phrynops,protopterus,python|0.0545462|
|Homo,Taeniopygia,Xenopus,caiman,caretta,chelonoidis_nigra,phrynops,protopterus,python::Ornithorhynchus|0.128635|
|Homo::Ornithorhynchus,Taeniopygia,Xenopus,caiman,caretta,chelonoidis_nigra,phrynops,protopterus,python|0.105423|
