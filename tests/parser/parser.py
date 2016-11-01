# -*- coding=utf-8 -*-

import sys
import re
import collections

def timeElapsedCSV(tests):
    outMap = {}

    for test in tests:
        m = re.search("#AIRPLANES:\s+(\d+)", test)
        m2 = re.search("#THREADS:\s+(\w+)", test)
        m3 = re.search("#USE AREA DIVISION:\s+(\w+)", test)
        if m and m2 and m3:
            airplaneCount = float(m.group(1))
            threads = m2.group(1)
            areaDivision = m3.group(1)

            subTests = test.split("TEST ")
            avg = 0.0
            subTestsLength = float(len(subTests))
            for subTest in subTests:
                m = re.search("ELAPSED\s+TIME:\s+([\d\.]+)\s+seconds", subTest)
                if m:
                    avg += float(m.group(1))
            avg /= subTestsLength

            if not airplaneCount in outMap:
                outMap[airplaneCount] = {
                    "FALSE_FALSE": 0.0,
                    "FALSE_TRUE": 0.0,
                    "TRUE_FALSE": 0.0,
                    "TRUE_TRUE": 0.0
                }

            outMap[airplaneCount][threads + "_"+ areaDivision] = avg

    out = "Número de Aeronves"
    od = collections.OrderedDict(sorted(outMap.items()))

    for k, v in od.iteritems():
        out += ",\"" + str(k) + "\""

    out += "\nSem otimização"
    for k, v in od.iteritems():
        out += ",\"" + str(v["FALSE_FALSE"]) + "\""

    out += "\nCom threads"
    for k, v in od.iteritems():
        out += ",\"" + str(v["TRUE_FALSE"]) + "\""

    out += "\nCom divisão em áreas"
    for k, v in od.iteritems():
        out += ",\"" + str(v["FALSE_TRUE"]) + "\""

    out += "\nCom threads e divisão em área"
    for k, v in od.iteritems():
        out += ",\"" + str(v["TRUE_TRUE"]) + "\""

    return re.sub("\.", ",", out)

testsFile = open(sys.argv[1])
tests = testsFile.read().split("#TEST")

print timeElapsedCSV(tests)