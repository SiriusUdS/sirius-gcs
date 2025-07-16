#ifndef COMMANDTYPE_H
#define COMMANDTYPE_H

enum class CommandType : size_t {
    FillValve,
    DumpValve,
    FillHeatPad,
    DumpHeatPad,
    Abort,
    Reset,

    Count // Always keep this as the last element
};

#endif // COMMANDTYPE_H
