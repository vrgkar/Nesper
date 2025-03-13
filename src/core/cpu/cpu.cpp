#include "cpu.h"

uint8_t CPU::read(uint16_t addr)
{
    switch (addr)
    {
        case 0x4014:    return m_r.oamdma;
        case 0x4016:    return m_r.joystrobe;   
    }

    return 0;
}

void CPU::write(uint8_t byte, uint16_t addr)
{
    switch (addr)
    {
        case 0x4014:    m_r.oamdma = byte; m_bus->broadcast(this, Event::OAMDMA); break;
        case 0x4016:    m_r.joystrobe = byte;  break;
    }
}

void CPU::process_events()
{
    for (auto event : m_events)
    {
        switch (event)
        {
            case Event::BRK:    break;
            case Event::IRQ:    break;
            case Event::NMI:    m_state = State::INTERRUPT; m_interrupt = Interrupt::NMI; break;
            case Event::NONE:   break;
        }
    }

    m_events.clear();
}

std::string CPU::disassemble(uint16_t addr)
{
    auto pstate = m_state;

   // m_state.r.pc = addr;

    m_state = pstate;

    return "";
}

void CPU::step()
{
    if (m_state == State::SUSPEND)
        return;

    ++m_cycles;

    if (!m_log)
        m_log.open("log.txt", std::ios::out);

    if (m_state == State::FETCH)
        fetch();
    
    if (m_state == State::DECODE)
    {
        decode();
        m_log << std::hex << std::uppercase << m_r.pc-1 << " " << +m_opcode << " " << instruction_to_string(m_instr) << " "
        << mode_to_string(m_mode) << "\t\t" << "A:" << (int)m_r.a << " X:" << (int)m_r.x << " Y:" << (int)m_r.y
        << " P:" << (int)m_r.sr << " SP: " << (int)m_r.sp  << " CYC:" << std::dec << m_cycles << std::endl;
    }

    if (m_state == State::EXECUTE_MODE)
        execute_mode();

    if (m_state == State::EXECUTE_INSTRUCTION)
        execute_instruction();
        
    if (m_state == State::INTERRUPT)
        interrupt();

    /* FIXME: Events after the second-last cycle of an instruction shouldn't be considered. */
    if (m_state == State::FETCH)
        process_events();
}

void CPU::step_instruction()
{
    do
        step();
    while (m_state != State::FETCH);
}

std::string CPU::get_instr_disassembly()
{
    return "";
}

std::string CPU::instruction_to_string(Instruction instr)
{
    switch (instr) 
    {
    case Instruction::ADC:         return "ADC";
    case Instruction::AND:         return "AND";
    case Instruction::ASL:         return "ASL";
    case Instruction::BCC:         return "BCC";
    case Instruction::BCS:         return "BCS";
    case Instruction::BEQ:         return "BEQ";
    case Instruction::BIT:         return "BIT";
    case Instruction::BMI:         return "BMI";
    case Instruction::BNE:         return "BNE";
    case Instruction::BPL:         return "BPL";
    case Instruction::BRK:         return "BRK";
    case Instruction::BVC:         return "BVC";
    case Instruction::BVS:         return "BVS";
    case Instruction::CLC:         return "CLC";
    case Instruction::CLD:         return "CLD";
    case Instruction::CLI:         return "CLI";
    case Instruction::CLV:         return "CLV";
    case Instruction::CMP:         return "CMP";
    case Instruction::CPX:         return "CPX";
    case Instruction::CPY:         return "CPY";
    case Instruction::DEC:         return "DEC";
    case Instruction::DEX:         return "DEX";
    case Instruction::DEY:         return "DEY";
    case Instruction::EOR:         return "EOR";
    case Instruction::INC:         return "INC";
    case Instruction::INX:         return "INX";
    case Instruction::INY:         return "INY";
    case Instruction::JMP:         return "JMP";
    case Instruction::JSR:         return "JSR";
    case Instruction::LDA:         return "LDA";
    case Instruction::LDX:         return "LDX";
    case Instruction::LDY:         return "LDY";
    case Instruction::LSR:         return "LSR";
    case Instruction::NOP:         return "NOP";
    case Instruction::ORA:         return "ORA";
    case Instruction::PHA:         return "PHA";
    case Instruction::PHP:         return "PHP";
    case Instruction::PLA:         return "PLA";
    case Instruction::PLP:         return "PLP";
    case Instruction::ROL:         return "ROL";
    case Instruction::ROR:         return "ROR";
    case Instruction::RTI:         return "RTI";
    case Instruction::RTS:         return "RTS";
    case Instruction::SBC:         return "SBC";
    case Instruction::SEC:         return "SEC";
    case Instruction::SED:         return "SED";
    case Instruction::SEI:         return "SEI";
    case Instruction::STA:         return "STA";
    case Instruction::STX:         return "STX";
    case Instruction::STY:         return "STY";
    case Instruction::TAX:         return "TAX";
    case Instruction::TAY:         return "TAY";
    case Instruction::TSX:         return "TSX";
    case Instruction::TXA:         return "TXA";
    case Instruction::TXS:         return "TXS";
    case Instruction::TYA:         return "TYA";      
    }

    return "";
}

std::string CPU::mode_to_string(Mode mode)
{
    switch (mode) 
    {
    case Mode::IMP:         return "IMP";
    case Mode::IMM:         return "IMM";
    case Mode::ZPG:         return "ZPG";
    case Mode::ZPX:         return "ZPX";
    case Mode::ZPY:         return "ZPY";
    case Mode::ACC:         return "ACC";
    case Mode::REL:         return "REL";
    case Mode::ABS:         return "ABS";
    case Mode::ABX:         return "ABX";
    case Mode::ABY:         return "ABY";
    case Mode::IND:         return "IND";
    case Mode::IDX:         return "IDX";
    case Mode::IDY:         return "IDY";  
    }

    return "";
}