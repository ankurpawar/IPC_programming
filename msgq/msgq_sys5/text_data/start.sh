#!/bin/bash
#
# Script to run process1 and process2 together
#
./process2 & #send process2 in background
./process1
