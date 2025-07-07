c --- [ banner ] -------------------------------------------------------------
c 
c CaDiCaL Radically Simplified CDCL SAT Solver
c Copyright (c) 2016-2021 A. Biere, M. Fleury, N. Froleyks
c 
c Version 1.5.2 2cc1496eafad50a42d3898748eb47f7acffaade7
c g++ (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0 -Wall -Wextra -O3 -DNDEBUG
c Thu May 29 20:42:27 EDT 2025 Linux HKwappytoppy 5.15.167.4-microsoft-standard-WSL2 x86_64
c 
c --- [ parsing input ] ------------------------------------------------------
c 
c reading DIMACS file from 'k6ram.cnf'
c opening file to read 'k6ram.cnf'
c found 'p cnf 300 354234' header
c found falsified original clause
c parsed 354234 clauses in 0.16 seconds process time
c closing file 'k6ram.cnf'
c after reading 13885248 bytes 13.2 MB
c 
c --- [ options ] ------------------------------------------------------------
c 
c   --allsat=true                  (different from default 'false')
c 
c --- [ solving ] ------------------------------------------------------------
c 
c 
c --- [ result ] -------------------------------------------------------------
c 
s 0 SOLUTIONS
c 
c --- [ run-time profiling ] -------------------------------------------------
c 
c process time taken by individual solving procedures
c (percentage relative to process time for solving)
c 
c         0.16 15620200.00% parse
c         0.00    0.00% search
c         0.00    0.00% simplify
c   =================================
c         0.00    0.00% solve
c 
c last line shows process time for solving
c (percentage relative to total process time)
c 
c --- [ statistics ] ---------------------------------------------------------
c 
c fixed:                        28         9.33 %  of all variables
c minimized:                     0         0.00 %  learned literals
c shrunken:                      0         0.00 %  learned literals
c minishrunken:                  0         0.00 %  learned literals
c propagations:                 28        28.00 M  per second
c 
c seconds are measured in process time for solving
c 
c --- [ resources ] ----------------------------------------------------------
c 
c total process time since initialization:         0.16    seconds
c total real time since initialization:            0.16    seconds
c maximum resident set size of process:           40.55    MB
c 
c --- [ shutting down ] ------------------------------------------------------
c 
c exit 20
