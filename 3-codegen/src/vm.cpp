#include <vector>
#include <cstdio>
#include <string>
#include <map>
#include <memory>
#include <fstream>
#include <iostream>
#include <cassert>

class Context;

class Instruction
{
private:
    unsigned index;
    std::string line;   
protected:    
    Instruction(unsigned _index, std::string _line)
        : index(_index)
        , line(_line)
    {}
public:
    virtual ~Instruction()
    {}
    
    std::string getLine() const
    { return line; }
    
    unsigned getIndex() const
    { return index; }
    
    virtual std::string getAssembly() const
    { return ""; }
    
    virtual void execute(Context &ctxt) const =0;
};

class Context
{
private:
    unsigned pc;
    std::map<std::string,int32_t> registers;
    std::vector<std::shared_ptr<Instruction> > instructions;
    std::map<std::string,unsigned> labels;
public:
    Context()
        : pc(0)
    {}

    int32_t getReg(std::string name)
    {
        std::cerr<<"getReg("<<name<<")\n";
        auto it=registers.find(name);
        if(it==registers.end())
            throw std::runtime_error("getReg("+name+") : No such register.");
        return it->second;
    }

    void setReg(std::string name, int32_t value)
    {
        registers[name]=value;
    }

    void advancePC()
    {
        pc++;
    }

    void jump(std::string label)
    {
        auto it=labels.find(label);
        if(it==labels.end())
            throw std::runtime_error("jump("+label+") : No such label.");
        pc=it->second;
    }

    void step()
    {
        if(pc >= instructions.size() ){
            throw std::runtime_error("step : pc has exceeded number of instructions.");
        }
        assert(instructions.at(pc));

        std::cerr<<"Exec "<<instructions[pc]->getIndex()<<":"<<instructions[pc]->getAssembly()<<", {";
        for(auto b : registers){
            std::cerr<<" "<<b.first<<":"<<b.second;
        }
        std::cerr<<"}\n";
      
        instructions.at(pc)->execute(*this);
    }

    void add_instruction(const std::string &line);
};



class ConstInstruction
    : public Instruction
{
private:
    std::string rDst;
    int32_t value;
public:
    ConstInstruction(unsigned _index, std::string _line, std::string _rDst, int32_t _value)
        : Instruction(_index, _line)
        , rDst(_rDst)
        , value(_value)
    {}
    
    virtual std::string getAssembly() const override
    { return "const "+rDst+" "+std::to_string(value); }


    virtual void execute(Context &ctxt) const override
    {
        ctxt.setReg(rDst, value);
        ctxt.advancePC();
    }
};

class InputInstruction
    : public Instruction
{
private:
    std::string rDst;
public:
    InputInstruction(unsigned _index, std::string _line, std::string _rDst)
        : Instruction(_index, _line)
        , rDst(_rDst)
    {}
    
    virtual std::string getAssembly() const override
    { return "input "+rDst; }


    virtual void execute(Context &ctxt) const override
    {
        int32_t value;
        std::cin >> value;
        ctxt.setReg(rDst, value);
        ctxt.advancePC();
    }
};

class ParamInstruction
    : public Instruction
{
private:
    std::string rDst;
    unsigned slot;
public:
    static std::vector<int32_t> args; 

    ParamInstruction(unsigned _index, std::string _line, std::string _rDst, unsigned _slot)
        : Instruction(_index, _line)
        , rDst(_rDst)
        , slot(_slot)
    {}
    
    virtual std::string getAssembly() const override
    { return "param "+rDst+" "+std::to_string(slot); }


    virtual void execute(Context &ctxt) const override
    {
        int32_t value=args.at(slot);
        ctxt.setReg(rDst, value);
        ctxt.advancePC();
    }
};

class OutputInstruction
    : public Instruction
{
private:
    std::string rSrc;
public:
    OutputInstruction(unsigned _index, std::string _line, std::string _rSrc)
        : Instruction(_index, _line)
        , rSrc(_rSrc)
    {}

    virtual void execute(Context &ctxt) const override
    {
        std::cout << ctxt.getReg(rSrc) << std::endl;
        ctxt.advancePC();
    }
};

class HaltInstruction
    : public Instruction
{
private:    
    std::string rSrc;
public:
    HaltInstruction(unsigned _index, std::string _line, std::string _rSrc)
        : Instruction(_index, _line)
        , rSrc(_rSrc)
    {}    
    
    virtual void execute(Context &ctxt) const override
    {
        exit(ctxt.getReg(rSrc));
    }
};

class AddInstruction
    : public Instruction
{
private:
    std::string rDst, rA, rB;
public:
    AddInstruction(unsigned _index, std::string _line, std::string _rDst, std::string _rA, std::string _rB)
        : Instruction(_index, _line)
        , rDst(_rDst)
        , rA(_rA)
        , rB(_rB)
    {}    
    
    virtual std::string getAssembly() const override
    { return "add "+rDst+" "+rA+" "+rB; }


    virtual void execute(Context &ctxt) const override
    {
        ctxt.setReg(rDst, ctxt.getReg(rA)+ctxt.getReg(rB) );
        ctxt.advancePC();
    }
};

