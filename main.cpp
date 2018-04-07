
#include"csvUtils.h"
using namespace std;
int main(int argc, char *argv[]){
        Map d;
        int chunk = stoi(argv[2]);
        d = read_csv(argv[1], chunk, true);
        write_csv(d, "out.csv", 0, ',');
        d = read_csv(argv[1], chunk, true);
        write_csv(d, "out.csv", 1, ',');
        d = read_csv(argv[1], chunk, true);
        write_csv(d, "out.csv", 1, ',');
        return 0;
}
