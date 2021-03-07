#ifndef ___Class_Song
#define ___Class_Song

#include"bar.h"
#include<vector>
#include<iostream>
#include<math.h>

extern const double critical;
extern const double effect;
extern const double zonewidth;

class song
{
	//���ʎ�荞�݂ɕK�v�ȃf�[�^
	int bpm;
	int measure;
	
	bool soflan_head;
	bool soflan_mid;
	bool cn_ch5;
	bool cn_lnobj;
	bool stop;
	bool mine;

	//#BPMbb xx�̑Ή��\�����
	std::vector<int> bpm_table1;//bb
	std::vector<int> bpm_table2;//xx

	//#LNOBJ ZZ
	std::string lnobj;

	//���ʂ���ǂݏo���f�[�^
	int notes[9];
	double time;
	//density���ρi�O�͔����Ă���j
	double density;

	//difficulty����
	double difave;
	//difficulty�W���΍�
	double difsdv;
	//�L��difficulty�i�Y�ꂽ�j
	double difeff;
	//difficulty��bandwidth���݂ŋ敪�������Ƃ��̏��ߐ�
	std::vector<int> difzone;
	//difzone�̓Y�����Ƃ����`
	std::vector<int>::size_type difmain;

public:

	//�N���Xbar�^����ׂ��x�N�g���B���ʂ��i�[����
	std::vector<bar*> bars;

	//�R���X�g���N�^
	song(){
		bpm = 0;
		measure = 0;
		soflan_head = 0;
		soflan_mid = 0;
		cn_ch5 = 0;
		cn_lnobj = 0;
		stop = 0;
		mine = 0;
		lnobj = "ZZZZ";
	}

	//private�f�[�^�����o�̃Z�b�^�ƃQ�b�^
	void set_bpm(int i) { bpm = i; }
	int get_bpm() const { return bpm; }
	void set_measure(int i){ measure = i; }
	int get_measure() const { return measure; }

	void set_bpm_table(int i, int j) { 
		bpm_table1.push_back(i);
		bpm_table2.push_back(j);
	}
	int get_bpm_table(int j) {
		for (std::vector<int>::size_type i = 0; i < bpm_table1.size(); i++){
			if (j == bpm_table1.at(i))return bpm_table2.at(i);
		}
		return 10000;
	}

	void set_lnobj(std::string str){ lnobj = str; }
//	std::string get_lnobj() { return lnobj; }

	void soflan_head_on(){ soflan_head = 1; }
	void soflan_mid_on(){ soflan_mid = 1; }
	void cn_ch5_on(){ cn_ch5 = 1; }
	void cn_lnobj_on(){ cn_lnobj; }

	//
	//�ǂݍ��݊֐�
	//
	//�g��bpm�̕ϊ�
	void convert_bpm();
	//bars���������imeasure�ˑ��j
	void set_bars_init();

	//""���ׂĂ�""bars.fumen���������ibars.beat�ˑ��j
	void set_fumen_init();

	//���ʃC���v�b�g�B���ߐ���ǂ�œn�������B
	void input_fumen(std::string);

	//
	void complement_joint();

	//
	//��͊֐�
	//
	void read_all(){
		for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
			bars.at(i)->read_all();
		}
		read_notes();
		read_time();
		read_density();
		read_difave();
		read_difsdv();

		read_difzone();
		read_difmain();

		read_difeff();
	}

	//�L�[�ʃm�[�c���ƑS�̃m�[�c�����I�[�o�[���[�h
	void read_notes();
	int get_notes(int i){ return notes[i]; }

	//�Ȃ̎���
	void read_time();
	double get_time(){ return time; }

	//bars.density�̏d�ݕt�����ρB�Ƃ肠����0�͏Ȃ��Ă�B
	void read_density();
	double get_density(){ return density; }

	//
	//���v�v�Z
	//
	//difficulty����
	void read_difave();
	double get_difave(){ return difave; }

	//difficulty�W���΍�
	void read_difsdv();
	double get_difsdv(){ return difsdv; }
	//difficulty�����X�g�ɂ��ċL�q
	std::string output_dif_list();

	//�L��difficulty
	void read_difeff();
	double get_difeff(){ return difeff; }

	//difficulty�]�[������
	void read_difzone();
	std::string write_difzone_list();

	//���C��difficulty�]�[��
	void read_difmain();
	std::vector<int>::size_type get_difmain(){return difmain;}

	
	//�������牺������
	void read_difmain3();
	
	double difeff1(){
		double bunshi = 0;
		double time = 0;
		for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
			//�����s����
			if (bars.at(i)->get_difficulty() > difave){
				bunshi += bars.at(i)->get_difficulty() * bars.at(i)->get_time();
				time += bars.at(i)->get_time();
			}
		}
		return bunshi / time;
	}

	double difeff2(){
		double bunshi = 0;
		double time = 0;
		for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
			//�����s����
			if (bars.at(i)->get_difficulty() > (difave-difsdv)){
				bunshi += bars.at(i)->get_difficulty() * bars.at(i)->get_time();
				time += bars.at(i)->get_time();
			}
		}
		return bunshi / time;
	}

	double difeff3(){
		double bunshi = 0;
		double time = 0;
		for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
			//�����s����
			if (bars.at(i)->get_difficulty() > zonewidth * difmain){
				bunshi += bars.at(i)->get_difficulty() * bars.at(i)->get_time();
				time += bars.at(i)->get_time();
			}
		}
		return bunshi / time;
	}

	double difeff4(){
		double bunshi = 0;
		double time = 0;
		std::vector<int>::size_type counter = 0;
		std::vector<int>::size_type j = difzone.size() - 1;
		for (; j > 0; j--){
			counter += difzone.at(j);
			if (counter >= static_cast<std::vector<int>::size_type>(measure / 2))
				break;
		}
		for (std::vector<bar>::size_type i = 0; i < static_cast<std::vector<bar>::size_type>(measure); i++){
			//�����s����
			if (bars.at(i)->get_difficulty() > zonewidth * j){
				bunshi += bars.at(i)->get_difficulty() * bars.at(i)->get_time();
				time += bars.at(i)->get_time();
			}
		}
		return bunshi / time;
	}


	//�f�o�b�O�p�֐�
//	std::vector<bool>::size_type aaa(std::vector<bar>::size_type i){return bars[i].aaaa(); }

};







#endif