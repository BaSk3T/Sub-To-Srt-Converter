#include <stdio.h>
#include <locale.h>

typedef int bool;
#define true 1
#define false 0

int main()
{
	setlocale(LC_ALL, "en_US.UTF-8");

	char subPath[256];
	int symbol;
	bool readTimeInterval = false;
	char intervalString[20];
	size_t index = 0;
	int interval;
	double frameRate;
	FILE *file = NULL;
	FILE *result;
	int mil;
	int seconds;
	int minutes;
	int hours;
	int line = 1;
	int intervalsCount = 0;
	int choice = -1;

	printf("Choose frame rate\n1 - 23.976\n2 - 25\n");
	
	while (choice != 1 && choice != 2) {
		printf("Enter choice: ");
		scanf("%d", &choice);
	}

	// remove \n (new line) from scanf
	fgetc(stdin);

	switch (choice)
	{
	case 1:
		frameRate = 41.7;
		break;
	case 2:
		frameRate = 40;
		break;
	default:
		break;
	}

	while (file == NULL) {
		printf("Enter file name in format \"name.extension\": ");
		fgets(subPath, sizeof(subPath), stdin);
		strtok(subPath, "\n");

		// open stream
		file = fopen(subPath, "r");
	}

	subPath[strlen(subPath) - 4] = NULL;
	snprintf(subPath, 256, "%s.srt", subPath);
	result = fopen(subPath, "w+");

	if (file) {
		while ((symbol = fgetc(file)) != EOF) {
			if (symbol == '}') {
				readTimeInterval = false;
				index = 0;
				interval = atoi(intervalString);

				if (interval == 0) {
					continue;
				}

				memset(&intervalString[0], NULL, sizeof(intervalString));
				interval *= frameRate;

				mil = (int)(interval % 1000);
				seconds = (int)(interval / 1000) % 60;
				minutes = (int)((interval / (1000 * 60)) % 60);
				hours = (int)((interval / (1000 * 60 * 60)) % 60);

				if (intervalsCount % 2 == 0) {
					snprintf(intervalString, 20, "%d\n%d:%d:%d,%d --> ", line, hours, minutes, seconds, mil);
				}
				else {
					snprintf(intervalString, 20, "%d:%d:%d,%d\n", hours, minutes, seconds, mil);
					line++;
				}

				intervalsCount++;
				fputs(intervalString, result);
				continue;
			}

			if (symbol == '{') {
				memset(&intervalString[0], NULL, sizeof(intervalString));
				readTimeInterval = true;
				continue;
			}

			if (readTimeInterval) {
				intervalString[index] = symbol;
				index++;
			}
			else {
				if (symbol == '\n') {
					fputc('\n', result);
				}

				if (symbol == '|') {
					symbol = '\n';
				}

				fputc(symbol, result);
			}
		}
	}

	// close stream
	fclose(file);
	fclose(result);
}