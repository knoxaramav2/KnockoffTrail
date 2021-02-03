#include <string>
#include <iostream>

#include "gameEngine.hpp"

using namespace std;

int main(int argc, char ** argv){

    cout << "START" << endl;

    Engine * engine = Engine::Instance();
    engine->RunScript("main.kts"); 

    
    engine->DumpRegisters();
    engine->DumpScriptFrames();

    cout << "END PROGRAM" <<endl;

    return 0;
}

