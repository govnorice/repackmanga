#include <iostream>
#include <string>
#include "modules/repack.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc >= 3) {
        string option1 = argv[1];
        string option2 = argv[2];

        if (option1 == "--path") {
            Repack::start(option2);
        }
    } else if (argc == 1) {
        cout << "GUI" << endl;
    } else {
        cerr << "Error: Insufficient arguments" << endl;
        return 1;
    }
    return 0;
}