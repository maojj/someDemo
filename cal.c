#include <stdio.h>

typedef int uint8_t;
typedef int uint16_t;
typedef int uint32_t;

#define MAXLENGTH 10
#define MAXHIEGHT 20

void printDigitals(uint8_t digitals[MAXHIEGHT][MAXLENGTH]) {
    uint8_t x,y;
    for (x = 0; x < MAXHIEGHT; x++)
    {
        for (y = MAXLENGTH - 1; y >= 0; y--)
        {
            printf("%d ", digitals[x][y]);
        }
        printf("\n");
    }
}


uint8_t calNum(uint32_t number, uint8_t *buffer, uint8_t bufferSize)
{
    uint8_t index = 0;
    do
    {
        buffer[index++] = number % 10;
        number = number / 10;
    } while (number != 0);

    return index;
}

uint32_t calValue(uint8_t *buffer, uint8_t bufferSize) {
    uint32_t result = 0;
    uint32_t factor = 1;
    uint8_t y = 0;
    while(y < bufferSize) {
        result += buffer[y] * factor;
        factor *= 10;
        y++;
    }
    return result;
}


// 对矩形区域所在的 endX-startX 行 进行累加， 结果存入 endX + 1 行中
void addDigitals(uint8_t digitals[MAXHIEGHT][MAXLENGTH], uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY) {
    uint8_t x,y;
    uint8_t tempSum;
    uint8_t targetX = endX + 1;

    // 结果位置清零
    for(y= startY; y<=endY; y++ ) {
        digitals[targetX][y]=0;
    }

    // 按列求和
    for (y = startY; y < endY; y++)
    {
        tempSum = 0;
        for (x = startX; x <= targetX; x++) //累加到结果行，结果行可能存了进位。
        {
            tempSum += digitals[x][y];
        }

        digitals[targetX][y] = tempSum % 10;
        digitals[targetX][y + 1] = (uint8_t)(tempSum / 10);
    }
}

void minus(uint8_t digitals[MAXHIEGHT][MAXLENGTH], uint8_t startX, uint8_t startY, uint8_t endX, uint8_t endY) {
    uint32_t total = calValue(&digitals[startX][startY], endY - startY + 1);
    uint32_t temp;
    uint8_t x = startX + 1;
    for(; x <= endX; x++)
    {
        temp = calValue(&digitals[x][startY], endY - startY + 1);
        total -= temp;
    }

    calNum(total, &digitals[endX + 1][startY], endY - startY + 1);
}

void calTwoMulti(uint32_t number1, uint32_t number2, uint8_t digitals[MAXHIEGHT][MAXLENGTH], uint8_t status[MAXHIEGHT][MAXLENGTH])
{
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

    for(x=0;x<MAXHIEGHT;x++) {
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

    // sumIndex 结果所在行
    sumIndex = 2 + index;
    // 最后一行乘积的长度
    lastLineMaxLength = j;

    // 累加求和
    addDigitals(digitals, 2, 0, sumIndex - 1, lastLineMaxLength);
}

void calTwoDivide(uint32_t dividend, uint32_t divisor,  uint8_t digitals[MAXHIEGHT][MAXLENGTH], uint8_t status[MAXHIEGHT][MAXLENGTH]) {
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
    uint32_t quotient;
    uint32_t remainder;
    uint8_t startY;
    uint8_t endY;

    for(x=0;x<MAXHIEGHT;x++) {
        for(y=0;y<MAXLENGTH;y++) {
            digitals[x][y]=0;
            status[x][y]=0;
        }
    }

    // 除数 放第一行，商放第二行，被除数放第三行，方便加减
    length2 = calNum(divisor, digitals[0], sizeof(digitals[0]));
    for (index = 0; index < length2; index++)
    {
        status[0][index] = 1;
    }

    // 被除数
    length1 = calNum(dividend, digitals[2], sizeof(digitals[2]));
    for (index = 0; index < length1; index++)
    {
        status[2][index] = 1;
    }

    // 计算商和余数
    quotient = dividend / divisor;
    remainder = dividend % divisor;
    tempLength = calNum(quotient, digitals[1], sizeof(digitals[1]));
    for (index = 0; index < tempLength; index++)
    {
        status[1][index] = 1;
    }

    // 找到商最高位， 开始每一位乘以除数， 填入下一行，计算差， 补齐后继续。
    index = MAXLENGTH - 1;
    while(index >= 0 && digitals[1][index] == 0) {
        index--;
    }

    x = 3; // 第四行开始
    for(y = index;y >= 0; y--) {
        // 计算出商的每一位与除数的乘积， 然后想减
        tempResult = digitals[1][y] * divisor;
        tempLength = calNum(tempResult, &digitals[x][y], MAXLENGTH - y);

        startY = y;
        endY = y + tempLength + 1;
        minus(digitals, x - 1, startY ,x, endY);

        x++;
        if(y > 0) {
            // 减完后，补一位,进行下次循环
            digitals[x][y-1] = digitals[2][y-1];
        }
        x++;
    }
}

int main()
{
    uint8_t dis[MAXHIEGHT][MAXLENGTH];
    uint8_t stats[MAXHIEGHT][MAXLENGTH];

    printf("\nmulti\n");
    calTwoMulti(9876, 8763, dis, stats);
    printDigitals(dis);

    uint32_t num1 = calValue(dis[6], MAXLENGTH);

    printf("\ndivide\n");
    calTwoDivide(98737, 34, dis, stats);
    printDigitals(dis);

    return 0;
}