for i in 1 2 5 10 20 40; do echo $i; ./start_dctest.sh $i Read_1G ; sleep 10; done
for i in 1 2 5 10 20 40; do echo $i; ./start_dctest.sh $i Read_1G_Rndm -r ; sleep 10; done
for i in 1 2 5 10 20 40; do echo $i; ./start_dctest.sh $i Read_1G_Out -o ; sleep 10; done
