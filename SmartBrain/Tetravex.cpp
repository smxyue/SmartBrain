#include "Tetravex.h"
#include <stdio.h>
#include <random>
#include <math.h>

int Tetravex::jiecheng(int n)
{
    if(n == 1)
        return 1;
    else
        return n*jiecheng(n - 1);
}
void Tetravex::initTryOrder()
{
    for (int i = 0; i < GAMEBLOCKS; i++)
    {
        tryOrder[i] = -1;
        p[i] = -1;
    }
    backList.clear();
    tryIndex = 0;
}
void Tetravex::ascMatrix()
{
    for (int i = 0; i < GAMEBLOCKS * 4; i++)
        matrix[i] += '0';
}
//���ݴ洢��ţ�������ھ����
int Tetravex::neighborCloset(int closet)
{
    int index = (int)(closet / 4.0); //���
    int dir = closet % 4;           //�ھӷ���
    int row = index / GAMESCALE;    //��������
    int col = index % GAMESCALE;    //��������
    switch (dir)
    {
    case 0:
        if (row > 0)
            return closet - 4 * GAMESCALE + 3;  //������һ��ͬ�е�3λ
        else
            return -1;
    case 1:
        if (col > 0)
            return closet - 3;
        else
            return -1;
    case 2:
        if (col < GAMESCALE - 1)
            return closet - 3;
        else
            return -1;
    case 3:
        if (row < GAMESCALE - 1)
            return closet + 4 * GAMESCALE - 3;
        else
            return -1;
    default:
        return -1;

    }
}

void Tetravex::makeGame()
{
    //ȫ���ÿ�
    for (int i = 0; i < GAMEBLOCKS * 4; i++)
    {
        matrix[i] = -1;
    }
    for (int i = 0; i < GAMEBLOCKS * 4; i++)
    {
        //����գ����ھ�
        if (matrix[i] == -1)
        {
            int nc = neighborCloset(i);
            int v = rand()%10;
            if (nc != -1)
            {
                if (matrix[nc] != -1)
                {
                    matrix[i] = matrix[nc];
                }
                else
                {

                    matrix[i] = v;
                    matrix[nc] = v;
                }
            }
            else
            {
                matrix[i] = v;
            }
        }
    }
}

void Tetravex::choasMatrix()
{
    int tmp[GAMEBLOCKS];
    for (int i = 0; i < GAMEBLOCKS; i++)
        tmp[i] = GAMEBLOCKS - 1 - i;    //˳��ȫ����
    for (int i = GAMEBLOCKS; i > 0; i--)
    {
        int room = rand()%i;    //��i��ѡ
        p[i - 1] = tmp[room]; //���ѡһ��
        if (room < i - 1)
        {
            for (int j = room; j < i - 1; j++)     //ѡ�еĺ��ƶ���
            {
                tmp[j] = tmp[j + 1];
            }
        }
    }
}
void Tetravex::inputMatrix()
{
    printf("Input block number one by one(up-left-right-down order:\n\r");
    int count = 0;
    while (count < GAMEBLOCKS * GAMEBLOCKS * 4)
    {
        char ch = getchar();
        if (ch >= '0' && ch <= '9')
        {
            matrix[count] = ch;
            printf("%2d:%c\n\r", count, ch);
            count++;
        }
    }
    printMatrix();
}

