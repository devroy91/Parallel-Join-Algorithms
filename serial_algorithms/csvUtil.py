def readRows(fileName, delim = ','):
    with open(fileName) as inp:
        header = inp.readline().strip().split(delim)
        rows = []
        for lines in inp:
            rows.append(lines.strip().split(delim))
            print(rows)
    return [header, rows]

def writeRows(fileName, header = None, rows = None, delim = ','):
    with open(fileName, 'w') as outp:
        if header:
            print(delim.join(header), file = outp)
        for lines in rows:
            print(delim.join(lines), file = outp)
        