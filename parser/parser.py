# -*- coding=utf-8 -*-

import sys
import re
import collections

def timeElapsedCSV(tests):
    outMap = {}

    for test in tests:
        m = re.search("#AIRPLANES:\s+(\d+)", test)
        m2 = re.search("#THREADS:\s+(\w+)", test)
        if m and m2:
            airplaneCount = float(m.group(1))
            threads = m2.group(1)

            subTests = test.split("TEST ")
            avg = 0.0
            subTestsLength = float(len(subTests))
            for subTest in subTests:
                m = re.search("ELAPSED\s+TIME:\s+([\d\.]+)\s+seconds", subTest)
                if m:
                    avg += float(m.group(1))
            avg /= subTestsLength

            if not airplaneCount in outMap:
                outMap[airplaneCount] = {"TRUE": 0.0, "FALSE": 0.0}

            outMap[airplaneCount][threads] = avg

    out = "Número de Aeronves"
    od = collections.OrderedDict(sorted(outMap.items()))

    for k, v in od.iteritems():
        out += ",\"" + str(k) + "\""

    out += "\nCom Threads"
    for k, v in od.iteritems():
        out += ",\"" + str(v["TRUE"]) + "\""

    out += "\nSem Threads"
    for k, v in od.iteritems():
        out += ",\"" + str(v["FALSE"]) + "\""

    return re.sub("\.", ",", out)

testsFile = open(sys.argv[1])
tests = testsFile.read().split("#TEST")

print timeElapsedCSV(tests)