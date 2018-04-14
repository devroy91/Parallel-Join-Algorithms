import time
import os
import subprocess
startS = time.time()
print("running Serial nij code")
subprocess.call(['./nij_Serial'])
endS = time.time()


start = time.time()
print("running Parallel nij code")
subprocess.call(['./nij_Parallel'])
end = time.time()
print "Serial code took {} seconds".format(endS - startS)
print "Parallel code took {} seconds".format(end - start)

startS = time.time()
print("running Serial ij code")
subprocess.call(['./ij_Serial'])
endS = time.time()


start = time.time()
print("running Parallel ij code")
subprocess.call(['./ij_Parallel'])
end = time.time()
print "Serial code took {} seconds".format(endS - startS)
print "Parallel code took {} seconds".format(end - start)
