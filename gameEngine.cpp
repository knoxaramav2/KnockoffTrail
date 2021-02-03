#include "gameEngine.hpp"
#include "crossplatform.hpp"
#include "util.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>

using namespace std;

Engine * Engine::_engine = nullptr;

static map<string, OpCodes> _opMap;

void InitOpCodeMap(){
    _opMap["noop"] = OP_NO;

    _opMap["run"] = OP_RUN;

    _opMap["cmp"] = OP_CMP;
    _opMap["jmp"] = OP_JMP;
    _opMap["jeq"] = OP_JEQ;
    _opMap["jls"] = OP_JLS;
    _opMap["jgt"] = OP_JGT;
    _opMap["sjmp"] = OP_SJMP;
    _opMap["sdef"] = OP_SDEF;

    _opMap["pudsp"] = OP_PUSH_DISPLAY;
    _opMap["podsp"] = OP_POP_DISPLAY;
    _opMap["pttl"] = OP_PUSH_TITLE;
    _opMap["popt"] = OP_PUSH_OPTION;

    _opMap["movi"] = OP_MOV_INT;
    _opMap["movf"] = OP_MOV_FLOAT;
    _opMap["movs"] = OP_MOV_STRING;
    _opMap["movb"] = OP_MOV_BYTE;

    _opMap["pui"] = OP_PUSH_INT;
    _opMap["puf"] = OP_PUSH_FLOAT;
    _opMap["pus"] = OP_PUSH_STRING;
    _opMap["pub"] = OP_PUSH_BYTE;
    _opMap["poi"] = OP_POP_INT;
    _opMap["pof"] = OP_POP_FLOAT;
    _opMap["pos"] = OP_POP_STRING;
    _opMap["pob"] = OP_POP_BYTE;
}

inline string PrimitiveDataTypeToString(TokenTypes type){

    string str;

    switch(type){
        case R_INT: str = "STRING"; break;
        case R_FLOAT: str = "FLOAT"; break;
        case R_BYTE: str = "BYTE"; break;
        case R_STRING: str = "STRING"; break;
        default: str = "NA"; break;
    }

    return str;
}

string MemCell::DiagVal(){

    ostringstream ret;
    ret << setfill(' ');
    ret << setw(15) << name << " : "
        << setw(07) << PrimitiveDataTypeToString(type) << " : "
        << setw (5) <<data;

    return ret.str();
}


void InitEngineStatics(){
    InitOpCodeMap();
}

Register::Register(string name, TokenTypes type){

    type = type;
    name = name;

    switch(type){
        case TokenTypes::R_BYTE:
        data = new char(0);
        break;
        case TokenTypes::R_FLOAT:
        data = new float(0);
        break;
        case TokenTypes::R_INT:
        data = new int(0);
        break;
        case TokenTypes::R_STRING:
        data = new string("");
        break;
        default:break;
    }
}

Register::~Register(){
    switch(type){
        case TokenTypes::R_BYTE:
        delete (char*) data;
        break;
        case TokenTypes::R_FLOAT:
        delete (float*) data;
        break;
        case TokenTypes::R_INT:
        delete (int*) data;
        break;
        case TokenTypes::R_STRING:
        delete (string*) data;
        break;
        default: break;
    }
}

TokenTypes Register::GetDataType(){
    return this->type;
}

string Register::GetName(){
    return name;
}

void * Register::GetData(){
    return data;
}

void Register::SetData(void * data){
    data = data;
}

void Register::FreeRegister(){
    try{
        switch (type)
        {
        case TokenTypes::R_BYTE:
            delete (char *) data;
        break;
        case TokenTypes::R_FLOAT:
            delete (float *) data;
        break;
        case TokenTypes::R_INT:
            delete (int *) data;
        break;
        case TokenTypes::R_STRING:
            delete (string *) data;
        break;
        
        default:
            break;
        }
    } catch(exception e){
        WriteToLog(e.what());
    }
}