void Tetravex::printMatrix()
{
    for (int row = 0;row < GAMEBLOCKS;row++)
    {
        for (int col = 0;col < GAMEBLOCKS;col++)
        {
            int start = (row * GAMEBLOCKS + col) * 4;
            printf("[%d %d %d %d] ", matrix[start], matrix[start + 1], matrix[start + 2], matrix[start + 3]);
        }
        printf("\n\r");
    }
}
bool Tetravex::checkItem(int* pp, int grid, int index) //��λ  ֵλ
{

    int tindex;
    int row = grid / GAMEBLOCKS;
    int col = grid % GAMEBLOCKS;
    //��
    if (row > 0)
    {
        tindex = (row - 1) * GAMEBLOCKS + col; //������ͬ
        tindex = pp[tindex];//��λʵ�ʵĿ��ǵڼ�����
        if (tindex != -1) //û����
        {
            if (matrix[index * 4] != matrix[tindex * 4 + 3])
            {
                return false;
            }
        }
    }
    //��
    if (col << GAMEBLOCKS - 2)
    {
        tindex = row * GAMEBLOCKS + col + 1;
        tindex = pp[tindex];
        if (tindex != -1)
        {
            if (matrix[index * 4 + 2] != matrix[tindex * 4 + 1])
            {
                return false;
            }
        }
    }
    //��
    if (row << GAMEBLOCKS - 2)
    {
        tindex = (row + 1) * GAMEBLOCKS + col;
        tindex = pp[tindex];
        if (tindex != -1)
        {
            if (matrix[index * 4 + 3] != matrix[tindex * 4])
            {
                return false;
            }
        }
    }
    //��
    if (col > 0)
    {
        tindex = row * GAMEBLOCKS + col - 1;
        tindex = pp[tindex];
        if (tindex != -1)
        {
            if (matrix[index * 4 + 1] != matrix[tindex * 4 + 2])
            {
                return false;
            }
        }
    }
    return true;
}

bool Tetravex::checkp()
{
    int tindex;//����λ
    for (int i = 0;i < GAMEBLOCKS * GAMEBLOCKS;i++)//�������9��λ���Ƿ���ȷ
    {
        int index = p[i]; //����λΪi��ʵ����matrix����ĸ�
        if (!checkItem(p, i, index))
            return false;
    }
    return true;
}

bool Tetravex::preCheck(int* pp, int len) //��������   ����
{
    for (int i = 0;i < len;i++)
    {
        if (!checkItem(pp, i, pp[i]))
            return false;
    }
    return true;
}

void Tetravex::qpl(int* chs, int* p, int size) //��ѡ���飬 �������飬���г���
{
    //�����λ��ʼ�����������ѡ�����������Ԫ�أ�Ȼ������˳���ȥ������ʣ�µĿ�ѡλ��������
    if (size == 1)
    {
        p[GAMEBLOCKS * GAMEBLOCKS - 1] = chs[0];
        if (preCheck(p, GAMEBLOCKS * GAMEBLOCKS))
        {
            //printf("try:%d\n\r", count);
            for (int l = 0;l < GAMEBLOCKS * GAMEBLOCKS;l++)
            {
                int r = p[l];
                printf("%d  {%c %c %c %c}  ", r, matrix[r * 4], matrix[r * 4 + 1], matrix[r * 4 + 2], matrix[r * 4 + 3]);
            }
        }
        //count++;
    }
    else
    {
        for (int i = 0;i < size;i++)
        {
            int newP[GAMEBLOCKS * GAMEBLOCKS] = { -1 };
            CloneP(p, newP);
            newP[GAMEBLOCKS * GAMEBLOCKS - size] = chs[i];
            int newChs[GAMEBLOCKS * GAMEBLOCKS];
            CloneChs(chs, newChs, i, size);
            int newSize = size - 1;
            if (preCheck(newP, GAMEBLOCKS * GAMEBLOCKS - size))//���ǰ�������Ѿ����ԣ��������������ж�����Ҫ��̽��
            {
                qpl(newChs, newP, newSize);
            }
        }
    }
}

void Tetravex::CloneChs(int* old, int* clone, int index, int size)
{
    for (int i = 0;i < index;i++)
    {
        clone[i] = old[i];
    }
    for (int i = index;i < size - 1;i++)
    {
        clone[i] = old[i + 1];
    }
}
void Tetravex::CloneP(int* p, int* newp)
{
    //�����Ѿ����кõ�λ�õ�ֵ������
    for (int i = 0;i < 16;i++)
    {
        newp[i] = p[i];
    }
}

