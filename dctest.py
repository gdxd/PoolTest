#!/usr/bin/env python
#
import sys, commands, os, random, time, optparse

#Pfad von dem dcRead-Programm mit Optionen: -o timeout -B Buffersize -d Laenge der Ausgabe

try:
    dcdir = os.environ['DCTESTDIR']
except:
    print "Error: environment DCTESTDIR not set, exiting"
    sys.exit(1)



parser = optparse.OptionParser()

#Einlesen der Information, wie viele dctest.py-Programme gleichzeitig laufen sollen...
parser.add_option('-l', '--filelist', type='string', help='list of files to read')
parser.add_option('-s', '--start', default=0, type='int', help='start-time')
parser.add_option('-e', '--end', default=0, type='int', help='end-time')
parser.add_option('-o', '--output', default=0, type='int', help='write output file') 
parser.add_option('-f', '--fracout', default=0.5, type='float', help='fraction to write output file')

# parser.add_option('-r', '--rndseek', default=0, type='int', help='random seek read')


options,arguments = parser.parse_args()


output = options.output


dc_cmd_base = dcdir + "/file_read -M 1000  "


print "Executing : ", dc_cmd_base

try:
    flist = open(options.filelist).readlines()
except:
    print "Error: problems with filelist: ", options.filelist
    sys.exit(2)

# wait for start-time
if options.start>0 :
    # tnow = gettimenow
    while options.start>time.time() :
        time.sleep(10)

# if end time not set run for 100 s
if options.end<=options.start :
    options.end = time.time() + 100


# directories
dirs=list(set([ os.path.dirname(f) for f in flist]))

#Zeitausgabe
print "Start", time.asctime()
print '### Start: ', time.time()
stop = False
while True:
    #randomize list
    random.shuffle(flist)

    for f in flist :
        f = f.strip()
        # stop if time limit reached
        if  options.end < time.time() :
            print '### Abbruch', time.time()
            stop = True
            break

	dowrite = output > 0 and random.random() > options.fracout
	# construct random output file
	if dowrite :
	    odir = random.choice(dirs)
	    ofile = "%s/out.%d" % (odir, random.randint(0,100000000))
	    dc_cmd = dc_cmd_base  + " -o " + ofile + " "
	else:
	    dc_cmd = dc_cmd_base

        print "Executing %s %s " % ( dc_cmd, f )
        
        rc1, out1 = commands.getstatusoutput(dc_cmd + f)

#        print rc1, out1
        
        if rc1 == 0 :
	    if dowrite : os.remove(ofile)
            sl=out1.split()
            #Versuche aus der Ausgabe die angesprochene Pool-Node zu isolieren
            try:
                readbytes=int(sl[0])
		writebytes = int(sl[1])
                readtime=float(sl[2])
                readfile=sl[-1]
                                        
            except:
                print("Trouble parsing output:", out1 )
                
            print "### ", rc1, readbytes, writebytes, readtime, readfile

            print '### time: ', time.time()
        else:
            print '512 ', rc1, out1
    # stop-flag ?
    if stop :
        break

    # next round thru filelist
    print "iterating file-list"
#


print "End: ", time.asctime()
print '### Start: ', time.time()
