#include <iostream>
#include <stdio.h>
#include <random>
#include <ctime>
using namespace std;

struct File //Вспомогательная структура, так как возникли проблемы с переоткрытием файлов
{
	FILE* file;
	File(const char* fileName, const char* openMode)
	{
		fopen_s(&file, fileName, openMode);
	}
	~File()
	{
		if (file != NULL)
			fclose(file);
	}
	operator FILE* () const
	{
		return file;
	}
	void rediscoveryFile(const char* newname, const char* openMode)
	{
		fclose(file);
		fopen_s(&file, newname, openMode);
	}
};

bool fileNULL(const char* fileName)// Проверка на файла на пустоту
{
	File f1(fileName, "rt");
	int a, readRes;
	if (feof(f1))
		return false;
	readRes = fscanf_s(f1, "%d", &a);
	if (readRes == EOF)
		return false;
	return true;

}

bool createFileWithRandomNumbers(const char *fileName, const int numbersCount, const int maxNumberValue)//Запись в файл случайных чисел, с передаваемым количеством
{
	std::mt19937 gen(time(0));
	File f1(fileName, "wt");
	if (f1 == NULL) {
		return 0;
	}
	for (int i = 0; i < numbersCount; i++)
	{
		int x = gen() % maxNumberValue;
		fprintf(f1, "%d\n", x);
	}
	fclose(f1);
	return 1;
}

bool isFileContainsSortedArray(const char* fileName)//Проверка файла на упорядоченность (отсортированность)
{
	File f1(fileName, "rt");
	int x, x2;
	fscanf_s(f1, "%d", &x);
	while (!feof(f1))
	{
		fscanf_s(f1, "%d", &x2);
		if (x > x2) {
			fclose(f1);
			return 0;
		}
		x = x2;
	}
	fclose(f1);
	return 1;
}
void fragmintationFile(const char* fileName, const char* filenameA, const char* filenameB)//Функция первичного разбиения
{
	File f1(fileName, "rt");
	File fileA(filenameA, "wt");
	File fileB(filenameB, "wt");
	int x;
	//Делим массив в файле пополам (в случае не делимости на 2, больше на 1 число будет в файле A)
	while (!feof(f1))
	{
		if (!feof(f1)) {
			fscanf_s(f1, "%d", &x);
			fprintf(fileA, "%d\n", x);
		}
		if (!feof(f1)) {
			fscanf_s(f1, "%d", &x);
			fprintf(fileB, "%d\n", x);
		}
	}
}

int countNumberInFile(const char* fileName)//Функция нахождения количества чисел в файле B
{
	File file(fileName, "rt");
	int x;
	int i = 0;
	while (!feof(file))
	{
		fscanf_s(file, "%d", &x);//Возмоно стоило сделать не считывание, а сдвиг по файлу, но так мы точно будет ходить только по числам
		i++;
	}
	return i - 1;
}

