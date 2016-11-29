#!/bin/sh
getTime(){
	now=$(date +"%r")
	echo $now > time.txt
}
getTime
