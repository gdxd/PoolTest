test=lcg-lrz-xcache0.Read_1G.v2
for i in  1 2 5 10 20 40; do cat $test.$i/* |  grep '###  0' | awk '{ sz += $3; st += $4 } END { print '$i', sz/st/1e6*'$i' }'; done
