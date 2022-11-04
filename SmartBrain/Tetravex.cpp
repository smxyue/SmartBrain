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
//根据存储序号，获得其邻居序号
int Tetravex::neighborCloset(int closet)
{
    int index = (int)(closet / 4.0); //块号
    int dir = closet % 4;           //邻居方向
    int row = index / GAMESCALE;    //块所在行
    int col = index % GAMESCALE;    //块所在列
    switch (dir)
    {
    case 0:
        if (row > 0)
            return closet - 4 * GAMESCALE + 3;  //返回上一行同列的3位
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
    //全部置空
    for (int i = 0; i < GAMEBLOCKS * 4; i++)
    {
        matrix[i] = -1;
    }
    for (int i = 0; i < GAMEBLOCKS * 4; i++)
    {
        //如果空，看邻居
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
        tmp[i] = GAMEBLOCKS - 1 - i;    //顺序全排列
    for (int i = GAMEBLOCKS; i > 0; i--)
    {
        int room = rand()%i;    //第i次选
        p[i - 1] = tmp[room]; //随机选一个
        if (room < i - 1)
        {
            for (int j = room; j < i - 1; j++)     //选中的后移丢弃
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
bool Tetravex::checkItem(int* pp, int grid, int index) //格位  值位
{

    int tindex;
    int row = grid / GAMEBLOCKS;
    int col = grid % GAMEBLOCKS;
    //上
    if (row > 0)
    {
        tindex = (row - 1) * GAMEBLOCKS + col; //行少列同
        tindex = pp[tindex];//该位实际的块是第几个。
        if (tindex != -1) //没有填
        {
            if (matrix[index * 4] != matrix[tindex * 4 + 3])
            {
                return false;
            }
        }
    }
    //右
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
    //下
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
    //左
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
    int tindex;//相邻位
    for (int i = 0;i < GAMEBLOCKS * GAMEBLOCKS;i++)//检查所有9个位置是否都正确
    {
        int index = p[i]; //当排位为i的实际是matrix里的哪个
        if (!checkItem(p, i, index))
            return false;
    }
    return true;
}

bool Tetravex::preCheck(int* pp, int len) //排列数组   长度
{
    for (int i = 0;i < len;i++)
    {
        if (!checkItem(pp, i, pp[i]))
            return false;
    }
    return true;
}

void Tetravex::qpl(int* chs, int* p, int size) //待选数组， 排序数组，排列长度
{
    //从最低位开始，依次填入待选数组里的所有元素，然后把这个顺序号去掉，用剩下的可选位排其他的
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
            if (preCheck(newP, GAMEBLOCKS * GAMEBLOCKS - size))//如果前置序列已经不对，后续的所有序列都不需要试探了
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
    //复制已经排列好的位置的值，并把
    for (int i = 0;i < 16;i++)
    {
        newp[i] = p[i];
    }
}

void Tetravex::qpl_old(int* chs, int* p, int size) //待选数组， 排序数组，排列长度
{
    //从最低位开始，依次填入待选数组里的所有元素，然后把这个顺序号去掉，用剩下的可选位排其他的
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

bool Tetravex::tryBlock(char room, char block) //block块放在room位置是否合适
{
    char mark = 0;
    char row = room / GAMEBLOCKS;
    char col = room % GAMEBLOCKS;
    char neighborRoom, neighborBlock;   //room邻居的序号，实际块号
    //顺序查看四周,此时，block是被看成是在当前room位置位置的
    if (row > 0)
    {
        neighborRoom = room - GAMEBLOCKS;
        neighborBlock = p[neighborRoom];    //块号-1表示这个位置还没有数
        if (neighborBlock != -1)
        {
            if (matrix[neighborBlock * 4 + 3] == matrix[block * 4]) //检查上邻
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
            if (matrix[neighborBlock * 4 + 2] == matrix[block * 4 + 1]) //检查左邻
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
            if (matrix[neighborBlock * 4 + 1] == matrix[block * 4 + 2]) //检查右邻
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
            if (matrix[neighborBlock * 4] == matrix[block * 4 + 3]) //检查下邻
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
bool Tetravex::isUsed(char block) //块index是否已经使用
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
char Tetravex::tryEmpty() //获取一个空位，试图填充，把候选项压栈
{
    char emptyRoom = -1;
    for (char i = 0; i < GAMEBLOCKS; i++)   //查找空位
    {
        if (p[i] == -1)
        {
            emptyRoom = i;
            break;
        }
    }
    if (emptyRoom == 0) //第一个空位自动填入
    {
        initTryOrder();
        p[0] = firstRoom;
        firstRoom++;
        if (firstRoom >= GAMEBLOCKS)
        {
            return 255; //无解
        }
        emptyRoom = 1;
    }
    if (emptyRoom == -1)    //如果全部非空
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
                while ((tryIndex > 0) && (tryOrder[tryIndex] != p[lastBack.room])) //p[lastBack.room]为最后一个有备选项的位置填入的块号
                {
                    for (char i = 1; i < GAMEBLOCKS; i++)
                    {
                        if (p[i] == tryOrder[tryIndex]) //找到该块所填充的位置，并作废，同时从试探集中也作废该块。
                        {
                            printf("x%d ", tryOrder[tryIndex]);
                            p[i] = -1;
                            tryOrder[tryIndex] = -1;
                            tryIndex--;
                            break;
                        }
                    }
                }
                p[lastBack.room] = lastBack.neighbor;//填入新试探块
                char lastRoom = lastBack.room;
                char lastNeighbor = lastBack.neighbor;
                printf("%d <= %d\n\r", lastBack.room, lastBack.neighbor);
                backList.pop_back();    //弹出 作废该试探块
                lastBack = backList.back();
                if (lastBack.room == lastRoom) //本位还有备选，本位在试探列表还要保留，否则，删掉
                {
                    // tryIndex ++;
                }
                tryOrder[tryIndex] = lastNeighbor;//新试探必须也填入

            }
            else//无备选无邻居,重新开始
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