void integrationFile(const char* fileNameA, const char* fileNameB, const char* fileNameC, const char* fileNameD, int p)
{
	int len_A = countNumberInFile(fileNameA);//Находит количество чисел в файле A
	int len_B = countNumberInFile(fileNameB);//Находит количество чисел в файле B
	File fileA(fileNameA, "rt");
	File fileB(fileNameB, "rt");
	File fileC(fileNameC, "wt");
	File fileD(fileNameD, "wt");
	int x0, x1;
	fscanf_s(fileA, "%d", &x0);
	fscanf_s(fileB, "%d", &x1);
	bool n = 0;
	int k = 0, m = 0;
	int i, j;
	while (k < len_A && m < len_B) {
		i = j = 0;
		while (k < len_A && m < len_B && i < p && j < p) { // Разбиваем массив пока в одном в одном из файлом не достигнем конца
			if (x0 < x1) {
				if (!n)
					fprintf(fileC, "%d\n", x0);
				else
					fprintf(fileD, "%d\n", x0);
				fscanf_s(fileA, "%d", &x0);
				i++;
				k++;
			}
			else {
				if (!n)
					fprintf(fileC, "%d\n", x1);
				else
					fprintf(fileD, "%d\n", x1);
				fscanf_s(fileB, "%d", &x1);
				j++;
				m++;
			}
		}
		//Дальше смотрим оба файла, в зависимости от того, который НЕкончился, продолжаем запись 
		while (k < len_A && i < p) {
			if (!n)
				fprintf(fileC, "%d\n", x0);
			else
				fprintf(fileD, "%d\n", x0);
			fscanf_s(fileA, "%d", &x0);
			i++;
			k++;
		}
		while (m < len_B && j < p) {
			if (!n)
				fprintf(fileC, "%d\n", x1);
			else
				fprintf(fileD, "%d\n", x1);
			fscanf_s(fileB, "%d", &x1);
			j++;
			m++;
		}
		n = 1 - n;
	}
	//Так как мы работаем с файлами, считывание и запись идут в различном порядке, 
	//поэтому сравниваем исходное количество и текущее (счетчики k и m) и записываем при необходимости
	while (k < len_A) {
		if (!n)
			fprintf(fileC, "%d\n", x0);
		else
			fprintf(fileD, "%d\n", x0);
		fscanf_s(fileA, "%d", &x0);
		k++;
	}
	while (m < len_B) {
		if (!n)
			fprintf(fileC, "%d\n", x1);
		else
			fprintf(fileD, "%d\n", x1);
		fscanf_s(fileB, "%d", &x1);
		m++;
	}
	fclose(fileA);
	fclose(fileB);
	fclose(fileC);
	fclose(fileD);
}

void sortFile(const char* fileName) // Функция сортировки из самого задания (почти ничего не меняла)
{
	const char* fileNameA = "fa.txt", *fileNameB = "fb.txt", *fileNameC = "fc.txt", *fileNameD = "fd.txt";
	fragmintationFile(fileName, fileNameA, fileNameB);
	int p = 1;
	while ((fileNULL(fileNameB) == 1) || (fileNULL(fileNameD) == 1))
	{
		integrationFile(fileNameA, fileNameB, fileNameC, fileNameD, p);
		p = p * 2;
		if ((fileNULL(fileNameB) == 1) || (fileNULL(fileNameD) == 1))
		{
			integrationFile(fileNameC, fileNameD, fileNameA, fileNameB, p);
			p = p * 2;
		}
	}
	if ((fileNULL(fileNameB) == 1)) {
		File fileA(fileNameA, "rt");
		File f1(fileName, "wt");
		int readRes = EOF + 1;
		int x;
		while (!feof(fileA) && readRes != EOF)
		{
			readRes = fscanf_s(fileA, "%d", &x);
			if (readRes != EOF)
				fprintf(f1, "%d\n", x);
		}
	}
	else
	{
		File fileC(fileNameC, "rt");
		File f1(fileName, "wt");
		int readRes = EOF + 1;
		int x;
		while (!feof(fileC) && readRes != EOF)
		{
			readRes = fscanf_s(fileC, "%d", &x);
			if (readRes != EOF)
				fprintf(f1, "%d\n", x);
		}
	}
}

int createAndSortFile(const char* fileName, const int numbersCount, const int maxNumberValue) // Создание и сортировка файла (ничего не меняла)
{
	if (!createFileWithRandomNumbers(fileName, numbersCount, maxNumberValue)) {
		return -1;
	}
	sortFile(fileName); //Вызов вашей функции сортировки
	if (!isFileContainsSortedArray(fileName)) {
		return -2;
	}
	return 1;
}

int main()
{
	const char* fileName = "file.txt";
	const int numbersCount = 1000;
	const int maxNumberValue = 1000000;
	createFileWithRandomNumbers(fileName, numbersCount, maxNumberValue);
	for (int i = 0; i < 1; i++) {
		switch (createAndSortFile(fileName, numbersCount, maxNumberValue)) {
		case 1:
			std::cout << "Test passed." << std::endl;
			break;

		case -1:
			std::cout << "Test failed: can't create file." << std::endl;
			break;

		case -2:
			std::cout << "Test failed: file isn't sorted." << std::endl;
			break;
		}
		return 0;
	}
}