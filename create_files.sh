#
# on lcg-lrz-dc63
#
for ding in /data/a/*; do echo $ding; a=0; while [ $a -lt 10 ]; do dd bs=1M count=1200 if=/data/a/1/0000A9BDFCB6241E4ECF8289CFA3D244351A of=$ding/test.$a; let a=a+1; echo $a; done; done
for ding in /data/b/*; do echo $ding; a=0; while [ $a -lt 10 ]; do dd bs=1M count=1200 if=/data/a/1/0000A9BDFCB6241E4ECF8289CFA3D244351A of=$ding/test.$a; let a=a+1; echo $a; done; done
