m "nfet"
l 0 8 6 8
l 6 0 6 16
l 16 0 10 0
l 10 0 10 16
l 10 16 16 16
s "G" 20 0 8
s "D" 18 16 0
s "S" 26 16 16
e
m "pfet"
i "nfet" 0 0
l 12 0 12 16
e
m "triangle"
l 0 0 0 32
l 32 16 0 32
l 32 16 0 0
e
m "hhump"
l 0 4 0 2
l 0 2 2 0
l 2 0 6 0
l 6 0 8 2
l 8 2 8 4
e
m "hinduct"
l 0 0 0 0
s "L0" 16 0 8
s "L1" 16 40 8
l 0 8 8 8
i "hhump" 8 4
i "hhump" 16 4
i "hhump" 24 4
l 32 8 40 8
l 40 16 40 16
e
m "lzener"
l 0 8 4 8
l 4 8 12 2
l 12 2 12 14
l 12 14 4 8
l 2 0 4 0
l 4 0 4 16
l 4 16 6 16
l 12 8 16 8
s "Z1" 16 0 8
s "Z0" 16 16 8
e
m "rzener"
l 16 8 12 8
l 12 8 4 2
l 4 2 4 14
l 4 14 12 8
l 14 0 12 0
l 12 0 12 16
l 12 16 10 16
l 4 8 0 8
s "Z0" 16 0 8
s "Z1" 16 16 8
e
m "vdiode"
l 8 16 8 12
l 8 12 14 4
l 14 4 2 4
l 2 4 8 12
l 16 12 0 12
l 8 4 8 0
s "D1" 16 8 16
s "D0" 16 8 0
e
m "vhump"
l 4 0 2 0
l 2 0 0 2
l 0 2 0 6
l 0 6 2 8
l 2 8 4 8
e
m "vinduct"
l 0 0 0 0
s "L0" 16 8 0
s "L1" 16 8 40
l 8 0 8 8
i "vhump" 4 8
i "vhump" 4 16
i "vhump" 4 24
l 8 32 8 40
l 16 40 16 40
e
m "xor"
l 0 8 13 8
l 0 24 13 24
l 50 16 56 16
s "Y" 22 56 16
s "B" 20 0 24
s "A" 20 0 8
l 13 11 10 0
l 13 21 13 11
l 10 32 13 21
i "or" 18 0
e
m "vres"
l 8 32 8 28
l 8 28 4 26
l 4 26 12 22
l 12 22 4 18
l 4 18 12 14
l 12 14 4 10
l 4 10 12 6
l 12 6 8 4
l 8 4 8 0
l 0 32 0 32
s "R1" 25 8 32
s "R0" 17 8 0
e
m "vcap"
l 0 8 32 8
l 0 16 32 16
l 16 16 16 24
l 16 0 16 8
s "C1" 25 16 24
s "C0" 17 16 0
e
m "tbuf"
s "Y" 22 32 16
s "A" 20 0 16
l 32 0 32 32
l 0 16 32 32
l 0 16 32 0
l 0 0 0 32
l 32 16 0 32
l 32 16 0 0
e
m "pup"
l 8 40 8 36
l 8 36 4 34
l 4 34 12 30
l 12 30 4 26
l 4 26 12 22
l 12 22 4 18
l 4 18 12 14
l 12 14 8 12
l 8 12 8 0
l 0 40 0 40
l 12 4 8 0
l 4 4 8 0
s "Y" 25 8 40
e
m "or3"
s "Y" 22 64 24
s "C" 20 0 40
s "B" 20 0 24
s "A" 20 0 8
i "lor" 8 0
l 0 24 13 24
l 0 8 11 8
l 0 40 11 40
l 56 24 64 24
e
m "or2"
s "Y" 22 48 16
s "B" 20 0 24
s "A" 20 0 8
i "or" 8 0
l 0 8 10 8
l 0 24 10 24
l 40 16 48 16
e
m "or"
l 3 11 0 0
l 3 21 3 11
l 0 32 3 21
l 16 32 0 32
l 21 29 16 32
l 25 25 21 29
l 29 21 25 25
l 32 16 29 21
l 28 9 32 16
l 23 4 28 9
l 16 0 23 4
l 0 0 16 0
l 0 0 0 0
e
m "ocnand"
s "Y" 22 40 16
s "B" 20 0 24
s "A" 20 0 8
i "bubble" 32 12
i "and" 0 0
l 24 2 24 31
e
m "nor5"
s "Y" 22 40 32
s "D" 20 0 56
s "C" 20 0 40
s "B" 20 0 24
s "A" 20 0 8
i "bubble" 32 28
i "or" 0 16
l 0 0 0 16
l 0 64 0 48
e
m "nor3"
s "Y" 22 64 24
s "C" 20 0 40
s "B" 20 0 24
s "A" 20 0 8
i "lnor" 8 0
l 0 24 13 24
l 0 40 11 40
l 0 8 11 8
e
m "nor2"
s "Y" 22 48 16
s "B" 20 0 24
s "A" 20 0 8
l 0 24 10 24
l 0 8 10 8
i "nor" 8 0
e
m "nor"
i "bubble" 32 12
i "or" 0 0
e
m "nand4"
s "Y" 22 40 32
s "D" 20 0 56
s "C" 20 0 40
s "B" 20 0 24
s "A" 20 0 8
i "bubble" 32 28
i "and" 0 16
l 0 0 0 16
l 0 64 0 48
e
m "nand3"
s "Y" 22 64 24
s "C" 20 0 40
s "B" 20 0 24
s "A" 20 0 8
i "lnand" 8 0
l 0 24 8 24
l 0 40 8 40
l 0 8 8 8
e
m "nand2"
s "Y" 22 48 16
s "B" 20 0 24
s "A" 20 0 8
l 0 24 8 24
l 0 8 8 8
i "nand" 8 0
e
m "nand"
i "bubble" 32 12
i "and" 0 0
e
m "lor"
l 5 16 0 0
l 5 32 5 16
l 0 48 5 32
l 24 48 0 48
l 32 43 24 48
l 38 38 32 43
l 43 32 38 38
l 48 24 43 32
l 42 14 48 24
l 34 6 42 14
l 24 0 34 6
l 0 0 24 0
l 0 0 0 0
e
m "lnor"
i "bubble" 48 20
i "lor" 0 0
e
m "lnand"
i "bubble" 48 20
i "land" 0 0
e
m "land"
l 0 48 0 0
l 24 48 0 48
l 32 47 24 48
l 39 42 32 47
l 45 36 39 42
l 48 28 45 36
l 48 20 48 28
l 45 12 48 20
l 39 6 45 12
l 32 1 39 6
l 24 0 32 1
l 0 0 24 0
e
m "inv2"
s "Y" 22 0 16
s "D" 20 40 16
l 40 0 40 32
l 8 16 40 32
l 8 16 40 0
i "bubble" 0 12
e
m "inv"
l 8 32 8 0
l 8 0 32 16
l 32 16 8 32
s "Y" 22 40 16
s "D" 20 0 16
i "bubble" 32 12
l 0 16 8 16
e
m "hres"
l 0 8 4 8
l 4 8 6 4
l 6 4 10 12
l 10 12 14 4
l 14 4 18 12
l 18 12 22 4
l 22 4 26 12
l 26 12 28 8
l 28 8 32 8
l 0 0 0 0
s "R1" 22 32 8
s "R0" 20 0 8
e
m "hcap"
l 12 0 12 16
l 4 0 4 16
l 12 8 16 8
l 0 8 4 8
s "C1" 22 16 8
s "C0" 20 0 8
e
m "hxtal"
l 4 0 4 16
l 20 0 20 16
l 0 8 4 8
l 20 8 24 8
l 8 4 16 4
l 16 4 16 12
l 16 12 8 12
l 8 12 8 4
s "X0" 20 0 8
s "X1" 22 24 8
e
m "vxtal"
l 0 4 16 4
l 0 20 16 20
l 8 0 8 4
l 8 20 8 24
l 4 8 4 16
l 4 16 12 16
l 12 16 12 8
l 12 8 4 8
s "X0" 20 8 0
s "X1" 22 8 24
e
m "ff"
b 0 8 48 80
l 21 76 27 76
l 24 9 24 15
l 21 12 27 12
l 8 64 0 68
l 0 60 8 64
s "D" 4 0 48
s "CK" 20 0 64
s "Q" 6 48 48
s "/Q" 6 48 64
s "/CLR" 25 24 88
s "/SET" 17 24 0
i "bubble" 20 80
i "bubble" 20 0
e
m "buf"
l 8 32 8 0
l 8 0 32 16
l 32 16 8 32
s "Y" 22 40 16
s "D" 20 0 16
l 32 16 40 16
l 0 16 8 16
e
m "pnp"
l 0 16 8 16
l 8 0 8 32
l 16 0 8 8
l 10 4 8 8
l 12 6 8 8
l 8 24 16 32
s "B" 20 0 16
s "C" 17 16 0
s "E" 25 16 32
e
m "hpnp"
l 16 16 16 8
l 0 8 32 8
l 0 0 8 8
l 4 6 8 8
l 6 4 8 8
l 24 8 32 0
s "B" 20 16 16
s "C" 17 0 0
s "E" 25 32 0
e
m "npn"
l 0 16 8 16
l 8 0 8 32
l 16 0 8 8
l 14 28 16 32
l 12 30 16 32
l 8 24 16 32
s "B" 20 0 16
s "C" 17 16 0
s "E" 25 16 32
e
m "vpot"
i "vres" 0 0
s "Y" 22 24 16
l 17 13 12 16
l 17 19 12 16
l 12 16 24 16
e
m "opamp"
l 8 32 8 0
l 8 0 32 16
l 32 16 8 32
s "OUT" 22 40 16
s "IN-" 20 0 8
s "IN+" 20 0 24
l 10 8 15 8
l 10 24 15 24
l 12 22 12 27
l 32 16 40 16
l 0 8 8 8
l 0 24 8 24
e
m "bubble"
l 0 2 2 0
l 0 6 0 2
l 2 8 0 6
l 6 8 2 8
l 8 6 6 8
l 8 2 8 6
l 6 0 8 2
l 2 0 6 0
e
m "and4"
s "Y" 22 32 32
s "D" 20 0 56
s "C" 20 0 40
s "B" 20 0 24
s "A" 20 0 8
i "and" 0 16
l 0 0 0 16
l 0 64 0 48
e
m "and3"
s "Y" 22 64 24
s "C" 20 0 40
s "B" 20 0 24
s "A" 20 0 8
i "land" 8 0
l 0 24 8 24
l 0 8 8 8
l 0 40 8 40
l 56 24 64 24
e
m "and2"
s "Y" 22 48 16
s "B" 20 0 24
s "A" 20 0 8
i "and" 8 0
l 0 8 8 8
l 0 24 8 24
l 40 16 48 16
e
m "and"
l 0 32 0 0
l 16 32 0 32
l 22 31 16 32
l 26 28 21 31
l 30 24 26 28
l 32 19 30 24
l 32 13 32 19
l 30 8 32 13
l 26 4 30 8
l 21 1 26 4
l 16 0 21 1
l 0 0 16 0
e
m "3sinvbar"
s "G-" 20 16 32
s "Y-" 22 40 16
s "I" 17 0 16
i "bubble" 32 12
i "triangle" 0 0
i "bubble" 12 24
e
m "3sinv2bar"
s "G-" 17 16 0
s "Y-" 22 40 16
s "D" 20 0 16
i "bubble" 32 12
i "triangle" 0 0
i "bubble" 12 0
e
m "3sinv2"
s "G" 17 16 32
s "Y-" 22 40 16
s "D" 20 0 16
l 16 24 16 32
i "bubble" 32 12
i "triangle" 0 0
e
m "3sinv"
s "G" 17 16 0
s "Y-" 22 40 16
s "D" 20 0 16
i "bubble" 32 12
i "triangle" 0 0
l 16 0 16 8
e
m "3sbuf"
s "G" 17 16 32
s "Y" 22 32 16
s "D" 20 0 16
l 16 24 16 32
i "triangle" 0 0
e
m "/or3"
s "Y" 22 40 24
s "C" 20 0 40
s "B" 20 0 24
s "A" 20 0 8
l 8 40 8 48
l 8 0 8 8
i "bubble" 40 20
i "bubble" 0 4
i "bubble" 0 20
i "bubble" 0 36
i "and" 8 8
e
m "/or"
i "bubble" 0 20
i "bubble" 0 4
i "bubble" 40 12
i "and" 8 0
e
m "/nor5"
s "Y" 22 40 40
s "E" 20 0 72
s "D" 20 0 56
s "C" 20 0 40
s "B" 20 0 24
s "A" 20 0 8
l 8 56 8 80
l 8 0 8 24
i "and" 8 24
i "bubble" 0 4
i "bubble" 0 20
i "bubble" 0 36
i "bubble" 0 52
i "bubble" 0 68
e
m "/nor3"
s "Y" 22 40 24
s "C" 20 0 40
s "B" 20 0 24
s "A" 20 0 8
l 8 40 8 48
l 8 0 8 8
i "bubble" 0 4
i "bubble" 0 20
i "bubble" 0 36
i "and" 8 8
e
m "/nor"
i "bubble" 0 4
i "bubble" 0 20
i "and" 8 0
e
m "/nand3"
i "or3" 8 0
i "bubble" 0 4
i "bubble" 0 20
i "bubble" 0 36
e
m "/nand"
i "bubble" 0 20
i "bubble" 0 4
i "or" 8 0
e
m "/invert"
s "Y" 22 40 16
s "I" 16 0 16
i "triangle" 8 0
i "bubble" 0 12
e
m "/and3"
s "Y" 22 40 24
s "C" 20 0 40
s "B" 20 0 24
s "A" 20 0 8
l 8 40 8 48
l 8 0 8 8
i "bubble" 40 20
i "bubble" 0 36
i "bubble" 0 20
i "bubble" 0 4
i "or" 8 8
e
m "/and"
i "bubble" 40 12
i "bubble" 0 20
i "bubble" 0 4
i "or" 8 0
e
