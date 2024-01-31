cmake_minimum_required(VERSION 3.22)

set(Data_Target essnusb_data)

set(Data_Header FgdDetectorPoint.h
                            FgdHit.h)

add_library(${Data_Target} SHARED)

target_sources(${Data_Target} PUBLIC 
            FgdDetectorPoint.cxx
            FgdHit.cxx)

LIBRARY_FOOTER(${Data_Target} ${Data_Header})
