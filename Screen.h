#define _CRT_SECURE_NO_WARNINGS

#pragma once
#include <iostream>
#include <Windows.h>
#include "Utils.h"

#define N_LANDMINE 10

using namespace std;

class Screen
{
	char* lines;//����, ���, ���� ������ ������ ���� �迭
	char* answers;//ȭ�� �Ʒ��� ����� ��� �迭, ������ ��ġ ���
	char* scene;//�÷��̾�� ������ �迭
	char* isVisted;//�̹� üũ�� ��ġ������ �����ϴ� �迭

	int countFlag = N_LANDMINE;//���� ����� ����
	int correctFlag = 0;//������ġ�� �ùٸ��� ���� ���
	int trapidx = 0;//������ ��ġ �ε���

	

public:
	int n_rows;
	int n_cols;

	Screen(int n_rows, int n_cols)
		: n_rows(n_rows), n_cols(n_cols),
		lines(new char[sizeof(char)* n_rows * n_cols + 1]),
		answers(new char[n_rows * (n_cols + 1)]),
		scene(new char[n_rows * (n_cols + 1)]),
		isVisted(new char[n_rows * (n_cols + 1)])
	{
		Borland::Initialize();
		clear();
	}

	virtual ~Screen() 
	{ 
	delete[] lines;
	delete[] answers;
	delete[] scene;
	delete[] isVisted;
	}

	int pos2offset(int x, int y, int n_cols)
	{
		return y * (n_cols + 1) + x;
	}
	

	void draw(const Position& pos, char shape)
	{
		int offset = pos2offset(pos.x, pos.y, n_cols);

		scene[offset] = shape;
	}

	void render()
	{
		Borland::GotoXY(0, 0);
		for (int i = 0; i < n_rows; i++)
			lines[i * n_cols + n_cols - 1] = '\n';
		lines[n_rows * n_cols] = '\0';
		printf("%s", lines);
		Sleep(100);
	}

	auto checkValidPos(const Position& pos) const
	{
		return (pos.x >= 0 && pos.x < n_cols - 1
			&&  pos.y >= 0 && pos.y < n_rows - 1);
	}

	void clear()
	{
		memset(lines, 'X', n_rows * (n_cols + 1));
		memset(answers, 'X', n_rows * (n_cols + 1));
		memset(isVisted, 'N', n_rows * (n_cols + 1));

		for (int i = 0; i < n_rows; i++)
		{
			lines[i * (n_cols + 1) + n_cols] = '\n';
			answers[i * (n_cols + 1) + n_cols] = '\n';
			isVisted[i * (n_cols + 1) + n_cols] = '\n';

		}
		lines[n_rows * (n_cols + 1)] = '\0';
		answers[n_rows * (n_cols + 1)] = '\0';
		isVisted[n_rows * (n_cols + 1)] = '\0';


	}
	void clearscene()//���� ȭ�� �ʱ�ȭ
	{
		memset(scene, 'X', n_rows * (n_cols + 1));
		for (int i = 0; i < n_rows; i++)
		{
			scene[i * (n_cols + 1) + n_cols] = '\n';
		}
		scene[n_rows * (n_cols + 1)] = '\0';
	}

	void setLandmine(char shape)//���� ��ġ �Լ�
	{
		for (int i = 0; i < N_LANDMINE; i++)
		{
			trapidx = createLandmine();

			if (answers[trapidx] == shape)
				--i;
			else
			{
				answers[trapidx] = shape;
			}

		}
	}

	int createLandmine()//�������� ���ڸ� �����ϴ� �Լ�
	{
		// ���ѷ��� ���� ������ ������ �ΰ�, ����� ������ +1, �����Ѱ� ������ �Ѿ ����
		srand(time(NULL));
		int x = rand() % n_rows;
		int y = rand() % n_cols;
		int trapOffset = pos2offset(x, y, n_cols);
		return trapOffset;//������ ������ ��ġ���� ��ȯ


	}

	int countTrap(int idx)//���� ��ġ �߽����� �ֺ� 8ĭ�� ���� ������ ���� �Լ�
	{
		int count = 0;

		for (int j = idx - 12; j < idx - 9; j++)//���� ������ �˻�
		{
			if (j > -1 && answers[j] == '*')
			{
				count++;
			}
		}
		for (int j = idx - 1; j < idx + 2; j = j + 2)//��� ��
		{
			if (j > -1 && answers[j] == '*')
			{
				count++;
			}
		}
		for (int j = idx + 10; j < idx + 13; j++)//�� �Ʒ���
		{
			if (j > -1 && answers[j] == '*')
			{
				count++;
			}
		}
		return count;
	}

