#!/bin/sh
arecord -f cd -t raw | ./visualiser "$@"