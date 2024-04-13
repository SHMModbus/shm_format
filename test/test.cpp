/*
 * Copyright (C) 2022 Nikolas Koesling <nikolas@koesling.info>.
 * This template is free software. You can redistribute it and/or modify it under the terms of the MIT License.
 */

#include <cstdlib>

int main() {
    int  result      = std::system("../Template_Project");
    auto exit_status = WEXITSTATUS(result);
    return exit_status;
}
