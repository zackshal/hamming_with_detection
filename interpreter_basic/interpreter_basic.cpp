#include <iostream>
#include <vector>

class Interp {
public:
    static int PC;
    static int AC;
    static int instr;
    static int instr_type;
    static int data_loc;
    static int ndata;
    static bool run_bits;

    static void interpreter(std::vector<int>& memory, int starting_address) {
        PC = starting_address;
        run_bits = true;

        while (run_bits && PC < memory.size()) {
            instr = memory[PC];
            PC = PC + 1;
            instr_type = get_instr_type(instr);
            data_loc = find_data(instr, instr_type);

            if (data_loc >= 0 && data_loc < memory.size()) {
                ndata = memory[data_loc];
            }
            else {
                ndata = 0; // значение по умолчанию
            }

            execute(instr_type, ndata);
        }
    }

private:
    static int get_instr_type(int instr) {
        return (instr >> 8) & 0xFF;
    }

    static int find_data(int instr, int type) {
        return instr & 0xFF;
    }

    static void execute(int type, int data) {
        switch (type) {
        case 0: // Загрузка
            AC = data;
            break;
        case 1: // Сложение
            AC += data;
            break;
        case 2: // Завершение
            run_bits = false;
            break;
        default:
            std::cout << "Неизвестный тип инструкции: " << type << std::endl;
            run_bits = false;
            break;
        }
    }
};

// Инициализация статических переменных
int Interp::PC = 0;
int Interp::AC = 0;
int Interp::instr = 0;
int Interp::instr_type = 0;
int Interp::data_loc = 0;
int Interp::ndata = 0;
bool Interp::run_bits = true;

int main() {
    // Пример памяти: инструкция загрузки из адреса 1, потом завершение
    std::vector<int> memory = {
        0x0001, // Загрузка из адреса 1
        42,     // Данные по адресу 1
        0x0200  // Завершить (тип 2, адрес не важен)
    };

    Interp::interpreter(memory, 0);
    std::cout << "AC = " << Interp::AC << std::endl;

    return 0;
}
