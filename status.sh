#!/bin/bash

total=$(tail -1 report.md | cut -d '/' -f 1)
max=$(tail -1 report.md | cut -d '/' -f 2)
if [ "$total" -lt "$max" ]
then
	 exit 1
fi
