#ifndef H_ACTOR
#define H_ACTOR

#include <string>

enum Profession{
    None,
    Doctor,
    Farmer,
    Hunter,
    Smith,
    Craftsmen,
    Tradesmen
};

enum Disease{
    Healthy=0,
    BrokenLeg=1,
    BrokenArm=2,
    Bleeding=4,
    Concussed=8,
    Poisoned=16,
    Parasite=32,
    Gangrene=64,
};

class Actor{

    public:

    Actor();

    std::string nationality;
    std::string firstName;
    std::string lastName;

    uint64_t disease;
    bool male;
    int health;
    int age;
};

class NPC : Actor{
    public :
    NPC();
};

class Player : Actor{

    public:
    Player();
};



#endif