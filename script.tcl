############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project hls_taus
set_top myproject
add_files hls_taus/BDT.h -cflags "-std=c++0x"
#add_files hls_taus/PF_Tau.cc
#add_files hls_taus/PF_Tau.hpp
add_files hls_taus/myproject.cpp -cflags "-std=c++0x"
add_files hls_taus/myproject.h
add_files -tb hls_taus/testbench.cpp -cflags "-std=c++0x"
add_files hls_taus/parameters.h
open_solution "solution1"
set_part {xc7vx690tffg1927-2} -tool vivado
create_clock -period 4.12 -name default
#source "./hls_taus/solution1/directives.tcl"
#csim_design -argv {-std=c++11} -compiler gcc
csim_design
csynth_design
#cosim_design
export_design -format ip_catalog
exit
