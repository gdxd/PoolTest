test=lcg-lrz-xcache2.Read_1G
for i in  1 2 5 10 20 40; do cat $test.$i/* |  grep '###  0' | awk '{ sz += $3 + $4; st += $5 } END { print '$i', sz/st/1e6*'$i' }'; done
