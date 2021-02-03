#ifndef H_REGISTRY
#define H_REGISTRY

#include <string>
#include <vector>

struct CsvLine{
    int width;
    char ** items;
};

//Describes purpose of item
enum ItemClass{
    None=0,
    Part=1,
    Transport=2,
    Tool=4,
    Fuel=8,
    Weapon=16,
    Ammo=32,
    Aid=64
};

//Additional restrictions or specifics of item
enum ItemSpecifier{
    None=0,
    Land=1,
    Water=2,
    Snow=4,
    Desert=8
};

struct ItemRecipe{
    std::string itemName [];
    int itemQuantity [];    
};

class Item{

    std::string name;
    std::vector<ItemRecipe> recipes;

    int baseValue;

    uint64_t itemClass;
    uint64_t ItemSpecifier;

    //Special values
    int arg1;
    int arg2;
    int arg3;
};

class ItemRegistry{
    
    public:

    std::vector<Item> items;
};

struct NpcNameList{
    std::string locality;

    std::vector<std::string> firstNames;
    std::vector<std::string> lastNames;
};

class NameRegistry{
    std::vector<NpcNameList> list;

    void AddFirstName(std::string locality, std::string name);
    void AddLastName(std::string locality, std::string name);
};

struct TownNames{
    std::string locality;

    std::vector<std::string> names;
};

class TownRegistry{
    std::vector<TownNames> towns;

    void AddTown(std::string locality, std::string name);
};

#endif