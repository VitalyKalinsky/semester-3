if g++ -o ./build/lab4new.exe -fsanitize=address -std=c++23 ./src/lab4new.cpp; then 
    ./build/lab4new.exe;
fi