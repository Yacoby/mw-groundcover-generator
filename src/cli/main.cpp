#include "CliMain.h"

#include <iostream>
#include <exception>

using namespace std;

int main(int argc, char* argv[]) {
    try {
        return cliMain(argc, argv);
    } catch (exception& e) {
        cout << "Something went wrong: " << e.what() << endl;
    }
}