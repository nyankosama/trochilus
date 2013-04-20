#! /bin/bash 
ab -c 400 -n 400 http://localhost:8000/index.html > out.txt