RegisterArray::RegisterArray(){
    
    _registers[SI]   = new Register("SI", TokenTypes::R_INT);
    _registers[TRI]  = new Register("TRI", TokenTypes::R_INT);
    _registers[TRF]  = new Register("TRF", TokenTypes::R_FLOAT);
    _registers[OPTI] = new Register("OPTI", TokenTypes::R_INT);

    _registers[CRF0] = new Register("CRF0", TokenTypes::R_FLOAT);
    _registers[CRF1] = new Register("CRF1", TokenTypes::R_FLOAT);
    _registers[CRF2] = new Register("CRF2", TokenTypes::R_FLOAT);
    _registers[CRF3] = new Register("CRF2", TokenTypes::R_FLOAT);

    _registers[CRI0] = new Register("CRI0", TokenTypes::R_INT);
    _registers[CRI1] = new Register("CRI1", TokenTypes::R_INT);
    _registers[CRI2] = new Register("CRI2", TokenTypes::R_INT);
    _registers[CRI3] = new Register("CRI3", TokenTypes::R_INT);

    _registers[BRDR] = new Register("BRDR", TokenTypes::R_BYTE);

    _registers[CCR0] = new Register("CCR0", TokenTypes::R_BYTE);
    _registers[CCR1] = new Register("CCR1", TokenTypes::R_BYTE);
    _registers[CCR2] = new Register("CCR2", TokenTypes::R_BYTE);
    _registers[CCR3] = new Register("CCR3", TokenTypes::R_BYTE);

    _registers[CSR0] = new Register("CSR0", TokenTypes::R_STRING);
    _registers[CSR1] = new Register("CSR1", TokenTypes::R_STRING);
    _registers[CSR2] = new Register("CSR2", TokenTypes::R_STRING);
    _registers[CSR3] = new Register("CSR3", TokenTypes::R_STRING);
}

//TODO less stupid way
void RegisterArray::DumpToFile(string output){
    
    //cout << output << endl;
    ofstream out(output, ios::out);

    auto regLen = RegisterIdx::RegIdxLen;

    for(auto i=0; i <regLen; ++i){
        
        Register * reg = _registers[i];

        string name = reg->GetName();
        TokenTypes type = reg->GetDataType();

        string data;
        string types = PrimitiveDataTypeToString(type);

        switch(type){
            case TokenTypes::R_BYTE:
                data = to_string(*(char *) reg->GetData());
            break;
            case TokenTypes::R_FLOAT:
                data = to_string(*(float *) reg->GetData());
            break;
            case TokenTypes::R_INT:
                data = to_string(*(int *) reg->GetData());
            break;
            case TokenTypes::R_STRING:
                data = *(string *) reg->GetData();
            break;
            case TokenTypes::R_ID:
            case TokenTypes::R_CONST_ID:
            case TokenTypes::R_OP_CODE:
                data = *(string *) reg->GetData();
            break;
            case TokenTypes::R_LABEL:
                data = *(string *) reg->GetData();
            break;
        }
    
        ostringstream sstream;
        sstream 
            << setfill(' ') << setw(6) << name << " | "
            << setfill(' ') << setw(6) << types << " | "
            << setfill(' ') << setw(30) << data 
            << endl
            ;

        out << reg->DiagVal() << endl;
    }

    out.close();
}

Register RegisterArray::GetRegister(RegisterIdx idx){
    return *(_registers[idx]);
}

void RegisterArray::SetRegister(RegisterIdx idx, void * data){
    _registers[idx]->SetData(data);
}


int ConstFrame::GetInt(string id){

    return 0;
}

float ConstFrame::GetFloat(string id){
    
    return 0;
}

char ConstFrame::GetByte(string id){

    return ' ';
}

string ConstFrame::GetString(string id){

    return "";
}

bool ConstFrame::Add(string id, int data){

    DataConst dc = _int[id];



    return true;
}

bool ConstFrame::Add(string id, float data){

    return true;
}

bool ConstFrame::Add(string id, char data){

    return true;
}

bool ConstFrame::Add(string id, string data){

    return true;
}

ostringstream ConstFrame::FormattedFrameString(){

    ostringstream ret;


    return ret;
}


Engine::Engine(){
    string exec = getExecDir();

    _gamedataDir    = exec+"/gamedata/";
    _itemListDir    = _gamedataDir+"itemLists";
    _nameListDir    = _gamedataDir+"nameLists";
    _townListDir    = _gamedataDir+"townLists";
    _scriptDir      = _gamedataDir+"scripts";

    _saveDir        = exec+"/save/";

    _frame = new ScriptFrame();

    InitEngineStatics();//Might want to move this
}

Engine * Engine::Instance(){
    if (_engine == nullptr){
        _engine = new Engine();
    }

    return _engine;
}

void Engine::AddConstToFrame(string id, void * val, TokenTypes type){

    switch(type){
        case R_INT:
            _frame->constFrame.Add(id, *static_cast<int*>(val));
        break;
        case R_FLOAT:
            _frame->constFrame.Add(id, *static_cast<float*>(val));
        break;
        case R_STRING:
            _frame->constFrame.Add(id, *static_cast<string*>(val));
        break;
        case R_BYTE:
            _frame->constFrame.Add(id, *static_cast<char*>(val));
        break;
        default:
            ostringstream os;
            os << "Unexpected const: "<<id<<", type="<<type;
            throw runtime_error(os.str());
        break;
    }
}

