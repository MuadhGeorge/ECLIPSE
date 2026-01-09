#include "ui.h"
#include <iostream>

int main() {
    try {
        eclipse::UI ui;
        
        if (!ui.init()) {
            std::cerr << "Failed to initialize UI" << std::endl;
            return 1;
        }
        
        ui.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
