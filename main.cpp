
#include"csvUtils.h"
using namespace std;
int main(){
        Map d;
        d = read_csv("ManagerS.csv");
        write_csv(d, "out.csv");
        return 0;
}
