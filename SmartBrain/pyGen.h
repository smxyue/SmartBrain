#pragma once
class pyGen
{
	int static pool[50];
    int population[100][10];
	void initPool();
	void getGroup(int *);
};



/*
https://zhuanlan.zhihu.com/p/563042544
import random
def create_answer(number_set, n) : # number_set为数组， n为创建的个体数目
result = []
for i in range(n) :
    result.append(random.sample(number_set, 10))
    return result
    #计算error，选择交换个体
    def error_level(new_answer, numbers_set) : # new_answer为所有个体
    error = []
    right_answer = sum(numbers_set) / 10
    for item in new_answer :
value = abs(right_answer - sum(item))
if value == 0 :
    error.append(10)# value次小值为0.1, 1 / value为10，这里大于或等于10都行
else :
    error.append(1 / value) # value越小越好，即error越大
    return error

    # 交换信息
    def choice_selected(old_answer, numbers_set) :
    result = [] # 保存一次繁衍的所有子代
    error = error_level(old_answer, numbers_set)
    error_one = [item / sum(error) for item in error]
    for i in range(1, len(error_one)) :
        error_one[i] += error_one[i - 1]

        for i in range(len(old_answer)//2): #繁衍次数
            temp = []#保存每次抽取的父本和母本
            for j in range(2) :#抽取父本和母本
                rand = random.uniform(0, 1)
                for k in range(len(error_one)) :
                    if k == 0 :
                        if rand < error_one[k] :
                            temp.append(old_answer[k])
                        else :
                            if rand >= error_one[k - 1] and rand < error_one[k] :
                                temp.append(old_answer[k])
                                rand = random.randint(0, 6) # 选择连续的3个数作为交换，一共10个数
                                temp_1 = temp[0][:rand] + temp[1][rand:rand + 3] + temp[0][rand + 3:]
                                temp_2 = temp[1][:rand] + temp[0][rand:rand + 3] + temp[1][rand + 3:]
                                if len(set(temp_1)) == 10 and len(set(temp_2)) == 10: # 判断子代是否具有相同元素，若有则保存父本和母本
                                    result.append(temp_1)
                                    result.append(temp_2)
                                else:
result.append(temp[0])
result.append(temp[1])
return result

def variation(old_answer, numbers_set, pro) : # pro为产生变异的概率
for i in range(len(old_answer)) :
    rand = random.uniform(0, 1)
    if rand < pro :
        rand_num = random.randint(0, 9)
        new_numbers_set = [i for i in numbers_set if i not in old_answer] # 变异的数不能与个体其他数相同
        old_answer[i] = old_answer[i][:rand_num] + random.sample(new_numbers_set, 1) + old_answer[i][rand_num + 1:]
        return old_answer

        numbers_set = random.sample(range(0, 1000), 50) # 在0到1000中随机选取50个数作为一个数组
        middle_answer = create_answer(numbers_set, 100)# 随机创建100个个体
        first_answer = middle_answer[0]
        great_answer = [] # 用来保存每一代最好的个体
        for i in range(1000) :# 共繁衍1000代
            middle_answer = choice_selected(middle_answer, numbers_set) # 交换信息后的个体
            middle_answer = variation(middle_answer, numbers_set, 0.1) # 个体产生变异
            error = error_level(middle_answer, numbers_set) # 计算每个个体的元素和与数组和1 / 10的error
            index = error.index(max(error)) # 保存error最大个体的下标（error越大越好，后面有解释）
            great_answer.append([middle_answer[index], error[index]])# 保存这一代最好的个体和error
            great_answer.sort(key = lambda x : x[1], reverse = True)# 按照error从大到小排序
            print("正确答案为", sum(numbers_set) / 10)
            print("给出的最优解为", great_answer[0][0])
            print("该和为", sum(great_answer[0][0]))
            print("选择系数为", great_answer[0][1])
*/