void Engine::AddTokenToFrame(Token*t){

}


Token * Tokenize(vector<string>& raw){
    Token * ret = new Token();
    OpCodes code = OpCodes::OP_NO;

    size_t len = raw.size();

    cout << ":: ";

    for (size_t i = 0; i < len; ++i){
        cout << raw[i] << " | ";
    }
    
    cout << endl;

    ret->op = code;

    return ret;
}

struct PreToken{
    string raw;
    TokenTypes type;
};

OpCodes StrToOp(string& raw){

    OpCodes ret = OP_NO;



    return ret;
}

Token * FinalizeToken(vector<PreToken> preTokens){

    Token * ret = nullptr;
    //Token * tkn = nullptr;

    for(size_t i = 0; i <preTokens.size(); ++i){
        PreToken * pt = &preTokens[i];
        Token * t = new Token();
        OpCodes ops = _opMap[pt->raw];

        switch (pt->type)
        {
            //Op codes: must be first if present
        case TokenTypes::R_OP_CODE:
            if (i > 0){
                ostringstream os;
                os << "Unexpected op code: " + pt->raw;
                throw runtime_error(os.str());
                }
            t->op = ops;
            break;
            //R vals
        case TokenTypes::R_INT:
        case TokenTypes::R_FLOAT:
        case TokenTypes::R_STRING:
        case TokenTypes::R_BYTE:
            if (t->r1 == nullptr){
                
                
            } else if (t->r2 == nullptr){

            } else if (t->r3 == nullptr){

            } else {
                ostringstream os;
                os << "Unexpected rval: " << pt->raw;
                throw runtime_error(os.str());
            }
        break;
            //Id references
        case TokenTypes::R_ID:

        break;
            //Const id replace (if not first)
        case TokenTypes::R_CONST_ID:

        break;
        
        default:
            break;
        }
    }


    return ret;
}

PreToken NextString(string& raw, size_t& idx){
    PreToken ret;
    ret.type=R_STRING;
    size_t start = ++idx;
    size_t len = raw.length();

    for(; idx < len; ++idx){
        if (raw[idx]=='\\') { 
            raw.erase(raw.begin()+idx);
            --len;
            continue;
        } else if (raw[idx] == '\''){
            ret.raw = raw.substr(start, idx-start);
            break;
        }
    }

    return ret;
}

PreToken NextNumber(string& raw, size_t& idx){
    PreToken ret;
    bool dec = false;
    size_t start = idx;
    size_t len = raw.length();
    
    for(; idx<len; ++idx){
        if (raw[idx] == '.'){
            if (dec) {
                dec = true;
            } else {
                throw runtime_error("Unexpected '.' during numeric parse.");
            }
        } else if (raw[idx] == ' ' || raw[idx] == '\t') {
            break;
        } else if (raw[idx] < '0' || raw[idx] > '9'){
            throw runtime_error("Unexpected character in numeric parse.");
        }
    }

    ret.raw = raw.substr(start, idx-start);
    ret.type = dec ? R_FLOAT : R_INT;

    return ret;
}

PreToken NextIdentifier(string& raw, size_t& idx, bool constDec){
    PreToken ret;
    size_t start = idx;
    size_t len = raw.length();

    if (constDec) {++idx;}

    for(; idx < len; ++idx){
        char c = raw[idx];

        if (
            (c < 'a' || c > 'z') &&
            (c < 'A' || c > 'Z') &&
            (c < '0' || c > '9') &&
            c != '_'
        ){
            break;
        }
    }

    ret.raw = raw.substr(start, idx-start);
    ret.type = constDec ? R_CONST_ID : R_ID;

    return ret;
}

void PushConstToFrame(vector<PreToken>& tkns, Engine& eng){

}

Token * SetLabel(PreToken& tkn, Engine& eng){
    Token * ret = new Token();
    ret->op = OP_LABEL;
    ret->r1 = new MemCell();
    ret->r1->name = tkn.raw;

    return ret;
}

