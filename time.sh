#!/bin/sh
getTime(){
	cur=$(date +"%r")
	echo $cur > time.txt
}
getTime
