# Мастерская № 15 "Физическая песочница"

## Цели мастерской(после внесенных изменений)
Познакомиться с физикой, ориентируясь на программу летней школы ФМШ, а также с моделированием как одним из эффективных на сегодняшний день решением различных задач.
Перед этим предстояло пробежаться по языку C, повторить основные моменты, нужные для работы в мастерской. По факту пришлось изучать его с нуля.

# Сборка
Необходимо скомпилировать файлы проектов с импользованием библиотеки freeglut.h. Далее запускаем исполняемый файл и следуем инструкциям для каждого проекта.

catapult.c
Запускается снаряд из "катапульты".
Клавиши z, x - смена позиции камеры.
r - вернуть снаряд в начальное положение.
y - запустить эсперимент.

planet.c
Симулятор взаимодействия планет. Количество планет задается вначале. Планеты помещаются на экран случайным образом. В программу заложены, но не задаются такие параметры, как масса, радиус и вектор скорости планет.

box.c
Модельный эксперимент поведения шарика в "коробке". Программа просит ввести такие параметры, как проекции вектора скорости по координатам и радиус сферы. На клавиши q, w, a, s реализовано вращение камеры.