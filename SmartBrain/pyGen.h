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
def create_answer(number_set, n) : # number_setΪ���飬 nΪ�����ĸ�����Ŀ
result = []
for i in range(n) :
    result.append(random.sample(number_set, 10))
    return result
    #����error��ѡ�񽻻�����
    def error_level(new_answer, numbers_set) : # new_answerΪ���и���
    error = []
    right_answer = sum(numbers_set) / 10
    for item in new_answer :
value = abs(right_answer - sum(item))
if value == 0 :
    error.append(10)# value��СֵΪ0.1, 1 / valueΪ10��������ڻ����10����
else :
    error.append(1 / value) # valueԽСԽ�ã���errorԽ��
    return error

    # ������Ϣ
    def choice_selected(old_answer, numbers_set) :
    result = [] # ����һ�η��ܵ������Ӵ�
    error = error_level(old_answer, numbers_set)
    error_one = [item / sum(error) for item in error]
    for i in range(1, len(error_one)) :
        error_one[i] += error_one[i - 1]

        for i in range(len(old_answer)//2): #���ܴ���
            temp = []#����ÿ�γ�ȡ�ĸ�����ĸ��
            for j in range(2) :#��ȡ������ĸ��
                rand = random.uniform(0, 1)
                for k in range(len(error_one)) :
                    if k == 0 :
                        if rand < error_one[k] :
                            temp.append(old_answer[k])
                        else :
                            if rand >= error_one[k - 1] and rand < error_one[k] :
                                temp.append(old_answer[k])
                                rand = random.randint(0, 6) # ѡ��������3������Ϊ������һ��10����
                                temp_1 = temp[0][:rand] + temp[1][rand:rand + 3] + temp[0][rand + 3:]
                                temp_2 = temp[1][:rand] + temp[0][rand:rand + 3] + temp[1][rand + 3:]
                                if len(set(temp_1)) == 10 and len(set(temp_2)) == 10: # �ж��Ӵ��Ƿ������ͬԪ�أ������򱣴游����ĸ��
                                    result.append(temp_1)
                                    result.append(temp_2)
                                else:
result.append(temp[0])
result.append(temp[1])
return result

def variation(old_answer, numbers_set, pro) : # proΪ��������ĸ���
for i in range(len(old_answer)) :
    rand = random.uniform(0, 1)
    if rand < pro :
        rand_num = random.randint(0, 9)
        new_numbers_set = [i for i in numbers_set if i not in old_answer] # ������������������������ͬ
        old_answer[i] = old_answer[i][:rand_num] + random.sample(new_numbers_set, 1) + old_answer[i][rand_num + 1:]
        return old_answer

        numbers_set = random.sample(range(0, 1000), 50) # ��0��1000�����ѡȡ50������Ϊһ������
        middle_answer = create_answer(numbers_set, 100)# �������100������
        first_answer = middle_answer[0]
        great_answer = [] # ��������ÿһ����õĸ���
        for i in range(1000) :# ������1000��
            middle_answer = choice_selected(middle_answer, numbers_set) # ������Ϣ��ĸ���
            middle_answer = variation(middle_answer, numbers_set, 0.1) # �����������
            error = error_level(middle_answer, numbers_set) # ����ÿ�������Ԫ�غ��������1 / 10��error
            index = error.index(max(error)) # ����error��������±꣨errorԽ��Խ�ã������н��ͣ�
            great_answer.append([middle_answer[index], error[index]])# ������һ����õĸ����error
            great_answer.sort(key = lambda x : x[1], reverse = True)# ����error�Ӵ�С����
            print("��ȷ��Ϊ", sum(numbers_set) / 10)
            print("���������Ž�Ϊ", great_answer[0][0])
            print("�ú�Ϊ", sum(great_answer[0][0]))
            print("ѡ��ϵ��Ϊ", great_answer[0][1])
*/