void Tetravex::qpl_old(int* chs, int* p, int size) //��ѡ���飬 �������飬���г���
{
    //�����λ��ʼ�����������ѡ�����������Ԫ�أ�Ȼ������˳���ȥ������ʣ�µĿ�ѡλ��������
    if (size == 1)
    {
        p[0] = chs[0];
        if (checkp())
        {
            //printf("try:%d\n\r", count);
            printMatrix();
            for (int l = 0;l < 16;l++)
            {
                int r = p[l];
                printf("%d  {%c %c %c %c}  ", r, matrix[r * 4], matrix[r * 4 + 1], matrix[r * 4 + 2], matrix[r * 4 + 3]);
            }
        }
        //count++;
    }
    else
    {
        for (int i = 0;i < size;i++)
        {
            int newP[16];
            CloneP(p, newP);
            newP[size] = chs[i];
            int newChs[16];
            CloneChs(chs, newChs, i, size);
            int newSize = size - 1;
            qpl(newChs, newP, newSize);
        }
    }
}

bool Tetravex::tryBlock(char room, char block) //block�����roomλ���Ƿ����
{
    char mark = 0;
    char row = room / GAMEBLOCKS;
    char col = room % GAMEBLOCKS;
    char neighborRoom, neighborBlock;   //room�ھӵ���ţ�ʵ�ʿ��
    //˳��鿴����,��ʱ��block�Ǳ��������ڵ�ǰroomλ��λ�õ�
    if (row > 0)
    {
        neighborRoom = room - GAMEBLOCKS;
        neighborBlock = p[neighborRoom];    //���-1��ʾ���λ�û�û����
        if (neighborBlock != -1)
        {
            if (matrix[neighborBlock * 4 + 3] == matrix[block * 4]) //�������
            {
                mark++;
            }
            else
            {
                return false;
            }
        }
    }
    if (col > 0)
    {
        neighborRoom = room - 1;
        neighborBlock = p[neighborRoom];
        if (neighborBlock != -1)
        {
            if (matrix[neighborBlock * 4 + 2] == matrix[block * 4 + 1]) //�������
            {
                mark++;
            }
            else
            {
                return false;
            }
        }
    }
    if (col < (GAMEBLOCKS - 1))
    {
        neighborRoom = room + 1;
        neighborBlock = p[neighborRoom];
        if (neighborBlock != -1)
        {
            if (matrix[neighborBlock * 4 + 1] == matrix[block * 4 + 2]) //�������
            {
                mark++;
            }
            else
            {
                return false;
            }
        }
    }
    if (row < GAMEBLOCKS - 1)
    {
        neighborRoom = room + GAMEBLOCKS;
        neighborBlock = p[neighborRoom];
        if (neighborBlock != -1)
        {
            if (matrix[neighborBlock * 4] == matrix[block * 4 + 3]) //�������
            {
                mark++;
            }
            else
            {
                return false;
            }
        }
    }
    if (mark > 0)
    {
        return true;
    }
    return false;
}
bool Tetravex::isUsed(char block) //��index�Ƿ��Ѿ�ʹ��
{
    for (int i = 0; i <= tryIndex; i++)
    {
        if (tryOrder[i] == block)
        {
            return true;
        }
    }
    // std::list<BackPoint>::iterator it;
    // {
    //     for (it = backList.begin(); it != backList.end(); it++)
    //     {
    //         if (it->neighbor == block)
    //         {
    //             return true;
    //         }
    //     }
    // }
    return false;
}
void Tetravex::printTryOrder()
{
    printf("TryOder:[");
    for (int i = 0; i <= tryIndex; i++)
    {
        printf("%d ", tryOrder[i]);
    }
    printf("]\n\r");
}

