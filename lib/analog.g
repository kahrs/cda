m "Vplus"
s "+" 16 0 16
s "-" 16 40 16
l 24 16 40 16
l 24 8 24 24
l 16 0 16 32
l 0 16 16 16
e
m "Vminus"
s "-" 16 0 16
s "+" 16 40 16
l 24 16 40 16
l 16 8 16 24
l 24 0 24 32
l 0 16 16 16
e
m "b_inductor"
s "L1" 16 0 8
s "L2" 16 64 8
l 0 8 8 8
i "bhump" 8 0
i "bhump" 20 0
i "bhump" 32 0
i "bhump" 44 0
l 56 4 56 8
l 56 8 64 8
e
m "bhump"
l 0 8 0 4
l 4 0 0 4
l 4 0 8 0
l 12 4 8 0
e
m "bl_npn"
s "B" 16 0 24
s "C" 16 32 0
s "E" 16 32 48
i "lbjt" 0 0
l 26 46 30 46
l 30 42 30 46
e
m "bl_pnp"
s "B" 16 0 24
s "C" 16 32 0
s "E" 16 32 48
i "lbjt" 0 0
l 30 42 26 42
l 26 46 26 42
e
m "bl_nmosfet"
s "G" 16 0 16
s "D" 16 40 24
s "S" 16 40 8
i "lmosfet" 0 0
l 32 28 36 24
l 32 20 36 24
e
m "br_nmosfet"
s "G" 16 40 16
s "D" 16 0 24
s "S" 16 0 8
i "rmosfet" 0 0
l 4 28 8 24
l 4 20 8 24
e
m "bl_pmosfet"
s "G" 16 0 16
s "D" 16 40 24
s "S" 16 40 8
i "lmosfet" 0 0
l 36 28 32 24
l 36 20 32 24
e
m "br_pmosfet"
s "G" 16 40 16
s "D" 16 0 24
s "S" 16 0 8
i "rmosfet" 0 0
l 8 28 4 24
l 8 20 4 24
e
m "br_npn"
s "B" 16 32 24
s "C" 16 0 48
s "E" 16 0 0
i "rbjt" 0 0
l 4 40 4 44
l 8 44 4 44
e
m "br_pnp"
s "B" 16 32 24
s "C" 16 0 48
s "E" 16 0 0
i "rbjt" 0 0
l 4 40 4 44
l 8 44 4 44
e
m "gnd"
s "GNDA" 16 8 0
l 8 0 8 8
l 0 8 16 8
l 4 12 12 12
l 6 16 10 16
e
m "hl_diode"
s "A" 16 32 8
s "C" 16 0 8
l 32 8 24 8
l 24 0 24 16
l 24 0 8 8
l 24 16 8 8
l 8 0 8 16
l 8 8 0 8
e
m "hr_diode"
s "C" 16 32 8
s "A" 16 0 8
l 0 8 8 8
l 8 0 8 16
l 8 0 24 8
l 8 16 24 8
l 24 0 24 16
l 24 8 32 8
e
m "h_resistor"
s "R1" 16 0 8
s "R2" 16 32 8
l 0 0 0 0
l 28 8 32 8
l 26 12 28 8
l 22 4 26 12
l 18 12 22 4
l 14 4 18 12
l 10 12 14 4
l 6 4 10 12
l 4 8 6 4
l 0 8 4 8
e
m "h_cap"
s "C1" 16 0 8
s "C2" 16 16 8
l 0 8 6 8
l 10 8 16 8
l 6 0 6 16
l 10 0 10 16
e
m "l_ecap"
s "C+" 16 0 8
s "C-" 16 16 8
l 0 8 6 8
l 10 8 16 8
l 6 0 6 16
l 10 0 10 16
l 6 18 6 23
l 4 20 9 20
e
m "r_ecap"
s "C-" 16 0 8
s "C+" 16 16 8
l 0 8 6 8
l 10 8 16 8
l 6 0 6 16
l 10 0 10 16
l 10 18 10 23
l 8 20 13 20
e
m "lbjt"
l 32 0 16 16
l 16 32 32 48
l 16 24 0 24
l 16 44 16 4
e
m "lmosfet"
l 0 16 16 16
l 16 24 16 8
l 24 32 24 0
l 24 24 40 24
l 24 8 40 8
e
m "rbjt"
l 0 0 16 16
l 16 32 0 48
l 16 24 32 24
l 16 44 16 4
e
m "rmosfet"
l 0 24 16 24
l 0 8 16 8
l 16 32 16 0
l 24 24 24 8
l 24 16 40 16
e
m "t_inductor"
s "L1" 16 0 0
s "L2" 16 64 0
l 0 0 8 0
i "thump" 8 0
i "thump" 20 0
i "thump" 32 0
i "thump" 44 0
l 56 4 56 0
l 56 0 64 0
e
m "thump"
l 0 0 0 4
l 4 8 0 4
l 4 8 8 8
l 8 8 12 4
e
m "tl_npn"
s "B" 16 0 24
s "E" 16 32 0
s "C" 16 32 48
i "lbjt" 0 0
l 26 2 30 2
l 30 6 30 2
e
m "tl_pnp"
s "B" 16 0 24
s "E" 16 32 0
s "C" 16 32 48
i "lbjt" 0 0
l 26 2 26 6
l 30 6 26 6
e
m "tl_nmosfet"
s "G" 16 0 16
s "S" 16 40 24
s "D" 16 40 8
i "lmosfet" 0 0
l 32 12 36 8
l 32 4 36 8
e
m "tr_nmosfet"
s "G" 16 40 16
s "S" 16 0 24
s "D" 16 0 8
i "rmosfet" 0 0
l 4 12 8 8
l 4 4 8 8
e
m "tl_pmosfet"
s "G" 16 0 16
s "S" 16 40 24
s "D" 16 40 8
i "lmosfet" 0 0
l 36 12 32 8
l 36 4 32 8
e
m "tr_pmosfet"
s "G" 16 40 16
s "S" 16 0 24
s "D" 16 0 8
i "rmosfet" 0 0
l 8 12 4 8
l 8 4 4 8
e
m "tr_npn"
s "B" 16 32 24
s "E" 16 0 48
s "C" 16 0 0
i "rbjt" 0 0
l 4 8 4 4
l 8 4 4 4
e
m "tr_pnp"
s "B" 16 32 24
s "E" 16 0 48
s "C" 16 0 0
i "rbjt" 0 0
l 4 8 8 8
l 8 4 8 8
e
m "vb_diode"
s "A" 16 8 0
s "C" 16 8 32
l 8 0 8 8
l 0 8 16 8
l 0 8 8 24
l 16 8 8 24
l 0 24 16 24
l 8 24 8 32
e
m "v_resistor"
s "R1" 16 8 0
s "R2" 16 8 32
l 0 32 0 32
l 8 4 8 0
l 12 6 8 4
l 4 10 12 6
l 12 14 4 10
l 4 18 12 14
l 12 22 4 18
l 4 26 12 22
l 8 28 4 26
l 8 32 8 28
e
m "vt_diode"
s "C" 16 8 0
s "A" 16 8 32
l 8 32 8 24
l 0 24 16 24
l 0 24 8 8
l 16 24 8 8
l 0 8 16 8
l 8 8 8 0
e
m "v_cap"
s "C1" 20 8 0
s "C2" 20 8 16
l 8 0 8 4
l 0 4 16 4
l 0 12 16 12
l 8 12 8 16
e
m "b_ecap"
s "C-" 16 8 0
s "C+" 16 8 16
l 8 0 8 4
l 0 4 16 4
l 0 12 16 12
l 8 12 8 16
l 20 12 25 12
l 22 10 22 15
e
m "t_ecap"
s "C+" 16 8 0
s "C-" 16 8 16
l 8 0 8 4
l 0 4 16 4
l 0 12 16 12
l 8 12 8 16
l 20 4 25 4
l 22 2 22 7
e
m "bigbuf"
l 0 0 0 48
l 40 24 0 0
l 0 48 40 24
e
m "h_pot"
s "R1" 16 0 8
s "R2" 16 16 32
s "R3" 16 32 8
l 32 0 32 0
l 4 8 0 8
l 6 12 4 8
l 10 4 6 12
l 14 12 10 4
l 18 4 14 12
l 22 12 18 4
l 26 4 22 12
l 28 8 26 4
l 32 8 28 8
l 16 12 20 16
l 16 12 16 32
l 16 12 12 16
e
m "v_pot"
s "R1" 16 8 0
s "R2" 16 32 16
s "R3" 16 8 32
l 0 32 0 32
l 8 4 8 0
l 12 6 8 4
l 4 10 12 6
l 12 14 4 10
l 4 18 12 14
l 12 22 4 18
l 4 26 12 22
l 8 28 4 26
l 8 32 8 28
l 12 16 16 20
l 12 16 32 16
l 12 16 16 12
e
m "lhump"
l 8 0 4 0
l 4 0 0 4
l 0 4 0 8
l 0 8 4 12
e
m "l_inductor"
s "L1" 16 8 0
s "L2" 16 8 64
l 8 0 8 8
i "lhump" 0 8
i "lhump" 0 20
i "lhump" 0 32
i "lhump" 0 44
l 4 56 8 56
l 8 56 8 64
e
m "r_inductor"
s "L1" 16 0 0
s "L2" 16 0 64
l 0 0 0 8
i "rhump" 0 8
i "rhump" 0 20
i "rhump" 0 32
i "rhump" 0 44
l 4 56 0 56
l 0 56 0 64
e
m "rhump"
l 0 0 4 0
l 4 0 8 4
l 8 4 8 8
l 8 8 4 12
e
m "v_transformer"
s "X12" 16 0 0
s "X21" 16 0 64
s "X11" 16 40 0
s "X22" 16 40 64
l 0 0 0 8
i "rhump" 0 8
i "rhump" 0 20
i "rhump" 0 32
i "rhump" 0 44
l 4 56 0 56
l 0 56 0 64
l 40 0 40 8
i "lhump" 32 8
i "lhump" 32 20
i "lhump" 32 32
i "lhump" 32 44
l 36 56 40 56
l 40 56 40 64
l 24 8 24 56
l 16 8 16 56
e
m "v_air_transformer"
s "X12" 16 0 0
s "X21" 16 0 64
s "X11" 16 32 0
s "X22" 16 32 64
l 0 0 0 8
i "rhump" 0 8
i "rhump" 0 20
i "rhump" 0 32
i "rhump" 0 44
l 4 56 0 56
l 0 56 0 64
l 32 0 32 8
i "lhump" 24 8
i "lhump" 24 20
i "lhump" 24 32
i "lhump" 24 44
l 28 56 32 56
l 32 56 32 64
e
m "h_air_transformer"
s "X11" 16 0 0
s "X21" 16 64 0
l 0 0 8 0
i "thump" 8 0
i "thump" 20 0
i "thump" 32 0
i "thump" 44 0
l 56 4 56 0
l 56 0 64 0
s "X10" 16 0 32
s "X11" 16 64 32
l 0 32 8 32
i "bhump" 8 24
i "bhump" 20 24
i "bhump" 32 24
i "bhump" 44 24
l 56 28 56 32
l 56 32 64 32
e
m "r_terminal"
s "P" 16 16 8
l 0 0 0 16
l 0 16 16 16
l 16 16 16 0
l 16 0 0 0
e
m "l_terminal"
s "P" 16 0 8
l 0 0 0 16
l 0 16 16 16
l 16 16 16 0
l 16 0 0 0
e
m "_circle4"
l 1 22 0 19
l 2 24 1 22
l 4 27 2 24
l 5 28 4 27
l 8 30 5 28
l 10 31 8 30
l 13 32 10 31
l 19 32 13 32
l 22 31 19 32
l 24 30 22 31
l 27 28 24 30
l 28 27 27 28
l 30 24 28 27
l 31 22 30 24
l 32 19 31 22
l 32 13 32 19
l 31 10 32 13
l 30 8 31 10
l 28 5 30 8
l 27 4 28 5
l 24 2 27 4
l 22 1 24 2
l 19 0 22 1
l 13 0 19 0
l 10 1 13 0
l 8 2 10 1
l 5 4 8 2
l 4 5 5 4
l 2 8 4 5
l 1 10 2 8
l 0 13 1 10
l 0 19 0 13
l 0 0 0 0
e
m "vl_currentsrc"
l 32 0 32 16
l 32 80 32 64
i "_circle4" 16 16
i "_circle4" 16 32
l 0 40 16 40
s "V+" 16 32 0
s "V-" 16 32 80
s "R" 16 0 40
e
m "r_opamp"
s "Y" 16 64 32
s "B" 16 0 48
s "A" 16 0 16
l 8 48 16 48
l 12 12 12 20
l 8 16 16 16
l 64 32 0 64
l 0 0 64 32
l 0 0 0 64
e
m "l_opamp"
l 0 32 64 0
l 64 0 64 64
l 64 64 0 32
l 48 48 56 48
l 48 16 56 16
l 52 12 52 20
s "Y" 16 0 32
s "B" 16 64 48
s "A" 16 64 16
e
