# cache-emulator

В этом репозитории находится реализация эмулятора 2-х уровневого кэша.

Результатом работы программы является процент кэш-промахов для каждого из уровней кэша. Для того чтобы посмотреть результат работы для других программ нужно поменять функцию в файле ```function.hpp```, чтобы изменить размер кэша, кэш-линии ... нужно поменять соответствующее значение в файле ```Memory.hpp```.

Пример запуска программы из консоли:
```
make main
./main
```

Пример вывода программы:
```
L1: 91.6667%
L2: 0%
```
