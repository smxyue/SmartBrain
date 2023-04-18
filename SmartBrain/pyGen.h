#pragma once
#define POP_SIZE 100
#define MUTATION_RATE 0.2
#define MAX_ITERATION 1000
#define CROSSOVER_RATE 0.8

class pyGen
{
	int  pool[50];
    int  population[POP_SIZE][10];
    double wanted;
    double fitness[POP_SIZE];

	void initPool();
	void getGroup(int *);
    void initPopulation();
    void getFitness();
    void select();
    void variation();
public:
    int main();
    void randTry();
};



/*
#date:2021-07-07
#author:会武术之白猫
#theme:遗传算法实例

import random

def create_answer(numbers_set,n):
    result=[]
    for i in range(n):
        result.append(random.sample(numbers_set,10))
    return result

def error_level(new_answer,numbers_set):
    error=[]
    right_answer=sum(numbers_set)/10
    for item in new_answer:
        value=abs(right_answer-sum(item))
        if value==0:
            error.append(10)
        else:
            error.append(1/value)
    return error

def choice_selected(old_answer,numbers_set):
    result=[]
    error=error_level(old_answer,numbers_set)
    error_one=[item/sum(error) for item in error]
    for i in range(1,len(error_one)):
        error_one[i]+=error_one[i-1]
    for i in range(len(old_answer)//2):
        temp=[]
        for j in range(2):
            rand=random.uniform(0,1)
            for k in range(len(error_one)):
                if k==0:
                    if rand<error_one[k]:
                        temp.append(old_answer[k])
                else:
                    if rand>=error_one[k-1] and rand<error_one[k]:
                        temp.append(old_answer[k])
        rand=random.randint(0,6)
        temp_1=temp[0][:rand]+temp[1][rand:rand+3]+temp[0][rand+3:]
        temp_2=temp[1][:rand]+temp[0][rand:rand+3]+temp[1][rand+3:]
        result.append(temp_1)
        result.append(temp_2)
    return result

def variation(old_answer,numbers_set,pro):
    for i in range(len(old_answer)):
        rand=random.uniform(0,1)
        if rand<pro:
            rand_num=random.randint(0,9)
            old_answer[i]=old_answer[i][:rand_num]+random.sample(numbers_set,1)+old_answer[i][rand_num+1:]
    return old_answer

numbers_set=random.sample(range(0,1000),50)
middle_answer=create_answer(numbers_set,100)
first_answer=middle_answer[0]
great_answer=[]
for i in range(1000):
    middle_answer=choice_selected(middle_answer,numbers_set)
    middle_answer=variation(middle_answer,numbers_set,0.1)
    error=error_level(middle_answer,numbers_set)
    index=error.index(max(error))
    great_answer.append([middle_answer[index],error[index]])
great_answer.sort(key=lambda x:x[1],reverse=True)
print("正确答案为",sum(numbers_set)/10)
print("给出的最优解为",great_answer[0][0])
print("该和为",sum(great_answer[0][0]))
print("选择系数为",great_answer[0][1])
print("最初解的和为",sum(first_answer))
*/