class SubInstruction
    : public Instruction
{
private:
    std::string rDst, rA, rB;
public:
    SubInstruction(unsigned _index, std::string _line, std::string _rDst, std::string _rA, std::string _rB)
        : Instruction(_index, _line)
        , rDst(_rDst)
        , rA(_rA)
        , rB(_rB)
    {}    
    
    virtual std::string getAssembly() const override
    { return "sub "+rDst+" "+rA+" "+rB; }


    virtual void execute(Context &ctxt) const override
    {
        ctxt.setReg(rDst, ctxt.getReg(rA)-ctxt.getReg(rB) );
        ctxt.advancePC();
    }
};

class LessThanInstruction
    : public Instruction
{
private:
    std::string rDst, rA, rB;
public:
    LessThanInstruction(unsigned _index, std::string _line, std::string _rDst, std::string _rA, std::string _rB)
        : Instruction(_index, _line)
        , rDst(_rDst)
        , rA(_rA)
        , rB(_rB)
    {}

    virtual void execute(Context &ctxt) const override
    {
        ctxt.setReg(rDst, ctxt.getReg(rA) < ctxt.getReg(rB) );
        ctxt.advancePC();
    }
};

class BranchEqualsInstruction
    : public Instruction
{
private:
    std::string label, rA, rB;
public:
    BranchEqualsInstruction(unsigned _index, std::string _line, std::string _rA, std::string _rB, std::string _label)
        : Instruction(_index, _line)
        , label(_label)
        , rA(_rA)
        , rB(_rB)
    {}    

    virtual void execute(Context &ctxt) const override
    {
        if(ctxt.getReg(rA)==ctxt.getReg(rB)){
            ctxt.jump(label);
        }else{
            ctxt.advancePC();
        }
    }
};

class BranchNotEqualsInstruction
    : public Instruction
{
private:
    std::string label, rA, rB;
public:
    BranchNotEqualsInstruction(unsigned _index, std::string _line, std::string _rA, std::string _rB, std::string _label)
        : Instruction(_index, _line)
        , label(_label)
        , rA(_rA)
        , rB(_rB)
    {}    

    virtual void execute(Context &ctxt) const override
    {
        if(ctxt.getReg(rA)!=ctxt.getReg(rB)){
            ctxt.jump(label);
        }else{
            ctxt.advancePC();
        }
    }
};





void Context::add_instruction(const std::string &line)
{
    char arg1[65]={0}, arg2[65]={0}, arg3[65]={0};
    int iarg;

    if(1==sscanf(line.c_str(), ":%[_a-zA-Z0-9]", arg1)){
        if(labels.find(arg1)!=labels.end()){
            throw std::runtime_error(std::string("Duplicate label : '")+arg1+"'");
        }
        labels[arg1]=instructions.size();
    }else if( 2==sscanf(line.c_str(), "const %64s %d", arg1, &iarg)) {
        instructions.push_back(std::make_shared<ConstInstruction>(instructions.size(), line, arg1, iarg));
    }else if( 1==sscanf(line.c_str(), "input %64s", arg1)) {
        instructions.push_back(std::make_shared<InputInstruction>(instructions.size(),line, arg1));
    }else if( 2==sscanf(line.c_str(), "param %64s %d", arg1, &iarg)) {
        instructions.push_back(std::make_shared<ParamInstruction>(instructions.size(),line, arg1, iarg));
    }else if( 1==sscanf(line.c_str(), "output %64s", arg1)) {
        instructions.push_back(std::make_shared<OutputInstruction>(instructions.size(),line, arg1));
    }else if( 3==sscanf(line.c_str(), "add %64s %64s %64s", arg1, arg2, arg3)) {
        instructions.push_back(std::make_shared<AddInstruction>(instructions.size(),line, arg1, arg2, arg3));
    }else if( 3==sscanf(line.c_str(), "sub %64s %64s %64s", arg1, arg2, arg3)) {
        instructions.push_back(std::make_shared<SubInstruction>(instructions.size(),line, arg1, arg2, arg3));
    }else if( 3==sscanf(line.c_str(), "lt %64s %64s %64s", arg1, arg2, arg3)) {
        instructions.push_back(std::make_shared<LessThanInstruction>(instructions.size(),line, arg1, arg2, arg3));
    }else if( 3==sscanf(line.c_str(), "beq %64s %64s %64s", arg1, arg2, arg3)) {
        instructions.push_back(std::make_shared<BranchEqualsInstruction>(instructions.size(),line, arg1, arg2, arg3));
    }else if( 3==sscanf(line.c_str(), "bne %64s %64s %64s", arg1, arg2, arg3)) {
        instructions.push_back(std::make_shared<BranchNotEqualsInstruction>(instructions.size(),line, arg1, arg2, arg3));     
    }else if( 1==sscanf(line.c_str(), "halt %64s", arg1)) {
        instructions.push_back(std::make_shared<HaltInstruction>(instructions.size(),line, arg1));
    
    }else{
        throw std::runtime_error("Couldn't parse line : "+line);
    }
}

// Side channel for getting global parameters into the instruction
std::vector<int32_t> ParamInstruction::args;

int main(int argc, char *argv[])
{
    try
    {
        
        Context ctxt;
        
        std::ifstream src(argv[1]);
        
        for(int i=2; i<argc; i++){
            ParamInstruction::args.push_back(atol(argv[i]));
        }
        
        std::string line;
        while(std::getline(src, line)){
            fprintf(stderr, "Src: %s\n", line.c_str());
            ctxt.add_instruction(line);
        }
        
        while(1){
            ctxt.step();
        }
        
    }catch(std::exception &e){
        std::cerr<<"Exception: "<<e.what()<<"\n";
        exit(1);
    }
    
    return 0;
}
