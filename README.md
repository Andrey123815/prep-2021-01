# # Подготовительная программа на С/С++

## Домашнее задание №5

Необходимо портировать библиотеку для работы с матрицами с C на C++.
Требования и условия аналогичны ДЗ №3.

Выполнение задания подразумевает использование стандарта C++17. Чтобы
 использовать возможности 17 стандарта нужен gcc версии не ниже, чем 7.
Проверить можно так `gcc -v`.

Как накатить новый gcc в Ubuntu написано
 [тут](https://gist.github.com/jlblancoc/99521194aba975286c80f93e47966dc5)

Использовать простые типы данных не стоит. Вместо `сonst char*` осваиваем
 `std::string`, вместо `int*` - `std::vector<int>` и т.д.
Это готовые обертки для этих типов данных, смысл существования которых
 идентичен идее этого домашнего задания.

Для сборки используется `cmake`. Установка последней версии
 описана [тут](https://cmake.org/install/)

Для тестирования используется gtest.
Установка - `sudo apt-get install --yes libgtest-dev`

## Описание интерфейса

* **_project/include/matrix.h_** - интерфейс.
* **_project/include/exception.h_** - исключения, используемые в библиотеке.

Библиотека находится в пространстве имен `prep`.

* `class Matrix` - основной класс.

### Конструкторы

* `Matrix::Matrix(size_t rows = 0, size_t cols = 0)` - создать пустую матрицу
 размера `<rows>*<cols>`.
* `Matrix::Matrix(std::istream& is)` - создание матрицы из потока. В случае
 проблем чтения бросает исключение `InvalidMatrixStream`.

### Базовые методы

* `size_t Matrix::getRows() const` - получить количество строк.
* `size_t Matrix::getCols() const` - получить количество столбцов.

* `double Matrix::operator()(size_t i, size_t j) const` - получить значение
 элемента на позиции `[<i>, <j>]`.
* `double& Matrix::operator()(size_t i, size_t j)` - получить **элемент** на
 позиции `[<i>, <j>]` для присваивания.

В случае выхода за границы матрицы бросают исключение `OutOfRange`.

Операторы сравнения:
* `bool Matrix::operator==(const Matrix& rhs) const` 
* `bool Matrix::operator!=(const Matrix& rhs) const`

**_NB_** Для неточного сравнения `double` пригодится
 `std::numeric_limits<double>::epsilon()`. **UPD** В тестах все значения имеют
  точность `1e-07`.

* `std::ostream& operator<<(std::ostream& os, const Matrix& matrix)` -
 перегруженный оператор вывода матрицы в поток.

**_NB_** Тут понадобится `std::setprecision()` и
 `std::numeric_limits<double>::max_digits10`

### Базовые математические операции

* `Matrix Matrix::operator+(const Matrix& rhs) const` - сложение матриц.
* `Matrix Matrix::operator-(const Matrix& rhs) const` - вычитание матриц.
* `Matrix Matrix::operator*(const Matrix& rhs) const` - умножение матриц.

В случае несоответствия размерностей бросают исключение `DimensionMismatch`.

* `Matrix Matrix::transp() const` - транспонирование матрицы.

Умножение матрицы на скаляр:
* `Matrix Matrix::operator*(double val) const` - правостороннее.
* `Matrix operator*(double val, const Matrix& matrix)` - левостороннее.
 
### Дополнительные математические операции

* `double det() const` - определитель матрицы.
* `Matrix adj() const` - присоединенная матрица.
* `Matrix inv() const` - обратная матрица. Брасает `SingularMatrix`.

Брасают исключения `DimensionMismatch`.

## Не забываем

Запустить линтеры:

```bash
$ ./linters/run.sh
```

Запустить тесты:

```bash
$ ctest -V -R Base
$ ctest -V -R Extra
```
