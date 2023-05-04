/*
2021/08/24 AM-00:16
�I�u�W�F�N�g�̐�����}���邽�߁A�V���O���g���̎��������݂Ă���B

�V���O���g���̃e���v���[�g���Q�l�ɂ����B
https://qiita.com/narumi_/items/b205e59e7fc81695f380
*/

#pragma once

template<class T>
class Singleton
{
public:
	static inline T& GetInstance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	void operator=(const Singleton& obj) {}	// ������Z�q�֎~
	Singleton(const Singleton& obj) {}		// �R�s�[�R���X�g���N�^�֎~
};