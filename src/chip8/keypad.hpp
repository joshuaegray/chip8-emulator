#pragma once
#include <cstdint>
class Keypad{
public:
    void setKey(uint8_t key, bool pressed);
    bool isPressed(uint8_t key) const;
    bool anyKeyPressed() const;

private:
    bool keys[16] = {false};
};