	void setNumber()//�ֺ� ������ ������ �迭�� �����س��� �Լ�
	{
		for (int i = 0; i < n_rows * (n_cols + 1); i++)
		{
			if (answers[i] == '*' || answers[i] == '\n')
				continue;
			else
			{
				int count = countTrap(i);
				answers[i] = count + '0';
			}
		}


	}

	void mapOpen(int idx)//���� ���� �Լ�
	{

		if (isVisted[idx] == 'N' && isVisted[idx] != '\n')//�̹� üũ�ߴ��� �˻�
		{
			isVisted[idx] = 'Y';
			if (answers[idx] == '0')//�ֺ� ���� ������ �����ϴ� �迭 answers�� �ش� ��ġ�� 0�̸�
			{
				for (int j = idx - 12; j < idx - 9; j++)//���� ������ �ֺ� 8�ڸ��� �˻�
				{
					if (j > -1)//�ε����� ���� �ȿ� �ִ��� Ȯ��
					{
						mapOpen(j);//����������� �ڱ��ڽ� ȣ��
					}

				}
				for (int j = idx - 1; j < idx + 2; j = j + 2)//��� �� �� �� �˻�
				{
					if (j > -1)
					{
						mapOpen(j);
					}

				}
				for (int j = idx + 10; j < idx + 13; j++)//�������� �˻�
				{
					if (j > -1)
					{
						mapOpen(j);
					}

				}
				lines[idx] = answers[idx];//�ڱ� �ڽ��� ��ġ ����
			}
			else//answers�� 0�� �ƴϸ�
			{
				if (answers[idx] != '*' && answers[idx] != '\n')//�������� �ٹٲ޹������� �˻�
				{
					lines[idx] = answers[idx];//answers�� 0�� �ƴϰ� ����/�ٹٲ� ���ڰ� �ƴϸ� 1�̻��� �����̹Ƿ�
				}
			}
		}
	}

	void renderLines()//������ �迭�� ����ϱ� ���� �Լ�, Ȯ�ο�
	{
		for (int i = 0; i < n_rows - 1; i++)
		{
			lines[i * (n_cols + 1) + n_cols] = '\n';
		}
		lines[(n_cols + 1) * n_rows] = '\0';

		Borland::GotoXY(0, 25);
		printf("%s", lines);
		printf("\n");
		printf("%s", isVisted);


	}
	void renderScene()//���� ȭ�� ��� �Լ�
	{
		for (int i = 0; i < n_rows - 1; i++)
		{
			scene[i * (n_cols + 1) + n_cols] = '\n';
		}
		scene[(n_cols + 1) * n_rows] = '\0';

		Borland::GotoXY(0, 0);
		printf("%s", scene);
		printf("���� ��� ����: %d  ", countFlag);
		printf("�� ����:M, ���:F ");


	}


	void renderAnswer()//���� ��ġ�� �ֺ� ���� ������ ������ �迭 ����ϴ� �Լ�, Ȯ�ο�
	{
		for (int i = 0; i < n_rows - 1; i++)
		{
			answers[i * (n_cols + 1) + n_cols] = '\n';
		}
		answers[(n_cols + 1) * n_rows] = '\0';
		Borland::GotoXY(0, 20);
		printf("%s", answers);
	}



	void gameOver()
	{
		Borland::GotoXY(0, 15);
		printf("GAME OVER");
		exit(0);
	}
	void gameClear()
	{
		Borland::GotoXY(15, 0);
		printf("GAME CLEAR");
	}
	void checkTrap(const Position& pos)//���ڿ��� ���� �����ϸ� ���� ����
	{
		if (answers[pos2offset(pos.x, pos.y, n_cols)] == '*')
		{
			gameOver();
		}
	}
	void setFlag(const Position& pos)//��� ���
	{
		bool isFlag = false;
		int idx = pos2offset(pos.x, pos.y, n_cols);
		lines[idx] = 'F';

	}
	void isFlag(const Position& pos)
	{
		bool isflag = false;
		int idx = 0;
		idx = pos2offset(pos.x, pos.y, n_cols);
		if (lines[idx] == 'F')//����� ������
		{
			countFlag++;

			lines[idx] = '0';//��� ȸ��
		}
		else//����� ������
		{
			countFlag--;

			lines[idx] = 'F';//��� ����
			isflag = true;
		}
	}
	void checkFlag(const Position& pos)//����� �ùٸ��� �������� �˻�
	{
		Borland::GotoXY(0, 10);
		int idx = 0;
		idx = pos2offset(pos.x, pos.y, n_cols);
		if (correctFlag == N_LANDMINE)
			gameClear();
		if (answers[idx] == '*')
		{
			correctFlag++;
		}
	}
	void screencopy()
	{
		strcpy(scene, lines);
	}

};

