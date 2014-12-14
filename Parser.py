import re
import sys
import random

def parsing(filename):
    data = open(filename, 'r')

    parsingData = []
    seqnum = 0
    find_weight = 0
    weight = {}
    seqItem = []
    distinctSeqItem = []

    for line in data:

            item = []
            dicItem = {}

            dataIndex = 0
            seqnum += 1
            if line[-1] == '\n':
                    line = line[:-1]
            line = line.split(' ')

            if line[0].find('weight')!= (-1):
                    find_weight = 1
                    raw_weight = data.next()
                    seqnum -= 1
                    break
            parsingData.append([])
            while (dataIndex + 1 < len(line)) & (find_weight ==  0):
                    itemSize = int(line[dataIndex]) * 2
                    eachseq = []
                    for i in range(1,itemSize+1,2):
                            #if weight.has_key(line[dataIndex+i]) == False:
                            #	weight[line[dataIndex+i]] = random.randint(1,10) 
                            if dicItem.has_key(line[dataIndex+i]) == False:
                                    item.append(line[dataIndex+i])
                            line[dataIndex+i] += ',' + line[dataIndex+i+1]
                            eachseq.append(line[dataIndex+i])
                    parsingData[len(parsingData) - 1].append(eachseq)
                    dataIndex += (itemSize+1)

            seqItem.append(item)
            seq = []
            for da in set(item):
                    seq.append(da)
            distinctSeqItem.append(seq)
    if find_weight == 1:
            line = raw_weight.split(' ')
            for i in range(0,len(line)-1,2):
                    weight[line[i]] = int(line[i+1])



    [distinctSeqItem[i].sort() for i in range(len(distinctSeqItem))]
    utilityMatrix(distinctSeqItem ,seqnum, parsingData, seqItem, weight)


def utilityMatrix(distinctSeqItem ,seqnum, parsingData, seqItem, weight):
    matrix = []

    out = open('./data/UtilityMatrix.txt','w')
    for i in range(len(parsingData)):
            totalUtility = 0
            seqMatrix = []
            for j in range(len(parsingData[i])):
                    #print "test",[weight[parsingData[i][j][k].split(',')[0]] * int(parsingData[i][j][k].split(',')[1]) for k in range(len(parsingData[i][j]))]
                    totalUtility = totalUtility + sum([weight[parsingData[i][j][k].split(',')[0]] * int(parsingData[i][j][k].split(',')[1]) for k in range(len(parsingData[i][j]))])

            print totalUtility
            for j in range(len(parsingData[i])):
                    nowMatrix = []
                    splitData = [parsingData[i][j][k].split(',') for k in range(len(parsingData[i][j]))]
                    itemIndex = 0
                    end = 0

                    for k in range(len(distinctSeqItem[i])):

                            #print j,distinctSeqItem[i][k],splitData[itemIndex][0], itemIndex, len(parsingData[i][j])

                            if (distinctSeqItem[i][k] == splitData[itemIndex][0]) & (end == 0):
                                    #print splitData[itemIndex][0]
                                    #print j,distinctSeqItem[i][k],splitData[itemIndex][0]
                                    totalUtility = totalUtility - weight[splitData[itemIndex][0]] * int(splitData[itemIndex][-1])
                                    nowMatrix.append([weight[splitData[itemIndex][0]] * int(splitData[itemIndex][-1]),totalUtility])
                                    if (itemIndex + 1) == len(parsingData[i][j]):
                                            end = 1
                                    else:
                                            itemIndex += 1
                            else:
                                    nowMatrix.append([0,totalUtility])
                    seqMatrix.append(nowMatrix)
            matrix.append(seqMatrix)
    for i in range(seqnum):

            out.write(str(len(parsingData[i])) + ',' + str(len(distinctSeqItem[i])) + ';')
            for data in distinctSeqItem[i]:
                    if distinctSeqItem[i].index(data) != (len(distinctSeqItem[i])-1):
                            out.write(str(data) + ',')
                    else:
                            out.write(str(data) + ';')

            for data in matrix[i]:
                    for item in data:
                            out.write(str(item[0]) + ',' + str(item[1]) + ';')
            out.write('\n')

if __name__ == "__main__":
    if len(sys.argv) != 2:
            print "Usage: <data>"
            exit(-1)

    filename = str(sys.argv[1])
    parsing(filename)