Token * ParseToken(string raw, Engine&eng){

    Token * base = nullptr;
    vector<PreToken> terms;
    size_t len = raw.length();
    size_t i = 0;

    for(; i < len; ++i){
        char c = raw[i];

        if (c ==' ' || c == '\t' || c == '\r' || c == '\n') {continue;}//WS
        else if (c == '#'){//Comment, end
            break;
        } else if (c == '\''){//string
            PreToken s = NextString(raw, i);
            terms.push_back(s);
        } else if (((c>='a' && c<='z') || (c>= 'A' && c<= 'Z')) || c=='.') {//identifier
            PreToken s = NextIdentifier(raw, i, c=='.');
            terms.push_back(s);
        } else if ( c >= '0' && c <= '9'){//numeric
            PreToken s = NextNumber(raw, i);
            terms.push_back(s);
        } else if (c == ':'){//jump label
            ++i;
            PreToken s = NextIdentifier(raw, i, false);
            s.type = TokenTypes::R_LABEL;
            terms.push_back(s);
        } else {
            throw runtime_error("Unexpected character '"+string(1,c)+"'.");
        }
    }

    if (terms.size() == 0){
        return nullptr;
    } else if (terms[0].type == TokenTypes::R_OP_CODE){
        base = FinalizeToken(terms);
    } else if (terms[0].type == TokenTypes::R_CONST_ID) {
        PushConstToFrame(terms, eng);
    } else if (terms[0].type == TokenTypes::R_LABEL) {
        base = SetLabel(terms[0], eng);
    } else {
        ostringstream os;
        os << "Unexpected token type " << terms[0].type;
        throw runtime_error(os.str());
    }

    return base;
}

vector<Token*> LoadScript(string filePath, Engine&eng){
    ifstream ifstr(filePath, ios::in);
    WriteToLog("Loading " + filePath);

    string buffer, continued;
    vector<Token*> ret;

    while(getline(ifstr, buffer)){
        Token * t = ParseToken(buffer, eng);
        if (t == nullptr) continue;
        ret.push_back(t);
    }

    for(size_t i = 0; i < ret.size(); ++i){
        auto v = ret[i];
        printf("%d", v->op);
    }

    ifstr.close();

    WriteToLog("::End load::");

    return ret;
}

int Engine::RunScript(string file){

    file = getExecDir() + "gamedata/scripts/" + file;
    vector<Token*> tokens = LoadScript(file, *this->_engine);
    _frame->tokenFrame.SetScript(tokens);

    DumpScriptFrames();
    return 0;
}

void Engine::DumpRegisters(){
    string execPath = getExecDir();
    string outPath = execPath + "registers.log";
    _registers.DumpToFile(outPath);
}

void Engine::DumpScriptFrames(){
    string execPath = getExecDir();
    string outPath = execPath + "frames.log";
    ostringstream output;

    auto frame = _frame;
    if (frame == nullptr) { return; }

    output << ":: Tokens ::  " << endl;
    output << _frame->tokenFrame.FormattedTokenString().str() << endl;
    output << ":: Constants ::" << endl;
    output << _frame->constFrame.FormattedFrameString().str() << endl;

    ofstream out(outPath, fstream::out);
    out << output.str() << endl;
    out.close();
}

TokenFrame::TokenFrame(){
    
}

TokenFrame::~TokenFrame(){
    for (size_t i = 0; i < _tokens.size(); ++i){
        Token * t = _tokens[i];

        for (Token * n = t; n != nullptr; n=n->next){
            delete n;
        }
    }
}

ostringstream TokenFrame::FormattedTokenString(){
    //const int offset = 4;
    ostringstream ret;
    if (_tokens.size() == 0) { return ret; }
    ret << setfill('\t');
    int idx = 1;

    for (size_t i = 0; i < _tokens.size(); ++i){
        Token * tkn = _tokens[i];

        for(Token * n = tkn; n->next!=nullptr; n=n->next){
            ostringstream sst;
            sst << setfill('\t') << setw(idx*i)
                << "NE Line (" << i << ") : "
                << "OP : " << n->op 
                << " | R1 : " << n->r1->DiagVal()
                << " | R2 : " << n->r2->DiagVal()
                << " | R3 : " << n->r3->DiagVal()
                << EL;
            WriteToLog(sst.str());
        }
    }

    return ret;
}

void TokenFrame::SetScript(vector<Token*> base){
    _tokens = base;
}


ScriptFrame::ScriptFrame(){
    _next = _previous = nullptr;
}

ScriptFrame::ScriptFrame(ScriptFrame*prev){
    _next = nullptr;
    _previous = prev;
    _previous->_next = this;
}

ScriptFrame::~ScriptFrame(){
    
}

ostringstream ScriptFrame::GetFormattedFrame(){

    ostringstream ret;
    ret << setfill('\t');

    ScriptFrame * curr = this;


    for (int lvl = 0; curr->_next != nullptr; ++lvl){
        ret << setw(lvl);
        ret << (curr->constFrame.FormattedFrameString()).str();
        ret << (curr->tokenFrame.FormattedTokenString()).str();
    }

    ret << endl;


    return ret;
}



