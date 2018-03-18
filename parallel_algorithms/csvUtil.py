def readRows(fileName, delim = ',', chunkSize = 1024):
    with open(fileName) as inp:
        header = inp.readline().strip().split(delim)
        yield header
        while True:
            rows = []
            block = inp.readlines(chunkSize)
            if not block:
                break
            for lines in block:
                rows.append(lines.strip().split(delim))
            yield rows
    

def writeRows(fileName, header = None, rows = None, delim = ','):
    with open(fileName, 'w') as outp:
        if header:
            print(delim.join(header), file = outp)
        for lines in rows:
            print(delim.join(lines), file = outp)

def findIdx(row, idx):
    return row.index(idx)