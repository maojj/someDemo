#include <stdio.h>

typedef int uint8_t;
typedef int uint16_t;
typedef int uint32_t;

#define MAXLENGTH 10

int calNum(uint32_t number, uint8_t *buffer, uint8_t bufferSize)
{
    uint8_t index = 0;

    do
    {
        buffer[index++] = number % 10;
        number = number / 10;
    } while (number != 0);

    return index;
}

void calTwoMulti(uint32_t number1, uint32_t number2, uint8_t digitals[MAXLENGTH][MAXLENGTH], uint8_t status[MAXLENGTH][MAXLENGTH])
{
    uint8_t buffer1[MAXLENGTH];
    uint8_t buffer2[MAXLENGTH];
    uint8_t length1;
    uint8_t length2;
    uint32_t tempResult;
    uint8_t tempLength;
    uint8_t index;
    uint8_t x;
    uint8_t y;
    uint8_t j;
    uint8_t sumIndex;
    uint8_t lastLineMaxLength;
    uint8_t tempSum;

    for(x=0;x<MAXLENGTH;x++) {
        for(y=0;y<MAXLENGTH;y++) {
            digitals[x][y]=0;
            status[x][y]=0;
        }
    }

    // 第一行
    length1 = calNum(number1, digitals[0], sizeof(digitals[0]));
    for (index = 0; index < length1; index++)
    {
        status[0][index] = 1;
    }

    // 第二行
    length2 = calNum(number2, digitals[1], sizeof(digitals[1]));
    for (index = 0; index < length2; index++)
    {
        status[1][index] = 1;
    }

    // 每一步乘法
    for (index = 0; index < length2; index++)
    {
        tempResult = number1 * digitals[1][index];
        tempLength = calNum(tempResult, &digitals[2 + index][index], MAXLENGTH - index);
        for (j = index; j < index + tempLength; j++)
        {
            status[2 + index][j] = 2; // 答案，也就是用户输入框，该用户填入
        }
    }

    sumIndex = 2 + index;
    lastLineMaxLength = j;
    // 累加求和
    for (index = 0; index < lastLineMaxLength; index++)
    {
        tempSum = 0;
        for (j = 2; j <= sumIndex; j++) //累加到结果行，结果行可能存了进位。
        {
            tempSum += digitals[j][index];
        }

        digitals[sumIndex][index] = tempSum % 10;
        digitals[sumIndex][index + 1] = (uint8_t)(tempSum / 10);
        
        status[sumIndex][index] = 2;

        if (digitals[sumIndex][index + 1] > 0)
        {
            status[sumIndex][index + 1] = 2;
        }
    }

    for (index = 0; index <= sumIndex; index++)
    {
        for (j = MAXLENGTH - 1; j >= 0; j--)
        {
            printf("%d ", digitals[index][j]);
        }
        printf("\n");
        if(index == 1 || index == sumIndex - 1) {
            printf("--------------------\n");
        }
    }

    index = 1;
}

int main()
{
    uint8_t dis[MAXLENGTH][MAXLENGTH];
    uint8_t stats[MAXLENGTH][MAXLENGTH];

    calTwoMulti(9876, 8763, dis, stats);
    return 0;
}