file = open('nuFlux100km_250kAm.txt', 'r')
fileE = open('nuFlux100km_250kAm_e.txt', 'w+')
fileAE = open('nuFlux100km_250kAm_anti_e.txt', 'w+')
fileMu = open('nuFlux100km_250kAm_mu.txt', 'w+')
fileAMu = open('nuFlux100km_250kAm_anti_mu.txt', 'w+')
Lines = file.readlines()
for line in Lines:
    #print("Line{}:{}".format(0,line.strip()))
    tokens=line.split()
    fileE.write("{}  {}  0  0  0  0  0 \n".format(tokens[0],tokens[1]))
    fileAE.write("{} 0  0  0  {}  0  0\n".format(tokens[0],tokens[4]))
    fileMu.write("{} 0  {}  0  0  0  0\n".format(tokens[0],tokens[2]))
    fileAMu.write("{} 0  0  0  0  {}  0\n".format(tokens[0],tokens[5]))
