#coding:utf-8
import math

MAX_TIME=100 #最大的
STEP=0.001

COUNT=int(MAX_TIME/STEP)

with open("./data.txt",'wt') as f:
    for x in range(COUNT):
        f.write("%f\n"%(math.sin(x)+1.1)) #轴1
        f.write("%f\n"%(math.sin(x)+1.2)) #轴2
        f.write("%f\n"%(math.sin(x)+1.3)) #轴3
        f.write("%f\n"%(math.sin(x)+1.4)) #轴4
        f.write("%f\n"%(math.sin(x)+1.5)) #轴5
        f.write("%f\n"%(math.sin(x)+1.6)) #轴6
        f.write("%f\n"%(math.sin(x)+1.7)) #轴7  