void Tetravex::printP(bool block = false)
{
    printf("\n\rPosition:[");
    for (int i = 0; i < GAMEBLOCKS; i++)
    {
        if (p[i] != -1)
        {
            printf("%d ", p[i]);
            if (block)
            {
                printf(":{%c %c %c %c} ",matrix[i*4],matrix[i*4+1],matrix[i*4+2],matrix[i*4+3]);
            }
        }
    }
    printf("]\n\r");
}
char Tetravex::tryEmpty() //��ȡһ����λ����ͼ��䣬�Ѻ�ѡ��ѹջ
{
    char emptyRoom = -1;
    for (char i = 0; i < GAMEBLOCKS; i++)   //���ҿ�λ
    {
        if (p[i] == -1)
        {
            emptyRoom = i;
            break;
        }
    }
    if (emptyRoom == 0) //��һ����λ�Զ�����
    {
        initTryOrder();
        p[0] = firstRoom;
        firstRoom++;
        if (firstRoom >= GAMEBLOCKS)
        {
            return 255; //�޽�
        }
        emptyRoom = 1;
    }
    if (emptyRoom == -1)    //���ȫ���ǿ�
    {
        bool isfull = true;
        for (char i = 0; i < GAMEBLOCKS; i++)
        {
            if (p[i] == -1)
            {
                isfull = false;
            }
        }
        if (isfull)
        {
            return 128;
        }
    }

    int count = 0;
    for (char i = 0; i < GAMEBLOCKS; i++)
    {
        if (!isUsed(i))
        {
            if (tryBlock(emptyRoom, i))
            {
                count++;
                if (count == 1)
                {
                    p[emptyRoom] = i;
                    tryIndex++;
                    tryOrder[tryIndex] = i;
                    printf("%d <- %d{%c,%c,%c,%c}\n\r", emptyRoom, i, matrix[i * 4], matrix[i * 4 + 1], matrix[i * 4 + 2], matrix[i * 4 + 3]);
                }
                else
                {
                    BackPoint backItem;
                    backItem.room = emptyRoom;
                    backItem.neighbor = i;
                    backList.push_back(backItem);
                    printf("%d => %d{%c,%c,%c,%c}\n\r", emptyRoom, i, matrix[i * 4], matrix[i * 4 + 1], matrix[i * 4 + 2], matrix[i * 4 + 3]);
                }
            }
        }
    }
    return count;
}
void Tetravex::printBackList()
{
    printf("backList:\n\r[");
    //for (std::list<BackPoint>::iterator it = backList.begin(); it != backList.end(); ++it)
    {
        //printf("%d:%d ", it->room, it->neighbor);
    }
    printf("]\n\r");
}
void Tetravex::test()
{
    inputMatrix();
    //ascMatrix();
    initTryOrder();
    char backCount = tryEmpty();
    while (backCount >= 0)
    {
        if (backCount == 0)
        {
            if (!backList.empty())
            {
                BackPoint lastBack = backList.back();
                printf("\n\rBack room %d...\n\r", lastBack.room);
                printTryOrder();
                printBackList();
                while ((tryIndex > 0) && (tryOrder[tryIndex] != p[lastBack.room])) //p[lastBack.room]Ϊ���һ���б�ѡ���λ������Ŀ��
                {
                    for (char i = 1; i < GAMEBLOCKS; i++)
                    {
                        if (p[i] == tryOrder[tryIndex]) //�ҵ��ÿ�������λ�ã������ϣ�ͬʱ����̽����Ҳ���ϸÿ顣
                        {
                            printf("x%d ", tryOrder[tryIndex]);
                            p[i] = -1;
                            tryOrder[tryIndex] = -1;
                            tryIndex--;
                            break;
                        }
                    }
                }
                p[lastBack.room] = lastBack.neighbor;//��������̽��
                char lastRoom = lastBack.room;
                char lastNeighbor = lastBack.neighbor;
                printf("%d <= %d\n\r", lastBack.room, lastBack.neighbor);
                backList.pop_back();    //���� ���ϸ���̽��
                lastBack = backList.back();
                if (lastBack.room == lastRoom) //��λ���б�ѡ����λ����̽�б�Ҫ����������ɾ��
                {
                    // tryIndex ++;
                }
                tryOrder[tryIndex] = lastNeighbor;//����̽����Ҳ����

            }
            else//�ޱ�ѡ���ھ�,���¿�ʼ
            {
                initTryOrder();
                backCount = tryEmpty();
            }
        }
        else if (backCount == 128)
        {
            printf("\n\rGot you:\n\r");
            printP(true);
        }
        else if (backCount == 255)
        {
            printf("\n\rNo anwser!");
        }
        backCount = tryEmpty();
    }
    printf("Done!\n\r");
    printP(true);
}