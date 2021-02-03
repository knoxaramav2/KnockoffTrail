#ifndef H_ENGINE
#define H_ENGINE

#include <string>
#include <vector>
#include <map>

enum RegisterIdx{

    //Specialty Registers
    SI,         //Switch Integer
    TRI,        //Typical Return Integer
    TRF,        //Typical Return Float
    OPTI,       //Option Integer
    
    //Common Float Registers
    CRF0,
    CRF1,
    CRF2,
    CRF3,

    //Common Integer Registers
    CRI0,
    CRI1,
    CRI2,
    CRI3,

    //Text Registers
    //Character Registers
    BRDR,       //Border Character
    
    //Common Character Registers
    CCR0,
    CCR1,
    CCR2,
    CCR3,

    //Common String Registers
    CSR0,
    CSR1,
    CSR2,
    CSR3,

    //Always last, not a register
    RegIdxLen
};

enum TokenTypes{
    //Primitives
    R_INT,
    R_FLOAT,
    R_BYTE,
    R_STRING,

    //Specifiers
    R_CONST_ID,
    R_ID,
    R_OP_CODE,
    R_LABEL
};

struct MemCell{

    protected:

    public:

    TokenTypes type;
    std::string name;
    void * data;

    std::string DiagVal();

};

class Register : public MemCell{

    public:
    Register(std::string name, TokenTypes type);
    ~Register();

    TokenTypes GetDataType();
    std::string GetName();
    void * GetData();
    void SetData(void * data);
    void FreeRegister();
};

class RegisterArray{
    Register* _registers [RegisterIdx::RegIdxLen];

    public:

    RegisterArray();

    void DumpToFile(std::string output);
    Register GetRegister(RegisterIdx idx);
    void SetRegister(RegisterIdx idx, void * data);
};

struct DataConst : MemCell{
    //PrimitiveDataType type;
    //void * data;
    //std::string name;
};

class ConstFrame{
    std::map<std::string, DataConst> _int;
    std::map<std::string, DataConst> _float;
    std::map<std::string, DataConst> _byte;
    std::map<std::string, DataConst> _string;

    public:

    int GetInt(std::string id);
    float GetFloat(std::string id);
    char GetByte(std::string id);
    std::string GetString(std::string id);

    bool Add(std::string id, int data);
    bool Add(std::string id, float data);
    bool Add(std::string id, char data);
    bool Add(std::string id, std::string data);

    std::ostringstream FormattedFrameString();
};

class Engine{

    std::string _gamedataDir;
    std::string _itemListDir;
    std::string _nameListDir;
    std::string _townListDir;
    std::string _scriptDir;
    std::string _saveDir;

    RegisterArray _registers;

    class ScriptFrame * _frame;

    Engine();

    static Engine * _engine;

    public:

    static Engine * Instance();

    int RunScript(std::string file);

    void DumpRegisters();
    void DumpScriptFrames();

    void AddTokenToFrame(struct Token*t);
    void AddConstToFrame(std::string id, void*val, enum TokenTypes type);

};


enum OpCodes{
    //General
    OP_NO,
    OP_LABEL,

    //File ops
    OP_RUN, //execute script

    //Logic
    OP_CMP,     //compare
    OP_JMP,     //jump
    OP_JEQ,     //jump equal_registers.DumpToFile(outPath);
    OP_JLS,     //jump less
    OP_JGT,     //jump greater
    OP_SJMP,    //switch jump
    OP_SDEF,    //switch default

    //UI
    OP_PUSH_DISPLAY,
    OP_POP_DISPLAY,
    OP_PUSH_TITLE,
    OP_PUSH_OPTION,

    //Data
    OP_MOV_INT,
    OP_MOV_FLOAT,
    OP_MOV_STRING,
    OP_MOV_BYTE,

    //Stack
    OP_PUSH_INT,
    OP_PUSH_FLOAT,
    OP_PUSH_STRING,
    OP_PUSH_BYTE,

    OP_POP_INT,
    OP_POP_FLOAT,
    OP_POP_STRING,
    OP_POP_BYTE,


};

struct Token{

    OpCodes op;
    
    struct Token * next;
    struct Token * target;

    MemCell * r1;
    MemCell * r2;
    MemCell * r3;
};

class TokenFrame{
    std::vector<Token*> _tokens;

    public:

    TokenFrame();
    ~TokenFrame();

    void SetScript(std::vector<Token*>scriptTokens);
    std::ostringstream FormattedTokenString();
};

class ScriptFrame{

    class ScriptFrame * _next;
    class ScriptFrame * _previous;

    public:

    ScriptFrame();
    ScriptFrame(class ScriptFrame * prev);
    ~ScriptFrame();

    TokenFrame tokenFrame;
    ConstFrame constFrame;

    std::ostringstream GetFormattedFrame();

};

#endif