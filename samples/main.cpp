#include <iostream>
#include <limits>
#include "polinom.h"

void printMenu();
void printInputRules();

int main() {
	setlocale(LC_ALL, "Russian");
	Polinom A, B;
	bool polinomInitialized = false;

	std::cout << "Программа для работы с полиномами от трёх переменных (x, y, z)\n";
	while (true) {
		if (!polinomInitialized) {
			printInputRules();

			std::cout << "Введите полином A: ";
			std::cin >> A;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			std::cout << "Введите полином B: ";
			std::cin >> B;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			std::cout << "\nПолином A: " << A << std::endl;
			std::cout << "Полином B: " << B << std::endl;

			polinomInitialized = true;
		}

		printMenu();

		int choice;
		std::cin >> choice;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (choice == 0) {
			std::cout << "Программа завершена.\n";
			break;
		}

		try {
			switch (choice) {
			case 1: {
				Polinom C = A + B;
				std::cout << "\nРезультат A + B = " << C << std::endl;
				break;
			}
			case 2: {
				Polinom C = A - B;
				std::cout << "\nРезультат A - B = " << C << std::endl;
				break;
			}
			case 3: {
				double c;
				std::cout << "Введите константу: ";
				std::cin >> c;
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				Polinom C = A * c;
				std::cout << "\nРезультат A * " << c << " = " << C << std::endl;
				break;
			}
			case 4: {
				Polinom C = A * B;
				std::cout << "\nРезультат A * B = " << C << std::endl;
				break;
			}
			case 5: {
				std::cout << "\nПолином A: " << A << std::endl;
				break;
			}
			case 6: {
				std::cout << "\nПолином B: " << B << std::endl;
				break;
			}
			case 7: {
				polinomInitialized = false;
				break;
			}
			default: {
				std::cout << "Неверный выбор. Попробуйте снова.\n";
				break;
			}
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Ошибка: " << e.what() << std::endl;
		}

		std::cout << "\nНажмите Enter для продолжения...";
		std::cin.get();
	}

	return 0;
}

void printMenu() {
	std::cout << "\n========== КАЛЬКУЛЯТОР ПОЛИНОМОВ ==========\n";
	std::cout << "Доступные операции:\n";
	std::cout << "1. Сложение (A + B)\n";
	std::cout << "2. Вычитание (A - B)\n";
	std::cout << "3. Умножение на константу (A * c)\n";
	std::cout << "4. Умножение полиномов (A * B)\n";
	std::cout << "5. Вывод полинома A\n";
	std::cout << "6. Вывод полинома B\n";
	std::cout << "7. Ввести новые полиномы\n";
	std::cout << "0. Выход\n";
	std::cout << "==============================\n";
	std::cout << "Выберите операцию: ";
}

void printInputRules() {
	std::cout << "\nПРАВИЛА ВВОДА ПОЛИНОМОВ:\n";
	std::cout << "1. Полином выводится в виде строки, например: 2x^2y^3z + 3.5xy - 4z^2\n";
	std::cout << "2. Допускаются степени переменных от 0 до 9\n";
	std::cout << "3. Переменные могут быть в любом порядке\n";
	std::cout << "4. Коэффициенты - вещественные числа\n";
	std::cout << "5. Примеры правильных записей: \n";
	std::cout << "    - 2x^2y^3z\n";
	std::cout << "    - -3.5xy^2\n";
	std::cout << "    - xyz + 2y^2z - 5\n";
	std::cout << "    - 2x^2 + 3y - 4z^3\n";
	std::cout << "\n";
}