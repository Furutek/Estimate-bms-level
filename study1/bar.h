#ifndef ___Class_Bar
#define ___Class_Bar

#include<iostream>
#include<vector>
#include<string>

class song;

extern const int resolution;

class bar
{
	friend song;//�J�����Ƃ���t�����h�ɂ�������A�]�v�ȃZ�b�^��Q�b�^���������񂠂�B

protected:

	std::vector<int> bpm;
	std::vector<int> bpm_tick;

	double beat = 1.0;

	//fumen[key].at(tick) = �m�[�c�L��
	std::vector<bool> fumen[8];
	//cn�̎d�l�́A(swi, sus)���A(0,0)==�����Ȃ�,(1,1)==on,(1,0)==��������,(0,1)==off
	std::vector<bool> cn_swi[8];
	std::vector<bool> cn_sus[8];


	//���ʂ���ǂݏo���f�[�^
	int notes[9];//notes[0~7]�͌��Ֆ��́Anotes[8]�͏��߂̃m�[�c��
	double time;
	double density;
	double difficulty;

public:

	//�R���X�g���N�^
	bar(){
		beat = 1.0;
		notes[0] = 0;
		notes[1] = 0;
		notes[2] = 0;
		notes[3] = 0;
		notes[4] = 0;
		notes[5] = 0;
		notes[6] = 0;
		notes[7] = 0;
		notes[8] = 0;
	}

	//�Q�b�^�Z�b�^
	void set_bpm(int b, int t) {//����ȂɕK�v����֐�����Ȃ�
		bpm.push_back(b);
		bpm_tick.push_back(t);
	}
	int bpm_ttob(int t){
		for (std::vector<int>::size_type i = 0; i < bpm.size(); i++){
			if (bpm_tick.at(i) == t)
				return bpm.at(i);
		}
		return 0;
	}
	int bpm_last(){
		int tick = 0;
		for (std::vector<int>::size_type i = 0; i < bpm.size(); i++){
			if (bpm_tick.at(i) > tick) tick = bpm_tick.at(i);
		}
		return bpm_ttob(tick);
	}

	void set_beat(double d) { beat = d; }
	double get_beat() { return  beat; }//�g���́H


	//
	//�ǂݍ��݊֐�
	//
	//���̏��߂�fumen���������ibeat�j�ˑ�
	void set_fumen_init();

	//���ʎ�荞�ݗp�֐�
	virtual void input_fumen(std::string, std::string);


	//
	//��͊֐�
	//
	void read_all(){
		read_notes();
		read_time();
		read_density();
		read_difficulty();
	}
	//�L�[�ʃm�[�c���ƁA�S�̃m�[�c��
	void read_notes();
	int get_notes(int i){ return notes[i]; }

	//time�͂��̏��߂ł̎���[sec]
	void read_time();
	double get_time(){ return time; }
	//density�͂��̏��߂ł�[notes/sec]�Ɍ��߂��B��������̕��������H
	void read_density(){ density = 1.0 * notes[8] / time; }
	double get_density(){ return density; }

	//density�ɕ␳��������`��difficulty���v�Z�B
	void read_difficulty();
	double get_difficulty(){ return difficulty; }



	//�f�o�b�O�p�֐�
	std::string aaaa(){
		std::vector<bool>::size_type i = 0;
		std::string str;
		for (; i < fumen[0].size(); i++){
			str += to_string(fumen[4].at(i));
		}
		return str;
	}
	double aaaaa() { return beat; }
};






/*�ŏ��̓\�t������LN�͔h���N���X������\�肾�������ǂ�߂��B

class bar_sof1 : public bar{

	public:
	void input_fumen(std::string);

};


class bar_sof2 : public bar{
	std::vector<int> bpm_tick;
	std::vector<int> bpm_value;
public:
	void input_fumen(std::string);
	void read_time();
	void read_difficulty();
};
*/



#endif